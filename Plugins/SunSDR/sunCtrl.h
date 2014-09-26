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
 * Copyright (C) Valery Mikhaylovsky
 * The authors can be reached by email at maksimus1210@gmail.com
 */

#ifndef SUNCTRL_H
#define SUNCTRL_H

#include <QObject>
#include <QtCore>
#include "qt_windows.h"

#define MAX_TX_LEN		64

#define SCTRL_DATA		0
#define SCTRL_CONTROL	2
#define SCTRL_INPUT		1

static const int Prmp[] = {3, 2, 0, 1, 3};

typedef void (*CtrlFunc0)(int);
typedef void (*CtrlFunc2)(BYTE*, int);
typedef void (*CtrlFunc3)();
typedef int  (*CtrlFunc4)();
typedef BYTE  (*CtrlFunc5)();

typedef struct
{
    bool Gain;
    bool Att;
    bool Amp1;
} PREAMP_STATE;

class sunCtrl : public QThread
{
	Q_OBJECT

	public:
		
		sunCtrl();
		~sunCtrl();
		
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

	private:
		QLibrary *pLib;
		CtrlFunc3 fInit;
		CtrlFunc3 fDeinit;
		CtrlFunc0 fOpen;
		CtrlFunc3 fClose;
		CtrlFunc4 fIsOpen;
		CtrlFunc2 fWrite;
		CtrlFunc5 fRead;
		
		bool Opened;
		int TimerID;

		int Ufwd;
		int Uref;

        bool XvEnable;
        bool XvBand;
        int AntMode;

		int CurrentBPF;

		BYTE C0Reg, C1Reg, C2Reg, C3Reg;
		BYTE D0Reg, D1Reg, D2Reg, D3Reg;
		BYTE circleBuf[0x10000];
        PREAMP_STATE preamp, preampSave;

        bool GainVal;
        bool AttVal;
        bool Amp1Val;

		WORD produceIndex;
		WORD consumeIndex;
		
		void write(WORD PortAddr, BYTE Data);
		void sendData();
		void write574Reg(BYTE Cx, BYTE Data);
		void write595Reg(BYTE Cx, BYTE Data);
		void writeDDSReg(BYTE Addr, BYTE Data);


		void setGain(bool Status);
		void setBPF(int BpfNum);
		void setAmp2(bool Mode);
		void setAmp1(bool Mode);
		void setXven(bool Mode);
		void setAttn(bool Mode);
		void setImprEn(bool Mode);
		void setPaBias(bool Mode);
		void setImpr(bool Mode);
		void setAdc(bool nCS, bool Sclk, bool Dout);
        void setXvBand(int Mode);
		BYTE adcProc();
		
		bool IsLoaded;
		bool IsStartDDS;
		int CurrentFreq;
		int SettingFreq;
		int DdsFreqMem;
		BYTE TempPreampState;


		QMutex	Mutex;
		QMutex	MutexKeys;
		QMutex	mut;
		bool	KeyPtt, TKeyPtt;
		bool	KeyDash, TKeyDash;
		bool	KeyDot, TKeyDot;
		void run();

        quint32 oscVhf, oscUhf;
};

#endif // SUNCTRL_H
