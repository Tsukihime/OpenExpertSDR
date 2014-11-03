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
#include "DttSP.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>

static double y2=0.0;

DttSP::DttSP(void *UsrData, QObject *parent): QThread(parent)
{
	UserData = UsrData;

	isLoad = false;
	DspPos = 0;
	DspCurrentPos = 0;

	use_last = false;
	tri_direction = 1;
	tri_val = 0.0;
	saw_val = 0.0;
	saw_direction = 1;

	DspSamleRate = 48000;

	pLib = new QLibrary("DttSP");
	pLib->load();
	if(!pLib->isLoaded())
		return;

	isLoad = true;
	DspSetup_SDR = (DllFunc0)pLib->resolve("Setup_SDR");
	DspDestroy_SDR = (DllFunc0)pLib->resolve("Destroy_SDR");
	DspSetMode = (DllFunc1)pLib->resolve("SetMode");
	DspAudioReset = (DllFunc0)pLib->resolve("AudioReset");
	DspSetDCBlock = (DllFunc2)pLib->resolve("SetDCBlock");
	DspSetFilter = (DllFunc3)pLib->resolve("SetFilter");
	DspRelease_Update = (DllFunc0)pLib->resolve("Release_Update");
	DspSetRXOutputGain = (DllFunc5)pLib->resolve("SetRXOutputGain");
	DspSetOsc = (DllFunc5)pLib->resolve("SetOsc");
	DspSetTXOsc = (DllFunc5)pLib->resolve("SetTXOsc");
	DspSetSampleRate = (DllFunc5)pLib->resolve("SetSampleRate");
	DspSetNR = (DllFunc2)pLib->resolve("SetNR");
	DspSetBlkNR = (DllFunc2)pLib->resolve("SetBlkNR");
	DspSetNRvals = (DllFunc6)pLib->resolve("SetNRvals");
	DspSetTXCompandSt = (DllFunc7)pLib->resolve("SetTXCompandSt");
	DspSetTXCompand = (DllFunc4)pLib->resolve("SetTXCompand");
	DspSetTXSquelchSt = (DllFunc2)pLib->resolve("SetTXSquelchSt");
	DspSetTXSquelchVal = (DllFunc8)pLib->resolve("SetTXSquelchVal");
	DspSetANF = (DllFunc2)pLib->resolve("SetANF");
	DspSetBlkANF = (DllFunc2)pLib->resolve("SetBlkANF");
	DspSetANFvals = (DllFunc6)pLib->resolve("SetANFvals");
	DspSetNB = (DllFunc2)pLib->resolve("SetNB");
	DspSetNBvals = (DllFunc4)pLib->resolve("SetNBvals");
	DspSetSDROM = (DllFunc2)pLib->resolve("SetSDROM");
	DspSetSDROMvals = (DllFunc4)pLib->resolve("SetSDROMvals");
	DspSetBIN = (DllFunc2)pLib->resolve("SetBIN");
	DspSetRXAGC = (DllFunc19)pLib->resolve("SetRXAGC");
	DspSetRXAGCAttack = (DllFunc7)pLib->resolve("SetRXAGCAttack");
	DspSetRXAGCDecay = (DllFunc7)pLib->resolve("SetRXAGCDecay");
	DspSetRXAGCHang = (DllFunc7)pLib->resolve("SetRXAGCHang");
	DspSetRXAGCSlope = (DllFunc7)pLib->resolve("SetRXAGCSlope");
	DspSetRXAGCHangThreshold = (DllFunc7)pLib->resolve("SetRXAGCHangThreshold");
	DspSetTXALCAttack = (DllFunc7)pLib->resolve("SetTXALCAttack");
	DspSetTXCarrierLevel = (DllFunc5)pLib->resolve("SetTXCarrierLevel");
	DspSetTXALCDecay = (DllFunc7)pLib->resolve("SetTXALCDecay");
	DspSetTXALCBot = (DllFunc5)pLib->resolve("SetTXALCBot");
	DspSetTXALCHang = (DllFunc7)pLib->resolve("SetTXALCHang");
	DspSetTXLevelerSt = (DllFunc2)pLib->resolve("SetTXLevelerSt");
	DspSetTXLevelerAttack = (DllFunc7)pLib->resolve("SetTXLevelerAttack");
	DspSetTXLevelerDecay = (DllFunc7)pLib->resolve("SetTXLevelerDecay");
	DspSetTXLevelerTop = (DllFunc4)pLib->resolve("SetTXLevelerTop");
	DspSetTXLevelerHang = (DllFunc7)pLib->resolve("SetTXLevelerHang");
	DspSetFixedAGC = (DllFunc4)pLib->resolve("SetFixedAGC");
	DspSetRXAGCTop = (DllFunc4)pLib->resolve("SetRXAGCTop");
	DspSetCorrectIQ = (DllFunc9)pLib->resolve("SetCorrectIQ");
	DspSetCorrectIQPhase = (DllFunc4)pLib->resolve("SetCorrectIQPhase");
	DspSetCorrectIQGain = (DllFunc4)pLib->resolve("SetCorrectIQGain");
	DspSetCorrectTXIQ = (DllFunc9)pLib->resolve("SetCorrectTXIQ");
	DspSetCorrectTXIQPhase = (DllFunc4)pLib->resolve("SetCorrectTXIQPhase");
	DspSetCorrectTXIQGain = (DllFunc4)pLib->resolve("SetCorrectTXIQGain");
	DspSetPWSmode = (DllFunc7)pLib->resolve("SetPWSmode");
	DspSetWindow = (DllFunc20)pLib->resolve("SetWindow");
	DspSetSpectrumPolyphase = (DllFunc2)pLib->resolve("SetSpectrumPolyphase");
	DspSetTXEQ = (DllFunc10)pLib->resolve("SetTXEQ");
	DspSetGrphTXEQ = (DllFunc10)pLib->resolve("SetGrphTXEQ");
	DspSetGrphTXEQcmd = (DllFunc2)pLib->resolve("SetGrphTXEQcmd");
	DspSetNotch160 = (DllFunc2)pLib->resolve("SetNotch160");
	DspSetGrphRXEQ = (DllFunc10)pLib->resolve("SetGrphRXEQ");
	DspSetGrphRXEQcmd = (DllFunc2)pLib->resolve("SetGrphRXEQcmd");
	DspSetTXAGCFF = (DllFunc2)pLib->resolve("SetTXAGCFF");
	DspSetTXAGCFFCompression = (DllFunc4)pLib->resolve("SetTXAGCFFCompression");
	DspSetSquelchVal = (DllFunc8)pLib->resolve("SetSquelchVal");
	DspSetSquelchState = (DllFunc2)pLib->resolve("SetSquelchState");
	DspSetTRX = (DllFunc11)pLib->resolve("SetTRX");
	DspSetDSPBuflen = (DllFunc7)pLib->resolve("SetDSPBuflen");
	DspSetAudioSize = (DllFunc12)pLib->resolve("SetAudioSize");
	DspSetTXAGCLimit = (DllFunc4)pLib->resolve("SetTXAGCLimit");
	DspProcess_Spectrum = (DllFunc13)pLib->resolve("Process_Spectrum");
	DspProcess_Panadapter = (DllFunc13)pLib->resolve("Process_Panadapter");
	DspProcess_Phase = (DllFunc14)pLib->resolve("Process_Phase");
	DspProcess_Scope = (DllFunc14)pLib->resolve("Process_Scope");
	DspSetRingBufferOffset = (DllFunc10)pLib->resolve("SetRingBufferOffset");
	DspCalculate_Meters = (DllFunc15)pLib->resolve("Calculate_Meters");
	DspNewResampler = (DllFunc16)pLib->resolve("NewResampler");
	DspDoResampler = (DllFunc17)pLib->resolve("DoResampler");
	DspDelPolyPhaseFIR = (DllFunc18)pLib->resolve("DelPolyPhaseFIR");
	DspNewResamplerF = (DllFunc16)pLib->resolve("NewResamplerF");
	DspDoResamplerF = (DllFunc21)pLib->resolve("DoResamplerF");
	DspDelPolyPhaseFIRF = (DllFunc18)pLib->resolve("DelPolyPhaseFIRF");
	DspSetRXListen = (DllFunc7)pLib->resolve("SetRXListen");
	DspSetRXOn = (DllFunc7)pLib->resolve("SetRXOn");
	DspSetRXOff = (DllFunc7)pLib->resolve("SetRXOff");
	DspSetRXPan = (DllFunc8)pLib->resolve("SetRXPan");
	DspProcessSamplesThread = (DllFunc0)pLib->resolve("process_samples_thread");
	DspAudioCallback = (DllFunc22)pLib->resolve("Audio_Callback");
	DspSetTXFilter = (DllFunc29)pLib->resolve("SetTXFilter");

	TrxMode = RX;
	DspSamleRate = 48000.0;
	GlitchBufTxIn = 1;
	GlitchBufTxOut = 1;
	GlitchBufRxIn = 1;
	GlitchBufRxOut = 1;
	RitEnable = false;
	RitValue = 0;

	isRx2On = false;
	Rx1Bal = Rx2Bal = 50;
}


