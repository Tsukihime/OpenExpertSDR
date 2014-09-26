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

#ifndef DTTSP_H_
#define DTTSP_H_

#include <QtGui>
#include <QFuture>
#include <process.h>
#include "qt_windows.h"
#include "Defines.h"
#include "ThreadFunc.h"

#define MAX_DSP_MEMORY_BUF	1000

#define RX_CHANNEL0 0
#define RX_CHANNEL1	1
#define RX_CHANNEL2	2
#define RX_LISTEN_CHANNEL	RX_CHANNEL1

#define DSP_PI 3.1415926536

typedef enum _swchstate {
  SWCH_FALL,
  SWCH_STDY,
  SWCH_RISE
} SWCHSTATE;

typedef enum _agcmode
{ agcOFF, agcLONG, agcSLOW, agcMED, agcFAST } AGCMODE;

typedef enum _windowtype
{
  RECTANGULAR_WINDOW,
  HANNING_WINDOW,
  WELCH_WINDOW,
  PARZEN_WINDOW,
  BARTLETT_WINDOW,
  HAMMING_WINDOW,
  BLACKMAN2_WINDOW,
  BLACKMAN3_WINDOW,
  BLACKMAN4_WINDOW,
  EXPONENTIAL_WINDOW,
  RIEMANN_WINDOW,
  BLACKMANHARRIS_WINDOW,
  NUTTALL_WINDOW,
} Windowtype;

typedef enum
{
  SIGNAL_STRENGTH,
  AVG_SIGNAL_STRENGTH,
  ADC_REAL,
  ADC_IMAG,
  AGC_GAIN,
  MIC,
  PWR,
  ALC,
  EQtap,
  LEVELER,
  COMP,
  CPDR,
  ALC_G,
  LVL_G
} METERTYPE;

typedef struct _COMPLEX
{
  float re, im;
} COMPLEX;

typedef enum
{
  FIR_Undef, FIR_Lowpass, FIR_Bandpass, FIR_Highpass, FIR_Hilbert,
  FIR_Bandstop
} FIR_response_type;

typedef struct _real_FIR
{
  float *coef;
  int size;
  FIR_response_type type;
  BOOLEAN cplx;
  struct
  {
    float lo, hi;
  } freq;
} RealFIRDesc, *RealFIR;

typedef struct resample_state
{
  COMPLEX *input, *output, *filterMemoryBuff;
  RealFIR filter;
  int filterMemoryBuffLength, inputArrayLength, numFiltTaps,
    indexfiltMemBuf, interpFactor, filterPhaseNum, deciFactor,
    numOutputSamples;
  int MASK;
} resampler, *ResSt;

typedef struct resampleF_state
{
  float *input, *output, *filterMemoryBuff;
  RealFIR filter;
  int filterMemoryBuffLength, inputArrayLength, numFiltTaps,
    indexfiltMemBuf, interpFactor, filterPhaseNum, deciFactor,
    numOutputSamples;
  int MASK;
} resamplerF, *ResStF;

typedef enum
{
	PIN_12	= 0x08,
	DASH	= 0x10,
	DOT		= 0x20,
	PA_DATA	= 0x40,
	PIN_11	= 0x80,
}STATUS_PIN;

typedef void (*DllFunc0)();
typedef int (*DllFunc1)(SDRMODE);
typedef void (*DllFunc2)(BOOLEAN);
typedef int (*DllFunc3)(double , double, int, TRXMODE);
typedef void (*DllFunc4)(double);
typedef int (*DllFunc5)(double);
typedef void (*DllFunc6)(int, int, double, double);
typedef void (*DllFunc7)(int);
typedef void (*DllFunc8)(float);
typedef void (*DllFunc9)(double, double);
typedef void (*DllFunc10)(int*);
typedef void (*DllFunc11)(TRXMODE);
typedef void (*DllFunc12)(unsigned int);
typedef void (*DllFunc13)(float*);
typedef void (*DllFunc14)(float*, int);
typedef float (*DllFunc15)(METERTYPE);
typedef void* (*DllFunc16)(int, int);
typedef void (*DllFunc17)(COMPLEX *, COMPLEX *, int, int *, ResSt);
typedef void (*DllFunc18)(ResSt);
typedef void (*DllFunc19)(AGCMODE);
typedef void (*DllFunc20)(Windowtype);
typedef void (*DllFunc21)(float *, float *, int, int*,ResStF);
typedef void (*DllFunc22)(float*, float*, float*, float*, unsigned int);
typedef void (*DllFunc23)(float, bool, bool, bool);
typedef void (*DllFunc24)(float, bool, float, float, float, float);
typedef void (*DllFunc25)(bool);
typedef bool (*DllFunc26)();
typedef void (*DllFunc27)(BYTE);
typedef void (*DllFunc28)(float*, float*, int);
typedef void (*DllFunc29)(uint, double, double);

class DttSP : public QThread
{
    Q_OBJECT

