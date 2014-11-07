/*
 * This file is part of ExpertSDR
 *
 * ExpertSDR is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * ExpertSDR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 *
 *
 * Copyright (C) 2014 Tsukihime
 */

#ifndef PLUGINCTRL_H
#define PLUGINCTRL_H

#include <QtGui>
#include "qt_windows.h"
#include "extioplugin.h"

enum StateChgReason {
    crPtt = 0,
    crDash = 1,
    crDot = 2,
    crAdc = 3
};

typedef __cdecl void (*CallbackFunc_SdrStateChanged)(QObject *PlugCtrl, StateChgReason reason, bool arg1, int arg2, int arg3);
typedef __cdecl void (*PluginFunc_getInfo)(char*);
typedef __cdecl void (*PluginFunc_showPluginGui)();

typedef __cdecl void (*PluginFunc_init)(QObject *handle, CallbackFunc_SdrStateChanged func);
typedef __cdecl void (*PluginFunc_deinit)();
typedef __cdecl void (*PluginFunc_open)(int, int);
typedef __cdecl void (*PluginFunc_close)();
typedef __cdecl bool (*PluginFunc_isOpen)();
typedef __cdecl void (*PluginFunc_setPreamp)(int);
typedef __cdecl void (*PluginFunc_setExtCtrl)(DWORD);
typedef __cdecl void (*PluginFunc_setDdsFreq)(int);
typedef __cdecl void (*PluginFunc_setTrxMode)(bool);
typedef __cdecl void (*PluginFunc_setMute)(bool);
typedef __cdecl void (*PluginFunc_setVhfOsc)(unsigned int);
typedef __cdecl void (*PluginFunc_setUhfOsc)(unsigned int);
typedef __cdecl void (*PluginFunc_setCalGen)(bool);
typedef __cdecl void (*PluginFunc_setXvAnt)(int);

typedef struct
{
    PluginFunc_getInfo getInfo;
    PluginFunc_showPluginGui showPluginGui;
    PluginFunc_init init;
    PluginFunc_deinit deinit;
    PluginFunc_open open;
    PluginFunc_close close;
    PluginFunc_isOpen isOpen;
    PluginFunc_setPreamp setPreamp;
    PluginFunc_setExtCtrl setExtCtrl;
    PluginFunc_setDdsFreq setDdsFreq;
    PluginFunc_setTrxMode setTrxMode;
    PluginFunc_setMute setMute;
    PluginFunc_setVhfOsc setVhfOsc;
    PluginFunc_setUhfOsc setUhfOsc;
    PluginFunc_setCalGen setCalGen;
    PluginFunc_setXvAnt setXvAnt;
} InternalPluginRouts;

class pluginCtrl : public ExtIOPlugin
{
    Q_OBJECT

public:

    pluginCtrl(QString libpath);
    ~pluginCtrl();

    void init();
    void deinit();
    void open(int SdrNum, int DdsMul);
    void close();
    bool isOpen();
    void setPreamp(int Preamp);
    void setExtCtrl(DWORD ExtData);
    void setDdsFreq(int Freq);
    void setTrxMode(bool Mode);
    void setMute(bool Status);
    void setVhfOsc(quint32 freq);
    void setUhfOsc(quint32 freq);
    void setCalGen(bool Mode);
    void setXvAnt(int Mode);

    static bool getInfo(QString libpath, QString &PlugName);
    bool isLoaded();
    void showPluginGui();

private:
    QLibrary *pPlugLib;
    bool pluginLoaded;

    int DdsFreq;
    int SampleRate;
    HwModeRxTx rxtx_mode;

    InternalPluginRouts routs;

    static __cdecl void SdrStateChanged(QObject *PlugCtrl, StateChgReason reason, bool arg1, int arg2, int arg3);

public slots:
    void SoundCardSampleRateChanged(int rate);

signals:
    void PttChanged(bool);
    void DashChanged(bool);
    void DotChanged(bool);
    void AdcChanged(int , int);
};

#endif // PLUGINCTRL_H