void DttSP::Open()
{
	DspSetup_SDR();
	DspRelease_Update();
	start(QThread::HighestPriority);
}

void DttSP::Close()
{
	DspDestroy_SDR();
}

void DttSP::AudioReset()
{
	DspAudioReset();
}

void DttSP::ReleaseUpdate()
{
	DspRelease_Update();
}

int DttSP::SetMode(SDRMODE Mode)
{
	int i;
	SdrMode = Mode;

	DspSetRXListen(RX_CHANNEL0);
	DspSetMode(Mode);
	DspSetRXListen(RX_CHANNEL1);
	DspSetMode(Mode);
	DspSetRXListen(RX_CHANNEL2);
	i = DspSetMode(Mode);
	DspSetRXListen(RX_LISTEN_CHANNEL);
	return(i);
}

void DttSP::SetDcBlock(bool Status)
{
	DspSetDCBlock(Status);
}

int DttSP::SetFilter(double low_frequency, double high_frequency, int taps, TRXMODE trx)
{
	int i;

	DspSetRXListen(RX_CHANNEL0);
	DspSetFilter(low_frequency,high_frequency,taps,trx);
	DspSetRXListen(RX_CHANNEL1);
	DspSetFilter(low_frequency,high_frequency,taps,trx);
	DspSetRXListen(RX_CHANNEL2);
	i = DspSetFilter(low_frequency,high_frequency,taps,trx);
	DspSetRXListen(RX_LISTEN_CHANNEL);

	return(DspSetFilter(low_frequency,high_frequency,taps,trx));
}