	private:
		DllFunc0  DspSetup_SDR;
		DllFunc0  DspDestroy_SDR;
		DllFunc1  DspSetMode;
		DllFunc0  DspAudioReset;
		DllFunc2  DspSetDCBlock;
		DllFunc3  DspSetFilter;
		DllFunc0  DspRelease_Update;
		DllFunc5  DspSetRXOutputGain;
		DllFunc5  DspSetOsc;
		DllFunc5  DspSetTXOsc;
		DllFunc5  DspSetSampleRate;
		DllFunc2  DspSetNR;
		DllFunc2  DspSetBlkNR;
		DllFunc6  DspSetNRvals;
		DllFunc7  DspSetTXCompandSt;
		DllFunc4  DspSetTXCompand;
		DllFunc2  DspSetTXSquelchSt;
		DllFunc8  DspSetTXSquelchVal;
		DllFunc2  DspSetANF;
		DllFunc2  DspSetBlkANF;
		DllFunc6  DspSetANFvals;
		DllFunc2  DspSetNB;
		DllFunc4  DspSetNBvals;
		DllFunc2  DspSetSDROM;
		DllFunc4  DspSetSDROMvals;
		DllFunc2  DspSetBIN;
		DllFunc19 DspSetRXAGC;
		DllFunc7  DspSetRXAGCAttack;
		DllFunc7  DspSetRXAGCDecay;
		DllFunc7  DspSetRXAGCHang;
		DllFunc7  DspSetRXAGCSlope;
		DllFunc7  DspSetRXAGCHangThreshold;
		DllFunc7  DspSetTXALCAttack;
		DllFunc5  DspSetTXCarrierLevel;
		DllFunc7  DspSetTXALCDecay;
		DllFunc5  DspSetTXALCBot;
		DllFunc7  DspSetTXALCHang;
		DllFunc2  DspSetTXLevelerSt;
		DllFunc7  DspSetTXLevelerAttack;
		DllFunc7  DspSetTXLevelerDecay;
		DllFunc4  DspSetTXLevelerTop;
		DllFunc7  DspSetTXLevelerHang;
		DllFunc4  DspSetFixedAGC;
		DllFunc4  DspSetRXAGCTop;
		DllFunc9  DspSetCorrectIQ;
		DllFunc4  DspSetCorrectIQPhase;
		DllFunc4  DspSetCorrectIQGain;
		DllFunc9  DspSetCorrectTXIQ;
		DllFunc4  DspSetCorrectTXIQPhase;
		DllFunc4  DspSetCorrectTXIQGain;
		DllFunc7  DspSetPWSmode;
		DllFunc20 DspSetWindow;
		DllFunc2  DspSetSpectrumPolyphase;
		DllFunc10 DspSetTXEQ;
		DllFunc10 DspSetGrphTXEQ;
		DllFunc2  DspSetGrphTXEQcmd;
		DllFunc2  DspSetNotch160;
		DllFunc10 DspSetGrphRXEQ;
		DllFunc2  DspSetGrphRXEQcmd;
		DllFunc2  DspSetTXAGCFF;
		DllFunc4  DspSetTXAGCFFCompression;
		DllFunc8  DspSetSquelchVal;
		DllFunc2  DspSetSquelchState;
		DllFunc11 DspSetTRX;
		DllFunc7  DspSetDSPBuflen;
		DllFunc12 DspSetAudioSize;
		DllFunc4  DspSetTXAGCLimit;
		DllFunc13 DspProcess_Spectrum;
		DllFunc13 DspProcess_Panadapter;
		DllFunc14 DspProcess_Phase;
		DllFunc14 DspProcess_Scope;
		DllFunc10 DspSetRingBufferOffset;
		DllFunc15 DspCalculate_Meters;
		DllFunc16 DspNewResampler;
		DllFunc17 DspDoResampler;
		DllFunc18 DspDelPolyPhaseFIR;
		DllFunc16 DspNewResamplerF;
		DllFunc21 DspDoResamplerF;
		DllFunc18 DspDelPolyPhaseFIRF;
		DllFunc7  DspSetRXListen;
		DllFunc7  DspSetRXOn;
		DllFunc7  DspSetRXOff;
		DllFunc8  DspSetRXPan;
		DllFunc0  DspProcessSamplesThread;
		DllFunc22 DspAudioCallback;
		DllFunc29 DspSetTXFilter;

		bool isRx2On;
		int Rx1Bal, Rx2Bal;

		int RitValue;
		bool RitEnable;

	public:

		float DspCircleBuf[MAX_DSP_MEMORY_BUF][2][4096];
		int 	DspPos;
		int 	DspCurrentPos;
		SDRMODE SdrMode;

		int		GlitchBufTxIn;
		int		GlitchBufTxOut;
		int		GlitchBufRxIn;
		int		GlitchBufRxOut;

		double DspSamleRate;
		double SamleRate();

		int DurationMsec;
		bool IsRunningThreadCWTone;

		DttSP(void *UsrData = 0, QObject *parent = 0);

		void *UserData;
		QLibrary *pLib;
		bool isLoad;
		TRXMODE TrxMode;

		BYTE DashDot;

