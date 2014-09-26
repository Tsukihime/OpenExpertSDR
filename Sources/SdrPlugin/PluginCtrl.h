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

enum StateChgReason {
    crPtt = 0,
    crDash = 1,
    crDot = 2,
    crAdc = 3
};

typedef __cdecl void (*CallbackFunc_SdrStateChanged)(QObject *PlugCtrl, StateChgReason reason, bool arg1, int arg2, int arg3);
typedef __cdecl void (*PluginFunc_getInfo)(char*);

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


class pluginCtrl : public QObject
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
    QString getInfo();
    bool isLoaded();

private:
    QString InfoStr;
    QLibrary *pPlugLib;
    bool pluginLoaded;

    PluginFunc_getInfo _getInfo;

    PluginFunc_init _init;
    PluginFunc_deinit _deinit;
    PluginFunc_open _open;
    PluginFunc_close _close;
    PluginFunc_isOpen _isOpen;
    PluginFunc_setPreamp _setPreamp;
    PluginFunc_setExtCtrl _setExtCtrl;
    PluginFunc_setDdsFreq _setDdsFreq;
    PluginFunc_setTrxMode _setTrxMode;
    PluginFunc_setMute _setMute;
    PluginFunc_setVhfOsc _setVhfOsc;
    PluginFunc_setUhfOsc _setUhfOsc;
    PluginFunc_setCalGen _setCalGen;
    PluginFunc_setXvAnt _setXvAnt;

    static __cdecl void SdrStateChanged(QObject *PlugCtrl, StateChgReason reason, bool arg1, int arg2, int arg3);

signals:
    void PttChanged(bool);
    void DashChanged(bool);
    void DotChanged(bool);
    void AdcChanged(int , int);
};

#endif // PLUGINCTRL_H