int DttSP::SetRxOutputGain(double Gain)
{
	int i;
	DspSetRXListen(RX_CHANNEL0);
	DspSetRXOutputGain(0);
	DspSetRXListen(RX_CHANNEL1);
	i = DspSetRXOutputGain(Gain);
	DspSetRXListen(RX_LISTEN_CHANNEL);
	return(i);
}

int DttSP::SetRxOsc(double Freq)
{
	if(TrxMode == RX)
	{
		if(RitEnable)
			Freq += RitValue;
	}

	int i;
	DspSetRXListen(RX_CHANNEL0);
	if(RX_LISTEN_CHANNEL == 0)
		DspSetOsc(Freq);
	else
		DspSetOsc(0.0);
	DspSetRXListen(RX_CHANNEL1);
	i = DspSetOsc(Freq);
	DspSetRXListen(RX_LISTEN_CHANNEL);
	return(i);
}

int DttSP::SetTxOsc(double Freq)
{
	return(DspSetTXOsc(Freq));
}

int DttSP::SetSampleRate(double Rate)
{
	DspSamleRate = Rate;
	return(DspSetSampleRate(Rate));
}

void DttSP::SetNr(bool Status)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetNR(Status);
	DspSetRXListen(RX_CHANNEL1);
	DspSetNR(Status);
	DspSetRXListen(RX_CHANNEL2);
	DspSetNR(Status);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetBlkNr(bool Status)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetBlkNR(Status);
	DspSetRXListen(RX_CHANNEL1);
	DspSetBlkNR(Status);
	DspSetRXListen(RX_CHANNEL2);
	DspSetBlkNR(Status);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetNrVals(int taps, int delay, double gain, double leak)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetNRvals(taps, delay, gain, leak);
	DspSetRXListen(RX_CHANNEL1);
	DspSetNRvals(taps, delay, gain, leak);
	DspSetRXListen(RX_CHANNEL2);
	DspSetNRvals(taps, delay, gain, leak);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetTxCompandSt(int Param)
{
	DspSetTXCompandSt(Param);
}

