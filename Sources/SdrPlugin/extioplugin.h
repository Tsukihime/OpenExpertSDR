#ifndef EXTIOPLUGIN_H
#define EXTIOPLUGIN_H

#include <QObject>
#include <QtGui>
#include "Defines.h"

typedef __stdcall void (*ExtIO_ShowGUI)();
typedef __stdcall bool (*ExtIO_InitHW)(char*, char*, int&);
typedef __stdcall bool (*ExtIO_OpenHW)();
typedef __stdcall void (*ExtIO_CloseHW)();
typedef __stdcall int (*ExtIO_StartHW)(long);
typedef __stdcall void (*ExtIO_StopHW)();
typedef __stdcall int (*ExtIO_SetHWLO)(long);
typedef __stdcall int (*ExtIO_GetStatus)();
typedef __stdcall void (*ExtIO_SetCallback)(void (*)(int, int, float, short*));
// ext routs
typedef __stdcall long (*ExtIO_GetHWLO)();
typedef __stdcall void (*ExtIO_TuneChanged)(long);
typedef __stdcall void (*ExtIO_IFLimitsChanged)(long, long);
typedef __stdcall long (*ExtIO_GetTune)();
typedef __stdcall char (*ExtIO_GetMode)();
typedef __stdcall void (*ExtIO_ModeChanged)(char);
typedef __stdcall long (*ExtIO_GetHWSR)();
typedef __stdcall void (*ExtIO_HideGUI)();
typedef __stdcall void (*ExtIO_RawDataReady)(long, int*, int*, int);
typedef __stdcall void (*ExtIO_GetFilters)(int&, int&, int&);
//
typedef __stdcall int (*ExtIOext_SetModeRxTx)(int);

//GetStatus GetHWSR, HideGUI, RawDataReady, GetFilters are not implemented

typedef struct
{
    ExtIO_ShowGUI ShowGUI;
    ExtIO_InitHW InitHW;
    ExtIO_OpenHW OpenHW;
    ExtIO_CloseHW CloseHW;
    ExtIO_StartHW StartHW;
    ExtIO_StopHW StopHW;
    ExtIO_SetHWLO SetHWLO;
    ExtIO_GetStatus GetStatus;
    ExtIO_SetCallback SetCallback;

    ExtIO_GetHWLO GetHWLO;
    ExtIO_TuneChanged TuneChanged;
    ExtIO_IFLimitsChanged IFLimitsChanged;
    ExtIO_GetTune GetTune;
    ExtIO_GetMode GetMode;
    ExtIO_ModeChanged ModeChanged;
    ExtIO_GetHWSR GetHWSR;
    ExtIO_HideGUI HideGUI;
    ExtIO_RawDataReady RawDataReady;
    ExtIO_GetFilters GetFilters;

    ExtIOext_SetModeRxTx SetModeRxTx;
} ExtIORouts;

enum ExtIOCallbackStatus {
    // for all extIO's
    ecsChangedSampleRate    = 100   // sampling speed has changed in the external HW
    , ecsChangedLO          = 101   // LO frequency has changed in the external HW
    , ecsLockLO             = 102
    , ecsUnlockLO           = 103
    , ecsChangedLONotTUNE   = 104   // CURRENTLY NOT YET IMPLEMENTED
    // LO freq. has changed, Winrad must keep the Tune freq. unchanged
    // (must immediately call GetHWLO() )
    , ecsChangedTUNE        = 105   // a change of the Tune freq. is being requested.
    // Winrad must call GetTune() to know which value is wanted
    , ecsChangedMODE        = 106   // a change of demod. mode is being requested.
    // Winrad must call GetMode() to know the new mode
    , ecsStart              = 107   // The DLL wants Winrad to Start
    , ecsStop               = 108   // The DLL wants Winrad to Stop
    , ecsChangedFILTER      = 109   // a change in the band limits is being requested
    // Winrad must call GetFilters()

    , ecsTXRequest          = 120   // DLL requests TX mode / User pressed PTT
    //   exciter/transmitter must wait until SetModeRxTx() is called!
    , ecsRXRequest          = 121   // DLL wants to leave TX mode / User released PTT
    //   exciter/transmitter must wait until SetModeRxTx() is called!
};

typedef enum
{
    hmRX  = 0,
    hmTX  = 1
} HwModeRxTx;

class ExtIOPlugin : public QObject
{
    Q_OBJECT

protected:
    ExtIOPlugin();
    void ExtIOPluginInit(QLibrary *plugin);
    void ExtIOPluginDeinit();

    void ShowGUI();
    bool OpenHW();
    void CloseHW();
    int StartHW(long freq);
    void StopHW();
    int SetHWLO(long LOfreq);

    long GetHWLO();
    void IFLimitsChanged(long low, long high);
    long GetTune();
    SDRMODE GetMode();

    void SetModeRxTx(bool mode);

    bool IsExtIOMode();
    bool IsExtIOOpen();

public:
    QString getInfoStr();
    static bool IsExtIO(QLibrary *plugin);

protected:
    QString InfoStr;

private:
    bool ExtIOMode;
    bool EXtIOOpen;
    int SampleRate;
    int sdrmode;

    long dds_f;
    int IFLimitLow;
    int IFLimitHigh;

    ExtIORouts routs;

    bool init_hw();
    void set_callback();
    void ubdate_if_limits();

    static ExtIOPlugin* plugin_instance;

    static __cdecl void extIOCallback(int cnt, int status, float IQoffs, short IQdata[]);

signals:
    void DDSChanged(long freq);
    void ChangeMode(int mode);
    void Start(bool state);
    void TuneChanged(long freq);
    void Ptt(bool);

public slots:
    void OnModeChanged(int mode);
    void OnTuneChanged(int freq);
    void SoundCardSampleRateChanged(int rate);

private slots:
    void OnExtIOCallback(int status);

};

#endif // EXTIOPLUGIN_H