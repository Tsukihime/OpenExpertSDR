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

#include "PluginCtrl.h"

pluginCtrl::pluginCtrl(QString libpath)
{
    pluginLoaded = false;
    pPlugLib = new QLibrary(libpath);
    pPlugLib->load();
    if(!pPlugLib->isLoaded())
        return;

    _getInfo = (PluginFunc_getInfo)pPlugLib->resolve("getInfo");
    if(_getInfo == NULL)
    {
        pPlugLib->unload();
        return;
    }

    QByteArray bname(MAX_PATH, 0);
    _getInfo(bname.data());
    InfoStr = QString::fromLatin1(bname.data());

    _init = (PluginFunc_init)pPlugLib->resolve("init");
    _deinit = (PluginFunc_deinit)pPlugLib->resolve("deinit");
    _open = (PluginFunc_open)pPlugLib->resolve("open");
    _close = (PluginFunc_close)pPlugLib->resolve("close");
    _isOpen = (PluginFunc_isOpen)pPlugLib->resolve("isOpen");
    _setPreamp = (PluginFunc_setPreamp)pPlugLib->resolve("setPreamp");
    _setExtCtrl = (PluginFunc_setExtCtrl)pPlugLib->resolve("setExtCtrl");
    _setDdsFreq = (PluginFunc_setDdsFreq)pPlugLib->resolve("setDdsFreq");
    _setTrxMode = (PluginFunc_setTrxMode)pPlugLib->resolve("setTrxMode");
    _setMute = (PluginFunc_setMute)pPlugLib->resolve("setMute");
    _setVhfOsc = (PluginFunc_setVhfOsc)pPlugLib->resolve("setVhfOsc");
    _setUhfOsc = (PluginFunc_setUhfOsc)pPlugLib->resolve("setUhfOsc");
    _setCalGen = (PluginFunc_setCalGen)pPlugLib->resolve("setCalGen");
    _setXvAnt = (PluginFunc_setXvAnt)pPlugLib->resolve("setXvAnt");
    _showPluginGui = (PluginFunc_showPluginGui)pPlugLib->resolve("showPluginGui");

    pluginLoaded = true;
}

pluginCtrl::~pluginCtrl()
{
    if(!pPlugLib->isLoaded())
        pPlugLib->unload();

    pPlugLib->deleteLater();
}

void pluginCtrl::init()
{
    if(_init && pluginLoaded)
        _init(this, &SdrStateChanged);
}

void pluginCtrl::deinit()
{
    if(_deinit && pluginLoaded)
        _deinit();
}

void pluginCtrl::open(int SdrNum, int DdsMul)
{
    if(_open && pluginLoaded)
        _open(SdrNum,DdsMul);
}

void pluginCtrl::close()
{
    if(_close && pluginLoaded)
        _close();
}

bool pluginCtrl::isOpen()
{
    if(_isOpen && pluginLoaded)
        return _isOpen();
    return false;
}

void pluginCtrl::setPreamp(int Preamp)
{
    if(_setPreamp && pluginLoaded)
        _setPreamp(Preamp);
}

void pluginCtrl::setExtCtrl(DWORD ExtData)
{
    if(_setExtCtrl && pluginLoaded)
        _setExtCtrl(ExtData);
}

void pluginCtrl::setDdsFreq(int Freq)
{
    if(_setDdsFreq && pluginLoaded)
        _setDdsFreq(Freq);
}

void pluginCtrl::setTrxMode(bool Mode)
{
    if(_setTrxMode && pluginLoaded)
        _setTrxMode(Mode);
}

void pluginCtrl::setMute(bool Status)
{
    if(_setMute && pluginLoaded)
        _setMute(Status);
}

void pluginCtrl::setVhfOsc(quint32 freq)
{
    if(_setVhfOsc && pluginLoaded)
        _setVhfOsc(freq);
}

void pluginCtrl::setUhfOsc(quint32 freq)
{
    if(_setUhfOsc && pluginLoaded)
        _setUhfOsc(freq);
}

void pluginCtrl::setCalGen(bool Mode)
{
    if(_setCalGen && pluginLoaded)
        _setCalGen(Mode);
}

void pluginCtrl::setXvAnt(int Mode)
{
    if(_setXvAnt && pluginLoaded)
        _setXvAnt(Mode);
}

bool pluginCtrl::getInfo(QString libpath, QString &PlugName)
{
    QLibrary plugin(libpath);
    plugin.load();
    if(!plugin.isLoaded())
        return false;

    PluginFunc_getInfo getInfoProc = (PluginFunc_getInfo)plugin.resolve("getInfo");
    if(getInfoProc == NULL)
    {
        plugin.unload();
        return false;
    }
    QByteArray bname(MAX_PATH, 0);
    getInfoProc(bname.data());
    plugin.unload();

    PlugName = QString::fromLatin1(bname.data());
    return true;
}

QString pluginCtrl::getInfo()
{
    return InfoStr;
}

bool pluginCtrl::isLoaded()
{
    return pluginLoaded;
}

void pluginCtrl::showPluginGui()
{
    if(_showPluginGui && pluginLoaded)
        _showPluginGui();
}

void pluginCtrl::SdrStateChanged(QObject *PlugCtrl, StateChgReason reason, bool arg1, int arg2, int arg3)
{
    if(PlugCtrl == NULL)
        return;

    switch (reason) {
    case crPtt:
        QMetaObject::invokeMethod(PlugCtrl, "PttChanged",  Qt::QueuedConnection,  Q_ARG(bool, arg1));
        break;
    case crDash:
        QMetaObject::invokeMethod(PlugCtrl, "DashChanged",  Qt::QueuedConnection,  Q_ARG(bool, arg1));
        break;
    case crDot:
        QMetaObject::invokeMethod(PlugCtrl, "DotChanged",  Qt::QueuedConnection,  Q_ARG(bool, arg1));
        break;
    case crAdc:
        QMetaObject::invokeMethod(PlugCtrl, "AdcChanged",  Qt::QueuedConnection,  Q_ARG(int, arg2), Q_ARG(int, arg3));
        break;
    default:
        break;
    }
}