void DttSP::SetTxCompand(double Param)
{
	DspSetTXCompand(Param);
}

void DttSP::SetTxSquelchSt(bool Status)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetTXSquelchSt(Status);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetTxSquelchVal(float Val)
{
	DspSetTXSquelchVal(Val);
}

void DttSP::SetAnf(bool Status)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetANF(Status);
	DspSetRXListen(RX_CHANNEL1);
	DspSetANF(Status);
	DspSetRXListen(RX_CHANNEL2);
	DspSetANF(Status);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetBlkAnf(bool Status)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetBlkANF(Status);
	DspSetRXListen(RX_CHANNEL1);
	DspSetBlkANF(Status);
	DspSetRXListen(RX_CHANNEL2);
	DspSetBlkANF(Status);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetANFvals(int taps, int delay, double gain, double leak)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetANFvals(taps,delay,gain,leak);
	DspSetRXListen(RX_CHANNEL1);
	DspSetANFvals(taps,delay,gain,leak);
	DspSetRXListen(RX_CHANNEL2);
	DspSetANFvals(taps,delay,gain,leak);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetNB(bool Status)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetNB(Status);
	DspSetRXListen(RX_CHANNEL1);
	DspSetNB(Status);
	DspSetRXListen(RX_CHANNEL2);
	DspSetNB(Status);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetNBvals(double Val)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetNBvals(Val);
	DspSetRXListen(RX_CHANNEL1);
	DspSetNBvals(Val);
	DspSetRXListen(RX_CHANNEL2);
	DspSetNBvals(Val);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetSdrOm(bool Status)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetSDROM(Status);
	DspSetRXListen(RX_CHANNEL1);
	DspSetSDROM(Status);
	DspSetRXListen(RX_CHANNEL2);
	DspSetSDROM(Status);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetSdrOmvals(double Val)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetSDROMvals(Val);
	DspSetRXListen(RX_CHANNEL1);
	DspSetSDROMvals(Val);
	DspSetRXListen(RX_CHANNEL2);
	DspSetSDROMvals(Val);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetBin(bool Status)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetBIN(Status);
	DspSetRXListen(RX_CHANNEL1);
	DspSetBIN(Status);
	DspSetRXListen(RX_CHANNEL2);
	DspSetBIN(Status);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetRxAgc(AGCMODE Mode)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetRXAGC(Mode);
	DspSetRXListen(RX_CHANNEL1);
	DspSetRXAGC(Mode);
	DspSetRXListen(RX_CHANNEL2);
	DspSetRXAGC(Mode);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetRxAgcAttack(int Val)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetRXAGCAttack(Val);
	DspSetRXListen(RX_CHANNEL1);
	DspSetRXAGCAttack(Val);
	DspSetRXListen(RX_CHANNEL2);
	DspSetRXAGCAttack(Val);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetRxAgcDecay(int Val)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetRXAGCDecay(Val);
	DspSetRXListen(RX_CHANNEL1);
	DspSetRXAGCDecay(Val);
	DspSetRXListen(RX_CHANNEL2);
	DspSetRXAGCDecay(Val);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetRxAgcHang(int Val)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetRXAGCHang(Val);
	DspSetRXListen(RX_CHANNEL1);
	DspSetRXAGCHang(Val);
	DspSetRXListen(RX_CHANNEL2);
	DspSetRXAGCHang(Val);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetRxAgcSlope(int Val)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetRXAGCSlope(Val);
	DspSetRXListen(RX_CHANNEL1);
	DspSetRXAGCSlope(Val);
	DspSetRXListen(RX_CHANNEL2);
	DspSetRXAGCSlope(Val);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetRxAgcHangThreshold(int Val)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetRXAGCHangThreshold(Val);
	DspSetRXListen(RX_CHANNEL1);
	DspSetRXAGCHangThreshold(Val);
	DspSetRXListen(RX_CHANNEL2);
	DspSetRXAGCHangThreshold(Val);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetTxAlcAttack(int Val)
{
	DspSetTXALCAttack(Val);
}