		void Open();
		void Close();
		void AudioReset();
        void ReleaseUpdate();
		int SetMode(SDRMODE Mode);
		SDRMODE GetMode(){ return SdrMode;};
		void SetDcBlock(bool Status);
		int SetFilter(double , double , int, TRXMODE);
		int SetRxOutputGain(double Gain);
		int SetRxOsc(double Freq);
		int SetTxOsc(double Freq);
		int SetSampleRate(double Rate);
		void SetNr(bool Status);
		void SetBlkNr(bool Status);
		void SetNrVals(int a, int b, double c, double d);
		void SetTxCompandSt(int Param);
		void SetTxCompand(double Param);
		void SetTxSquelchSt(bool Status);
		void SetTxSquelchVal(float Val);
		void SetAnf(bool Status);
		void SetBlkAnf(bool Status);
		void SetANFvals(int a, int b, double c, double d);
		void SetNB(bool Status);
		void SetNBvals(double Val);
		void SetSdrOm(bool Status);
		void SetSdrOmvals(double Val);
		void SetBin(bool Status);
		void SetRxAgc(AGCMODE Mode);
		void SetRxAgcAttack(int Val);
		void SetRxAgcDecay(int Val);
		void SetRxAgcHang(int Val);
		void SetRxAgcSlope(int Val);
		void SetRxAgcHangThreshold(int Val);
		void SetTxAlcAttack(int Val);
		void SetTxAlcDecay(bool Status);
		int  SetTxAlcBot(double Val);
		void SetTxAlcHang(int Val);
		void SetTxLevelerSt(bool Status);
		void SetTxLevelerAttack(int Val);
		void SetTXLevelerDecay(int Val);
		void SetTxLevelerTop(double Val);
		void SetTxLevelerHang(int Val);
		void SetTxFilter(uint Thread, double Low, double High);
		void SetFixedAgc(double Val);
		void SetRxAgcTop(double Val);
		void SetCorrectTXIQ(double Gain, double Phase);
		void SetCorrectTXIQPhase(double Phase);
		void SetCorrectTXIQGain(double Gain);
		void SetPwsMode(int Val);
		void SetWindow(Windowtype Wtype);
		void SetSpectrumPolyphase(bool Status);
		void SetTxEQ(int *p);
		void SetGrphTXEQcmd(bool Status);
		void SetNotch160(bool Status);
		void SetGrphRxEQ(int *p);
		void SetGrphRxEQcmd(bool Status);
		void SetTxAgcFF(bool Status);
		void SetTxAgcFfCompression(double Val);
		void SetTxCarrierLevel(float Val);
		void SetSquelchVal(double Val);
		void SetSquelchState(bool Status);
		void SetTrx(TRXMODE Mode);
		void SetDSPBuflen(int BufLen);
		void SetAudioSize(unsigned int Size);
		void SetTxAgcLimit(double Val);
		void ProcessSpectrum(float *pBuf);
		void ProcessPanadapter(float *pBuf);
		void ProcessPhase(float *pBuf, int Param);
		void ProcessScope(float *pBuf, int Param);
		void SetRingBufferOffset(int *Param);
		float CalculateMeters(METERTYPE Meter);
		void* NewResampler(int a, int b);
		void DoResampler(COMPLEX *p1, COMPLEX *p2, int a, int *b, ResSt c);
		void DelPolyPhaseFIR(ResSt p);
		void* NewResamplerF(int a, int b);
		void DoResamplerF(float *p1, float *p2, int a, int* b,ResStF c);
		void DelPolyPhaseFIRF(ResSt p);
		void SetRxListen(int Val);
		void SetRXOn(int Val);
		void SetRxOff(int Val);
		void SetRxPan(float Val);
		void ProcessSamplesThread();
		void AudioCallback(float* pInL, float* pInR, float* pOutL, float* pOutR, unsigned int FrameCount);
		void run();

		double SineWave(float* Buf, int Samples, double Phase, double Freq);
		double CosineWave(float* Buf, int Samples, double Phase, double Freq);
		void ScaleBuffer(float *InBuf, float *OutBuf, int Samples, float Scale);

		void SineWave2Tone(float* buf, int samples, double phase1, double phase2, double freq1, double freq2, double *updated_phase1, double *updated_phase2);
		void CosineWave2Tone(float* buf, int samples, double phase1, double phase2, double freq1, double freq2, double *updated_phase1, double *updated_phase2);

		bool use_last;
		double Boxmuller(double m,double s);
		void Noise(float* buf, int samples);

		int tri_direction;
		double tri_val;
		void Triangle(float* buf, int samples, double freq);

		double saw_val;
		int saw_direction;
		void Sawtooth(float* buf, int samples, double freq);

		void ClearBuffer(float *buf, int samples);

public slots:
		void OnGlitchCompChanged(QByteArray p);

		void SetRx2On(bool on);
		void SetRx1Volume(int Val);
		void SetRx2Volume(int Val);
		void SetRx1Balance(int Val);
		void SetRx2Balance(int Val);
		int  SetRx2Osc(double Freq);

		void SetRitEnable(bool state);
		void SetRitValue(int value);


signals:
		void TrxModeChanged(bool TxMode);
};

#endif /* DTTSP_H_ */
