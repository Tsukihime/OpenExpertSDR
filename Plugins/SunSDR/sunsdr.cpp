#include "sunsdr.h"
#include "sunCtrl.h"

/*** service funcs ***********************************************************/
UINT_PTR handle = 0;
CallbackFunc_SdrStateChanged SdrStateChanged = NULL;

void PttChanged(bool KeyPtt)
{
    if(SdrStateChanged && handle)
    {
        SdrStateChanged(handle, crPtt, KeyPtt, 0, 0);
    }
}

void DashChanged(bool KeyDash)
{
    if(SdrStateChanged && handle)
    {
        SdrStateChanged(handle, crDash, KeyDash, 0, 0);
    }
}

void DotChanged(bool KeyDot)
{
    if(SdrStateChanged && handle)
    {
        SdrStateChanged(handle, crDot, KeyDot, 0, 0);
    }
}

void AdcChanged(int Ufwd, int Uref)
{
    if(SdrStateChanged && handle)
    {
        SdrStateChanged(handle, crAdc, false, Ufwd, Uref);
    }
}

sunCtrl* pSunSDR = NULL; // not service

EXPORT void init(UINT_PTR _handle, CallbackFunc_SdrStateChanged func)
{
    handle = _handle;
    SdrStateChanged = func;
    /*** service funcs end***********************************************************/
    pSunSDR = new sunCtrl();
    //pSunSDR->init();
}

EXPORT void getInfo(char* name)
{
    strcpy(name, "SunSDR");
}

EXPORT void deinit()
{
    //pSunSDR->deinit();
    delete pSunSDR;
}

EXPORT void open(int SdrNum, int DdsMul)
{
    pSunSDR->open(SdrNum, DdsMul);
}

EXPORT void close()
{
    pSunSDR->close();
}

EXPORT bool isOpen()
{
    return pSunSDR->isOpen();
}

EXPORT void setPreamp(int Preamp)
{
    pSunSDR->setPreamp(Preamp);
}

EXPORT void setExtCtrl(DWORD ExtData)
{
    pSunSDR->setExtCtrl(ExtData);
}

EXPORT void setDdsFreq(int Freq)
{
    pSunSDR->setDdsFreq(Freq);
}

EXPORT void setTrxMode(bool Mode)
{
    pSunSDR->setTrxMode(Mode);
}

EXPORT void setMute(bool Status)
{
    pSunSDR->setMute(Status);
}

EXPORT void setVhfOsc(unsigned int freq)
{
    pSunSDR->setVhfOsc(freq);
}

EXPORT void setUhfOsc(unsigned int freq)
{
    pSunSDR->setUhfOsc(freq);
}

EXPORT void setCalGen(bool Mode)
{
    pSunSDR->setCalGen(Mode);
}

EXPORT void setXvAnt(int Mode)
{
    pSunSDR->setXvAnt(Mode);
}

EXPORT void showPluginGui()
{
    // no gui in this plugin
}