void DttSP::SetTxAlcDecay(bool Status)
{
	DspSetTXALCDecay(Status);
}

int DttSP::SetTxAlcBot(double Val)
{
	return(DspSetTXALCBot(Val));
}

void DttSP::SetTxAlcHang(int Val)
{
	DspSetTXALCHang(Val);
}

void DttSP::SetTxLevelerSt(bool Status)
{
	DspSetTXLevelerSt(Status);
}

void DttSP::SetTxLevelerAttack(int Val)
{
	DspSetTXLevelerAttack(Val);
}

void DttSP::SetTXLevelerDecay(int Val)
{
	DspSetTXLevelerDecay(Val);
}

void DttSP::SetTxLevelerTop(double Val)
{
	DspSetTXLevelerTop(Val);
}

void DttSP::SetTxLevelerHang(int Val)
{
	DspSetTXLevelerHang(Val);
}

void DttSP::SetTxFilter(uint Thread, double Low, double High)
{
	DspSetTXFilter(Thread, Low, High);
}

void DttSP::SetFixedAgc(double Val)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetFixedAGC(Val);
	DspSetRXListen(RX_CHANNEL1);
	DspSetFixedAGC(Val);
	DspSetRXListen(RX_CHANNEL2);
	DspSetFixedAGC(Val);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetRxAgcTop(double Val)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetRXAGCTop(Val);
	DspSetRXListen(RX_CHANNEL1);
	DspSetRXAGCTop(Val);
	DspSetRXListen(RX_CHANNEL2);
	DspSetRXAGCTop(Val);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetCorrectTXIQ(double Gain, double Phase)
{
	DspSetCorrectTXIQ(Gain/10.0, Phase/10.0);
}

void DttSP::SetCorrectTXIQPhase(double Phase)
{
	DspSetCorrectTXIQPhase(Phase/10.0);
}

void DttSP::SetCorrectTXIQGain(double Gain)
{
	DspSetCorrectTXIQGain(Gain/10.0);
}

void DttSP::SetPwsMode(int Val)
{
	DspSetPWSmode(Val);
}

void DttSP::SetWindow(Windowtype Wtype)
{
	DspSetWindow(Wtype);
}

void DttSP::SetSpectrumPolyphase(bool Status)
{
	DspSetSpectrumPolyphase(Status);
}

void DttSP::SetTxEQ(int *p)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetGrphTXEQ(p);
	DspSetRXListen(RX_CHANNEL1);
	DspSetGrphTXEQ(p);
	DspSetRXListen(RX_CHANNEL2);
	DspSetGrphTXEQ(p);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetGrphTXEQcmd(bool Status)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetGrphTXEQcmd(Status);
	DspSetRXListen(RX_CHANNEL1);
	DspSetGrphTXEQcmd(Status);
	DspSetRXListen(RX_CHANNEL2);
	DspSetGrphTXEQcmd(Status);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetNotch160(bool Status)
{
	DspSetNotch160(Status);
}

void DttSP::SetGrphRxEQ(int *p)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetGrphRXEQ(p);
	DspSetRXListen(RX_CHANNEL1);
	DspSetGrphRXEQ(p);
	DspSetRXListen(RX_CHANNEL2);
	DspSetGrphRXEQ(p);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetGrphRxEQcmd(bool Status)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetGrphRXEQcmd(Status);
	DspSetRXListen(RX_CHANNEL1);
	DspSetGrphRXEQcmd(Status);
	DspSetRXListen(RX_CHANNEL2);
	DspSetGrphRXEQcmd(Status);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetTxAgcFF(bool Status)
{
	DspSetTXAGCFF(Status);
}

void DttSP::SetTxAgcFfCompression(double Val)
{
	DspSetTXAGCFFCompression(Val);
}

