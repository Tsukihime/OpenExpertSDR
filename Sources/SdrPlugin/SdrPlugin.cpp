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
 * Copyright (C) 2012 Valery Mikhaylovsky
 * The authors can be reached by email at maksimus1210@gmail.com
 */

#include "SdrPlugin.h"


SdrPlugin::SdrPlugin(Options *pOpt, StreamCallback *pCallBack, StreamCallback *pCallBack4, void *UsrData, QWidget *parent) : QWidget(parent)
{
	CalibrateFreq = 1.0;
	FreqDDS = 1800000;
	isStarted = false;
	pUi = &pOpt->ui;
	AudioCallBack = pCallBack;
	AudioCallBack4 = pCallBack4;
	SdrType = SUNSDR;
	pAudio = new pa19(UsrData);
	pSunSDR = new sunCtrl;
	pAudio->open();
	pUi->cbPaDriver->addItems(pAudio->driverName());
	pUi->cbPaIn->addItems(pAudio->inDevName(0));
	pUi->cbPaOut->addItems(pAudio->outDevName(0));
	connect(pOpt, SIGNAL(driverChanged(int)), this, SLOT(onPaDriverChanged(int)));
	connect(pUi->tbSDR, SIGNAL(currentChanged(int)), this, SLOT(onSdrTypeChanged(int)));
	connect(pSunSDR, SIGNAL(PttChanged(bool)),  this, SIGNAL(PttChanged(bool)));
	connect(pSunSDR, SIGNAL(DashChanged(bool)), this, SIGNAL(DashChanged(bool)));
	connect(pSunSDR, SIGNAL(DotChanged(bool)),  this, SIGNAL(DotChanged(bool)));
	connect(pSunSDR, SIGNAL(AdcChanged(int, int)),  this, SIGNAL(AdcChanged(int, int)));

	Sun2CalibrateFreq = 1.0;
}

SdrPlugin::~SdrPlugin()
{
	delete pSunSDR;
	delete pAudio;
}

void SdrPlugin::onPaDriverChanged(int Index)
{
	pUi->cbPaIn->clear();
	pUi->cbPaIn->addItems(pAudio->inDevName(Index));
	pUi->cbPaOut->clear();
	pUi->cbPaOut->addItems(pAudio->outDevName(Index));
}

void SdrPlugin::onSdrTypeChanged(int Index)
{
	SdrType = (SDR_DEVICE)Index;
}

void SdrPlugin::onS2ChangeBuffers(int NumBuffers)
{
	QString s;
	s.setNum(NumBuffers);
}

void SdrPlugin::SetSdrType(SDR_DEVICE Type)
{
	SdrType = Type;
}

void SdrPlugin::SetPreamp(int Preamp)
{
	pSunSDR->setPreamp(Preamp);
}

void SdrPlugin::SetExtCtrl(DWORD ExtData)
{
	pSunSDR->setExtCtrl((BYTE)ExtData);
}

void SdrPlugin::SetDdsFreq(float Freq)
{
	FreqDDS = Freq*CalibrateFreq;
	pSunSDR->setDdsFreq(FreqDDS);
}

float SdrPlugin::GetDdsFreq()
{
	return FreqDDS/CalibrateFreq;
}

void SdrPlugin::SetTrxMode(bool Mode)
{
	pSunSDR->setTrxMode(Mode);
}

void SdrPlugin::Close()
{
	pAudio->close();
}

int SdrPlugin::Start()
{
	if(isStarted)
		return -4;

	OptPlug.cbPaDriverIndex = pUi->cbPaDriver->currentIndex();
	OptPlug.cbPaOutIndex = pUi->cbPaOut->currentIndex();
	OptPlug.cbPaInIndex = pUi->cbPaIn->currentIndex();
	OptPlug.cbPaChannelsIndex = pUi->cbPaChannels->currentIndex();
	OptPlug.cbPaSampleRate = pUi->cbPaSampleRate->currentText().toInt();
	OptPlug.cbPaBufferSizeIndex = pUi->cbPaBufferSize->currentIndex();
	OptPlug.sbPaLattency = pUi->sbPaLattency->value();
	OptPlug.sbDdsMul = pUi->cbDdsOsc->currentIndex()+1;
	pAudio->setParam(&OptPlug);
	if(OptPlug.cbPaChannelsIndex == 0)
	{
		if(pAudio->start(AudioCallBack)!=0)
			return -1;
	}
	else
	{
		if(pAudio->start(AudioCallBack4)!=0)
			return -1;
	}
	pSunSDR->open(0, OptPlug.sbDdsMul);
	if(pSunSDR->isOpen()==0)
		emit sunsdrConnectInfo("SunSDR is not connected.");
	else
		emit sunsdrConnectInfo("SunSDR is connected.");
	return 0;

	pUi->tbSDR->setEnabled(false);
	isStarted = true;
	return 0;
}

void SdrPlugin::Stop()
{
	pAudio->stop();
	pSunSDR->close();
	pUi->tbSDR->setEnabled(true);
	isStarted = false;
}

bool SdrPlugin::IsStart()
{
	return isStarted;
}

void SdrPlugin::SetDdsCalibrate(double val)
{
	CalibrateFreq = val;
}

void SdrPlugin::setCalGen(bool Mode)
{
	pSunSDR->setCalGen(Mode);
}

void SdrPlugin::setVhfOsc(quint32 freq)
{
	pSunSDR->setVhfOsc(freq);
}

void SdrPlugin::setUhfOsc(quint32 freq)
{
	pSunSDR->setUhfOsc(freq);
}

void SdrPlugin::setMute(bool status)
{
	pSunSDR->setMute(status);
}

void SdrPlugin::setXvAnt(int Mode)
{
	pSunSDR->setXvAnt(Mode);
}