void DttSP::SetTxCarrierLevel(float Val)
{
	DspSetTXCarrierLevel(Val);
}

void DttSP::SetSquelchVal(double Val)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetSquelchVal(Val);
	DspSetRXListen(RX_CHANNEL1);
	DspSetSquelchVal(Val);
	DspSetRXListen(RX_CHANNEL2);
	DspSetSquelchVal(Val);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetSquelchState(bool Status)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetSquelchState(Status);
	DspSetRXListen(RX_CHANNEL1);
	DspSetSquelchState(Status);
	DspSetRXListen(RX_CHANNEL2);
	DspSetSquelchState(Status);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetTrx(TRXMODE Mode)
{
	DspSetTRX(Mode);
	TrxMode = Mode;
}

void DttSP::SetDSPBuflen(int BufLen)
{
	DspSetRXListen(RX_CHANNEL0);
	DspSetDSPBuflen(BufLen);
	DspSetRXListen(RX_CHANNEL1);
	DspSetDSPBuflen(BufLen);
	DspSetRXListen(RX_CHANNEL2);
	DspSetDSPBuflen(BufLen);
	DspSetRXListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetAudioSize(unsigned int Size)
{
	DspSetAudioSize(Size);
}

void DttSP::SetTxAgcLimit(double Val)
{
	DspSetTXAGCLimit(Val);
}

void DttSP::ProcessSpectrum(float *pBuf)
{
	DspProcess_Spectrum(pBuf);
}

void DttSP::ProcessPanadapter(float *pBuf)
{
	DspProcess_Panadapter(pBuf);
}

void DttSP::ProcessPhase(float *pBuf, int Param)
{
	DspProcess_Phase(pBuf, Param);
}

void DttSP::ProcessScope(float *pBuf, int Param)
{
	DspProcess_Scope(pBuf, Param);
}

void DttSP::SetRingBufferOffset(int *Param)
{
	DspSetRingBufferOffset(Param);
}

float DttSP::CalculateMeters(METERTYPE Meter)
{
	return(DspCalculate_Meters(Meter));
}

void* DttSP::NewResampler(int samplerate_in, int samplerate_out)
{
	return(DspNewResampler(samplerate_in,samplerate_out));
}

void DttSP::DoResampler(COMPLEX *p1, COMPLEX *p2, int a, int *b, ResSt c)
{
	DspDoResampler(p1, p2, a, b, c);
}

void DttSP::DelPolyPhaseFIR(ResSt p)
{
	DspDelPolyPhaseFIR(p);
}

void* DttSP::NewResamplerF(int samplerate_in, int samplerate_out)
{
	return(DspNewResamplerF(samplerate_in, samplerate_out));
}

void DttSP::DoResamplerF(float *input, float *output, int numsamps, int* outsamps, ResStF ptr)
{
	if((int)ptr == 0)
		return;
	DspDoResamplerF(input, output, numsamps, outsamps, ptr);
}

void DttSP::DelPolyPhaseFIRF(ResSt p)
{
	DspDelPolyPhaseFIRF(p);
}

void DttSP::SetRxListen(int rx_index)
{
	DspSetRXListen(rx_index);
}

void DttSP::SetRXOn(int rx_index)
{
	DspSetRXOn(rx_index);
}

void DttSP::SetRxOff(int rx_index)
{
	DspSetRXOff(rx_index);
}

void DttSP::SetRxPan(float Val)
{
	DspSetRXPan(Val);
}

void DttSP::ProcessSamplesThread()
{
	DspProcessSamplesThread();
}

void DttSP::AudioCallback(float* pInL, float* pInR, float* pOutL, float* pOutR, unsigned int FrameCount)
{
	DspAudioCallback(pInL, pInR, pOutL, pOutR, FrameCount);
}

void DttSP::run()
{
	ProcessSamplesThread();
}

double DttSP::SineWave(float* Buf, int Samples, double Phase, double Freq)
{
	double phase_step = Freq/DspSamleRate*2*DSP_PI;
	double cosval = cos(Phase);
	double sinval = sin(Phase);
	double cosdelta = cos(phase_step);
	double sindelta = sin(phase_step);
	double tmpval;

	for(int i = 0; i < Samples; i++ )
	{
		tmpval = cosval*cosdelta - sinval*sindelta;
		sinval = cosval*sindelta + sinval*cosdelta;
		cosval = tmpval;

		Buf[i] = (float)(sinval);
		Phase += phase_step;
	}
	return Phase;
}

double DttSP::CosineWave(float* Buf, int Samples, double Phase, double Freq)
{
	double phase_step = Freq/DspSamleRate*2*DSP_PI;
	double cosval = cos(Phase);
	double sinval = sin(Phase);
	double cosdelta = cos(phase_step);
	double sindelta = sin(phase_step);
	double tmpval;

	for(int i = 0; i < Samples; i++ )
	{
		tmpval = cosval*cosdelta - sinval*sindelta;
		sinval = cosval*sindelta + sinval*cosdelta;
		cosval = tmpval;

		Buf[i] = (float)(cosval);

		Phase += phase_step;
	}

	return Phase;
}

void DttSP::ScaleBuffer(float *InBuf, float *OutBuf, int Samples, float Scale)
{
	for(int i = 0; i < Samples; i++)
		OutBuf[i] = InBuf[i] * Scale;
}

void DttSP::SineWave2Tone(float* buf, int samples, double phase1, double phase2, double freq1, double freq2, double *updated_phase1, double *updated_phase2)
{
	double phase_step1 = freq1/DspSamleRate*2*DSP_PI;
	double cosval1 = cos(phase1);
	double sinval1 = sin(phase1);
	double cosdelta1 = cos(phase_step1);
	double sindelta1 = sin(phase_step1);

	double phase_step2 = freq2/DspSamleRate*2*DSP_PI;
	double cosval2 = cos(phase2);
	double sinval2 = sin(phase2);
	double cosdelta2 = cos(phase_step2);
	double sindelta2 = sin(phase_step2);
	double tmpval;

	for(int i = 0; i < samples; i++ )
	{
		tmpval = cosval1*cosdelta1 - sinval1*sindelta1;
		sinval1 = cosval1*sindelta1 + sinval1*cosdelta1;
		cosval1 = tmpval;

		tmpval = cosval2*cosdelta2 - sinval2*sindelta2;
		sinval2 = cosval2*sindelta2 + sinval2*cosdelta2;
		cosval2 = tmpval;

		buf[i] = (float)(sinval1*0.5 + sinval2*0.5);

		phase1 += phase_step1;
		phase2 += phase_step2;
	}

	(*updated_phase1) = phase1;
	(*updated_phase2) = phase2;
}

void DttSP::CosineWave2Tone(float* buf, int samples, double phase1, double phase2, double freq1, double freq2, double *updated_phase1, double *updated_phase2)
{
	double phase_step1 = freq1/DspSamleRate*2*DSP_PI;
	double cosval1 = cos(phase1);
	double sinval1 = sin(phase1);
	double cosdelta1 = cos(phase_step1);
	double sindelta1 = sin(phase_step1);

	double phase_step2 = freq2/DspSamleRate*2*DSP_PI;
	double cosval2 = cos(phase2);
	double sinval2 = sin(phase2);
	double cosdelta2 = cos(phase_step2);
	double sindelta2 = sin(phase_step2);
	double tmpval;

	for(int i=0; i<samples; i++ )
	{
		tmpval = cosval1*cosdelta1 - sinval1*sindelta1;
		sinval1 = cosval1*sindelta1 + sinval1*cosdelta1;
		cosval1 = tmpval;

		tmpval = cosval2*cosdelta2 - sinval2*sindelta2;
		sinval2 = cosval2*sindelta2 + sinval2*cosdelta2;
		cosval2 = tmpval;

		buf[i] = (float)(cosval1*0.5 + cosval2*0.5);

		phase1 += phase_step1;
		phase2 += phase_step2;
	}

	(*updated_phase1) = phase1;
	(*updated_phase2) = phase2;
}

double DttSP::Boxmuller(double m,double s)
{
	double x1,x2,w,y1;
	if (use_last)
	{
		y1 = y2;
		use_last = false;
	}
	else
	{
		do
		{
			x1 = (2.0 * ((rand()%1000)/1000.0) - 1.0);
			x2 = (2.0 * ((rand()%1000)/1000.0) - 1.0);
			w = x1 * x1 + x2 * x2;
		} while ( w >= 1.0);

		w = qSqrt((-2.0 * qLn(w))/w);
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = true;
	}

	return( m + y1 * s );
}

void DttSP::Noise(float* buf, int samples)
{
	for(int i=0; i<samples; i++)
	{
		buf[i] = (float)Boxmuller(0.0,0.2);
	}
}

void DttSP::Triangle(float* buf, int samples, double freq)
{
	double step = freq/DspSamleRate*2*tri_direction;
	for(int i=0; i<samples; i++)
	{
		buf[i] = (float)tri_val;
		tri_val += step;
		if(tri_val >= 1.0 || tri_val <= -1.0)
		{
			step = -step;
			tri_val += 2*step;
			if(step < 0)
				tri_direction = -1;
			else
				tri_direction = 1;
		}
	}
}

void DttSP::Sawtooth(float* buf, int samples, double freq)
{
	double step = freq/DspSamleRate*saw_direction;
	for(int i=0; i<samples; i++)
	{
		buf[i] = (float)saw_val;
		saw_val += step;

		if(saw_val >= 1.0)
			saw_val -= 2.0;
		if(saw_val <= -1.0)
			saw_val += 2.0;
	}
}

void DttSP::ClearBuffer(float *buf, int samples)
{
	memset(buf, 0, samples*sizeof(float));
}

double DttSP::SamleRate()
{
	return DspSamleRate;
}

void DttSP::OnGlitchCompChanged(QByteArray p)
{
	GlitchBufTxIn = p.data()[0];
	GlitchBufTxOut =p.data()[1];
	GlitchBufRxIn = p.data()[2];
	GlitchBufRxOut = p.data()[3];
}

void DttSP::SetRx2On(bool on)
{
	SetRxListen(RX_CHANNEL2);
	isRx2On = on;
	if(on)
	{
		SetRXOn(RX_CHANNEL2);
		SetRxListen(RX_CHANNEL1);
		DspSetRXPan(Rx1Bal/100.0);
		SetRxListen(RX_LISTEN_CHANNEL);
	}
	else
	{
		SetRxOff(RX_CHANNEL2);
		SetRxListen(RX_CHANNEL1);
		DspSetRXPan(0.5);
		SetRxListen(RX_LISTEN_CHANNEL);
	}
	SetRxListen(RX_LISTEN_CHANNEL);

}

void DttSP::SetRx1Volume(int Val)
{
	SetRxListen(RX_CHANNEL1);
	DspSetRXOutputGain(Val/100.0);
	SetRxListen(RX_LISTEN_CHANNEL);
}

void DttSP::SetRx2Volume(int Val)
{
	SetRxListen(RX_CHANNEL2);
	DspSetRXOutputGain(Val/100.0);
	SetRxListen(RX_LISTEN_CHANNEL);
}
void DttSP::SetRx1Balance(int Val)
{
	if(isRx2On)
	{
		SetRxListen(RX_CHANNEL1);
		DspSetRXPan(Val/100.0);
		SetRxListen(RX_LISTEN_CHANNEL);
	}
	Rx1Bal = Val;
}
void DttSP::SetRx2Balance(int Val)
{
	SetRxListen(RX_CHANNEL2);
	DspSetRXPan(Val/100.0);
	SetRxListen(RX_LISTEN_CHANNEL);
}
int DttSP::SetRx2Osc(double Freq)
{
	int i;
	DspSetRXListen(RX_CHANNEL0);
	if(RX_LISTEN_CHANNEL == 0)
		DspSetOsc(Freq);
	else
		DspSetOsc(0.0);
	DspSetRXListen(RX_CHANNEL2);
	i = DspSetOsc(Freq);
	DspSetRXListen(RX_LISTEN_CHANNEL);
	return (i);
}

void DttSP::SetRitEnable(bool state)
{
	RitEnable = state;
}

void DttSP::SetRitValue(int value)
{
	RitValue = -value;
}
