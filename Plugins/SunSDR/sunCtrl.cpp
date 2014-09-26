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

#include "sunCtrl.h"
#include "sunsdr.h"

#define USE_INVERSION	1

#define CheckCnt(Produce, Consume)	((Produce >= Consume) ? (Produce - Consume) : (0x10000 + Produce - Consume))

static const BYTE DdsDefaultRegValue[40] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
											 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
											 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
											 0x00, 0x40, 0x00, 0x00, 0x00, 0x10, 0x64, 0x01,
											 0x20, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00};

sunCtrl::sunCtrl() : QThread()
{
	IsLoaded 	= true;
	IsStartDDS 	= false;
	SettingFreq = 0;
	CurrentFreq = 0;

	KeyPtt 	= false;
	KeyDash = false;
	KeyDot 	= false;

	TKeyPtt  = false;
	TKeyDash = false;
	TKeyDot	 = false;

	C0Reg = C1Reg = C2Reg = C3Reg = D0Reg = D1Reg = D2Reg = D3Reg = 0;

	D2Reg = 0x04;
    D0Reg = 0x20;

	pLib = new QLibrary("sCtrl");
	pLib->load();
	if(!pLib->isLoaded())
		return;
	fInit    = (CtrlFunc3)pLib->resolve("init");
	fDeinit  = (CtrlFunc3)pLib->resolve("deinit");
	fOpen    = (CtrlFunc0)pLib->resolve("open");
	fClose   = (CtrlFunc3)pLib->resolve("close");
	fIsOpen  = (CtrlFunc4)pLib->resolve("isOpen");
	fWrite   = (CtrlFunc2)pLib->resolve("write");
	fRead    = (CtrlFunc5)pLib->resolve("read");

	Opened = false;
	DdsFreqMem = 0;
	CurrentBPF = -1;
	TempPreampState = 0;

    preamp.Gain = true;
    preamp.Att  = true;
    preamp.Amp1 = false;

    preampSave = preamp;

    oscVhf = 120000000;
    oscUhf = 102400000*4;

    XvEnable = false;
    XvBand = false;
    AntMode = 0;

	fInit();
}

sunCtrl::~sunCtrl()
{
	if(isOpen())
		close();

	fDeinit();
}

void sunCtrl::open(int SdrNum, int DdsMul)
{
	mut.lock();

    DdsFreqMem = 0;
    CurrentBPF = -1;
	fOpen(SdrNum);
	if(!Opened)
	{
		if(fIsOpen())
		{
			Opened = true;
			Ufwd = 0;
			Uref = 0;
            start(QThread::NormalPriority);
        	write574Reg(0, C0Reg);
        	write574Reg(1, C1Reg);
        	write574Reg(2, C2Reg);
        	write574Reg(3, C3Reg);
        	write595Reg(0, D0Reg);
        	write595Reg(1, D1Reg);
        	write595Reg(2, D2Reg);
        	write595Reg(3, D3Reg);
        	sendData();
			write574Reg(3, 0x40);
			write574Reg(3, 0xC0);
			write574Reg(3, 0x40);
			write574Reg(3, 0x40);
			mut.unlock();
			writeDDSReg(0x1D, 1<<4);
			if(DdsMul==1)
				writeDDSReg(0x1E, 1<<5);
			else if(DdsMul>1 && DdsMul<21)
				writeDDSReg(0x1E, DdsMul&0x1F);
			writeDDSReg(0x20, 1<<6);
			mut.lock();
			sendData();
		}
	}
	mut.unlock();
}

void sunCtrl::close()
{
	mut.lock();
    DdsFreqMem = 0;
    CurrentBPF = -1;
	C0Reg = C1Reg = C2Reg = C3Reg = D0Reg = D1Reg = D2Reg = D3Reg = 0;
    D2Reg = 0x04;
	write574Reg(3, 0x40);
	write574Reg(3, 0xC0);
	write574Reg(3, 0x40);
	write574Reg(3, 0x40);
	mut.unlock();
	for(int i = 0; i < 40; i++)
		writeDDSReg(i, DdsDefaultRegValue[i]);
	mut.lock();

	write574Reg(0, C0Reg);
	write574Reg(1, C1Reg);
	write574Reg(2, C2Reg);
	write574Reg(3, C3Reg);
	write595Reg(0, D0Reg);
	write595Reg(1, D1Reg);
	write595Reg(2, D2Reg);
	write595Reg(3, D3Reg);
	sendData();
    Opened = false;
    msleep(3);
    if(isRunning())
    {
    	terminate();
    	wait();
    }
    fClose();
    mut.unlock();
}

bool  sunCtrl::isOpen()
{
	return Opened;
}

void sunCtrl::setPreamp(int Preamp)
{
	switch(Preamp & 0x7)
	{
		case 0:
			setGain(true);
			setAttn(true);
			setAmp1(false);
		break;
		case 1:
			setGain(true);
			setAttn(false);
			setAmp1(false);
		break;
		case 2:
			setGain(false);
			setAttn(true);
			setAmp1(false);
		break;
		case 3:
			setGain(false);
			setAttn(false);
			setAmp1(false);
		break;
		case 4:
			setGain(false);
			setAttn(false);
			setAmp1(true);
		break;
		default:
		break;
	}
	setDdsFreq(DdsFreqMem);
}

void sunCtrl::setExtCtrl(DWORD ExtData)
{
	QMutexLocker locker(&mut);

	C0Reg &= 0x80;
	C0Reg |= (ExtData & 0x0000007F);
    D0Reg &= ~(1<<6);
    D0Reg |= ExtData & (1<<6);

    write574Reg(0, C0Reg);
    write595Reg(0, D0Reg);
	sendData();
}

void sunCtrl::setDdsFreq(int Freq)
{
    quint32 tmpFreq;
    if((Freq > 130000000) && (Freq < 150000000))
        tmpFreq = Freq - oscVhf;
    else if((Freq > 420000000) && (Freq < 450000000))
        tmpFreq = Freq - oscUhf;
    else
        tmpFreq = Freq;

    DdsFreqMem = Freq;
	quint64 Ftw = 0;
    long double Ftwf = (0x1000000000000 / 200000000.0)*tmpFreq;
	Ftw = (quint64)Ftwf;
	for(int i = 4; i<10; i++)
	{
		BYTE V = (BYTE)(Ftw>>((9-i)*8));
		writeDDSReg(i, V);
	}
	if(Freq < 2500000)
    {
        setXven(false);
		setBPF(0);
    }
	else if((2500000 <= Freq) && (Freq < 4000000))
    {
        setXven(false);
		setBPF(1);
    }
	else if((4000000 <= Freq) && (Freq < 6000000))
    {
        setXven(false);
		setBPF(2);
    }
	else if((6000000 <= Freq) && (Freq < 7300000))
    {
        setXven(false);
		setBPF(3);
    }
	else if((7300000 <= Freq) && (Freq < 12000000))
    {
        setXven(false);
		setBPF(4);
    }
	else if((12000000 <= Freq) && (Freq < 14500000))
    {
        setXven(false);
		setBPF(5);
    }
	else if((14500000 <= Freq) && (Freq < 19000000))
    {
        setXven(false);
		setBPF(6);
    }
	else if((19000000 <= Freq) && (Freq < 21500000))
    {
        setXven(false);
		setBPF(7);
    }
	else if((21500000 <= Freq) && (Freq < 25200000))
    {
        setXven(false);
		setBPF(8);
    }
	else if((25200000 <= Freq) && (Freq < 30000000))
    {
        setXven(false);
		setBPF(9);
    }
	else if((30000000 <= Freq) && (Freq < 65000000))
    {
        setXven(false);
		setBPF(10);
    }
    else if((140000000 <= Freq) && (Freq < 150000000))
    {
        setXven(true);
        setXvBand(0);
        setBPF(9);
    }
    else if((420000000 <= Freq) && (Freq < 450000000))
    {
        setXven(true);
        setXvBand(1);
        setBPF(9);
    }
	else
	{
		mut.lock();
		sendData();
		mut.unlock();
	}
}

void sunCtrl::setTrxMode(bool Mode)
{
	if(Mode)
	{
        preampSave = preamp;
		setGain(false);
		setAttn(false);
		setAmp1(false);
		mut.lock();
		D1Reg |= 0x02;
		write595Reg(1, D1Reg);
        C1Reg |= 0x40;
		write574Reg(1, C1Reg);
		mut.unlock();
		setAmp2(true);
	}
	else
	{
		mut.lock();
        C1Reg &= ~0x40;
        write574Reg(1, C1Reg);
        mut.unlock();
        setAmp2(false);
        setGain(preampSave.Gain);
        setAttn(preampSave.Att);
        setAmp1(preampSave.Amp1);
	}
	setDdsFreq(DdsFreqMem);
}

void sunCtrl::setMute(bool Status)
{
	QMutexLocker locker(&mut);
	if(fIsOpen())
	{
		(Status) ? C1Reg &= ~0x80 : C1Reg |= 0x80;
		write574Reg(1, C1Reg);
	}
	sendData();
}

void sunCtrl::setVhfOsc(quint32 freq)
{
    oscVhf = freq;
}

void sunCtrl::setUhfOsc(quint32 freq)
{
    oscUhf = freq;
}

inline void sunCtrl::write(WORD PortAddr, BYTE Data)
{

	PortAddr &= 0x0003;
	if(PortAddr == 0)
	{
		circleBuf[produceIndex++] = 0x0F & Data;
		circleBuf[produceIndex++] = (0x0F & (Data >> 4)) | 0x10;
	}
	else
	{
		circleBuf[produceIndex++] = (0x0F & Data) | 0xF0;
		circleBuf[produceIndex++] = (0x0F & Data) | 0xF0;
	}
}

inline void sunCtrl::write574Reg(BYTE Cx, BYTE Data)
{
#if USE_INVERSION
	write(SCTRL_CONTROL, 0x05);
	write(SCTRL_CONTROL, 0x05);
	write(SCTRL_CONTROL, 0x05);
	write(SCTRL_DATA, Data);
	switch(Cx)
	{
		case 0:
			write(SCTRL_CONTROL, 0x04);
			write(SCTRL_CONTROL, 0x05);
		break;

		case 1:
			write(SCTRL_CONTROL, 0x07);
			write(SCTRL_CONTROL, 0x05);
		break;

		case 2:
			write(SCTRL_CONTROL, 0x01);
			write(SCTRL_CONTROL, 0x05);
		break;

		case 3:
			write(SCTRL_CONTROL, 0x0D);
			write(SCTRL_CONTROL, 0x05);
		break;

		default:
			break;
	}
	write(SCTRL_CONTROL, 0x05);
#else
	write(SCTRL_CONTROL, 0x0F);
	write(SCTRL_CONTROL, 0x0F);
	write(SCTRL_CONTROL, 0x0F);
	write(SCTRL_DATA, Data);
	switch(Cx)
	{
		case 0:
			write(SCTRL_CONTROL, 0x0E);
			write(SCTRL_CONTROL, 0x0F);
		break;

		case 1:
			write(SCTRL_CONTROL, 0x0D);
			write(SCTRL_CONTROL, 0x0F);
		break;

		case 2:
			write(SCTRL_CONTROL, 0x0B);
			write(SCTRL_CONTROL, 0x0F);			
		break;

		case 3:
			write(SCTRL_CONTROL, 0x07);
			write(SCTRL_CONTROL, 0x0F);
		break;

		default:
			break;
	}
	write(SCTRL_CONTROL, 0x0F);
	write(SCTRL_CONTROL, 0x0F);
	write(SCTRL_CONTROL, 0x0F);
#endif
}

inline void sunCtrl::write595Reg(BYTE Cx, BYTE Data)
{
	C1Reg &= 0xC0;
	C1Reg |= 0x24 | ((Cx & 0x3) << 3);
	for(int i = 7; i >=0; i--)
	{
		C1Reg &= 0xFC;
		C1Reg |= 0x01 & (Data>>i);
		write574Reg(1, C1Reg);
		C1Reg |= 0x02;
		write574Reg(1, C1Reg);
	}
	C1Reg &= 0xDF;
	write574Reg(1, C1Reg);
	C1Reg |= 0x20;
	write574Reg(1, C1Reg);
}

void sunCtrl::writeDDSReg(BYTE Addr, BYTE Data)
{
	QMutexLocker locker(&mut);
	BYTE b1 = Data;
	BYTE b2 = (Addr | 0x40);
	BYTE b3 = Addr;
	BYTE b4 = 0x40;

	write574Reg(2, b1);
	write574Reg(3, b2);
	write574Reg(3, b3);
	write574Reg(3, b4);
}

void sunCtrl::setGain(bool Status)
{
	QMutexLocker locker(&mut);
    preamp.Gain = Status;
    (Status) ? C0Reg &= ~0x80 : C0Reg |= 0x80;
    write574Reg(0, C0Reg);
}

void sunCtrl::setBPF(int BpfNum)
{
	QMutexLocker locker(&mut);
	if(BpfNum == CurrentBPF)
	{
		sendData();
		return;
	}
	CurrentBPF = BpfNum;
	switch(BpfNum)
	{
		case 0:
			BpfNum = 9;
		break;

		case 1:
			BpfNum = 7;
		break;

		case 2:
			BpfNum = 2;
		break;

		case 3:
			BpfNum = 5;
		break;

		case 4:
			BpfNum = 4;
		break;

		case 5:
			BpfNum = 3;
		break;

		case 6:
			BpfNum = 8;
		break;

		case 7:
			BpfNum = 8;
		break;

		case 8:
			BpfNum = 6;
		break;

		case 9:
			BpfNum = 6;
		break;

		case 10:
			BpfNum = 1;
		break;

		default:
			BpfNum = 0;
		break;
	}
	if(BpfNum < 8)
	{
		write595Reg(3, 1<<BpfNum);
        write595Reg(2, 0x04);
	}
	else
	{
		if(BpfNum < 10)
		{
			write595Reg(3, 0);
            write595Reg(2, (1<<(BpfNum-8))|(0x04));
		}
		else
		{
			write595Reg(3, 0);
            write595Reg(2, 0x04);
		}
	}
	sendData();
}

void sunCtrl::setAmp2(bool Mode)
{
	QMutexLocker locker(&mut);
	D1Reg &= 0xFE;
	D1Reg |= (BYTE)Mode;
	write595Reg(1, D1Reg);
	sendData();
}

void sunCtrl::setAmp1(bool Mode)
{
	QMutexLocker locker(&mut);
	D1Reg |= 1<<1;
    preamp.Amp1 = false;
	if((C1Reg & 0x40) == 0)
	{
		if(Mode)
        {
			D1Reg &= 0xFD;
            preamp.Amp1 = Mode;
        }
	}
	write595Reg(1, D1Reg);
}

void sunCtrl::setCalGen(bool Mode)
{
	QMutexLocker locker(&mut);
    (Mode) ? D1Reg |= 0x20 : D1Reg &= 0xDF;
	write595Reg(1, D1Reg);
	sendData();
}

void sunCtrl::setXvAnt(int Mode)
{
    if(Mode < 0) Mode = 0;
    if(Mode > 1) Mode = 1;
    AntMode = Mode;
    if(XvBand)  Mode = 0;
    QMutexLocker locker(&mut);
    (Mode == 0) ? D1Reg |= 0x04 : D1Reg &= 0xFB;
    write595Reg(1, D1Reg);
    sendData();
}

void sunCtrl::setXvBand(int Mode)
{
    if(Mode < 0) Mode = 0;
    if(Mode > 1) Mode = 1;

    XvBand = (bool) Mode;
    QMutexLocker locker(&mut);
    (Mode == 1) ? D1Reg |= 0x80 : D1Reg &= 0x7F;
    if(XvBand) 	Mode = 0;
    else        Mode = AntMode;
    (Mode == 0) ? D1Reg |= 0x04 : D1Reg &= 0xFB;
    write595Reg(1, D1Reg);
    sendData();
}

void sunCtrl::setXven(bool Mode)
{
	QMutexLocker locker(&mut);
    XvEnable = Mode;
    (Mode) ? D1Reg |= 0x08 : D1Reg &= 0xF7;
    write595Reg(1, D1Reg);
    sendData();
}

void sunCtrl::setAttn(bool Mode)
{
	QMutexLocker locker(&mut);
    preamp.Att  = Mode;
	(Mode) ? D1Reg |= 0x10 : D1Reg &= 0xEF;
	write595Reg(1, D1Reg);
}

void sunCtrl::setImprEn(bool Mode)
{
	QMutexLocker locker(&mut);
	D1Reg &= 0xDF;
	D1Reg |= ((BYTE)Mode)<<5;
	write595Reg(1, D1Reg);
	sendData();
}

void sunCtrl::setPaBias(bool Mode)
{
	QMutexLocker locker(&mut);
	D1Reg &= 0xBF;
	D1Reg |= ((BYTE)Mode)<<6;
	write595Reg(1, D1Reg);
	sendData();
}

void sunCtrl::setImpr(bool Mode)
{
	QMutexLocker locker(&mut);
	D1Reg &= 0x7F;
	D1Reg |= ((BYTE)Mode)<<7;
	write595Reg(1, D1Reg);
	sendData();
}

void sunCtrl::setAdc(bool nCS, bool Sclk, bool Dout)
{
	QMutexLocker locker(&mut);
	D0Reg &= ~0x38;
	if(nCS)  D0Reg |= (0x1 << 5);
	if(Sclk) D0Reg |= (0x1 << 3);
	if(Dout) D0Reg |= (0x1 << 4);

	write595Reg(0, D0Reg);
	sendData();
}

BYTE sunCtrl::adcProc()
{
	BYTE Data;
    static BYTE AdcReg = 0x00;
    static BYTE AdcData;
	static int Status = 0;
	static int count = 0;
	switch(Status)
	{
		case 0:
            if(count != 0) AdcReg = 0x08;
            else AdcReg = 0x00;
			setAdc(false, true, (bool)(AdcReg & 0x80));
			Data = fRead();
		break;
		case 1:
			setAdc(false, false, (bool)(AdcReg & 0x80));
			Data = fRead();
		break;
		case 2:
			setAdc(false, true, (bool)(AdcReg & 0x80));
			Data = fRead();
		break;
		case 3:
			setAdc(false, false, (bool)(AdcReg & 0x40));
			Data = fRead();
		break;
		case 4:
			setAdc(false, true, (bool)(AdcReg & 0x40));
			Data = fRead();
			AdcData = 0;
		break;
		case 5:
			setAdc(false, false, (bool)(AdcReg & 0x20));
			Data = fRead();
		break;
		case 6:
			setAdc(false, true, (bool)(AdcReg & 0x20));
			Data = fRead();
		break;
		case 7:
			setAdc(false, false, (bool)(AdcReg & 0x10));
			Data = fRead();
		break;
		case 8:
			setAdc(false, true, (bool)(AdcReg & 0x10));
			Data = fRead();
		break;
		case 9:
			setAdc(false, false, (bool)(AdcReg & 0x08));
			Data = fRead();
		break;
		case 10:
			setAdc(false, true, (bool)(AdcReg & 0x08));
			Data = fRead();
		break;
		case 11:
			setAdc(false, false, (bool)(AdcReg & 0x04));
			Data = fRead();
		break;
		case 12:
			setAdc(false, true, (bool)(AdcReg & 0x04));
			Data = fRead();
			AdcData |= (Data & 0x40) ? 0x01 : 0x00;
			AdcData <<= 1;
		break;
		case 13:
			setAdc(false, false, (bool)(AdcReg & 0x02));
			Data = fRead();
		break;
		case 14:
			setAdc(false, true, (bool)(AdcReg & 0x02));
			Data = fRead();
			AdcData |= (Data & 0x40) ? 0x01 : 0x00;
			AdcData <<= 1;
		break;
		case 15:
			setAdc(false, false, (bool)(AdcReg & 0x01));
			Data = fRead();
		break;
		case 16:
			setAdc(false, true, (bool)(AdcReg & 0x01));
			Data = fRead();
			AdcData |= (Data & 0x40) ? 0x01 : 0x00;
			AdcData <<= 1;
		break;
		case 17:
			setAdc(false, false, false);
			Data = fRead();
		break;
		case 18:
			setAdc(false, true, false);
			Data = fRead();
			AdcData |= (Data & 0x40) ? 0x01 : 0x00;
			AdcData <<= 1;
		break;
		case 19:
			setAdc(false, false, false);
			Data = fRead();
		break;
		case 20:
			setAdc(false, true, false);
			Data = fRead();
			AdcData |= (Data & 0x40) ? 0x01 : 0x00;
			AdcData <<= 1;
		break;
		case 21:
			setAdc(false, false, false);
			Data = fRead();
		break;
		case 22:
			setAdc(false, true, false);
			Data = fRead();
			AdcData |= (Data & 0x40) ? 0x01 : 0x00;
			AdcData <<= 1;
		break;
		case 23:
			setAdc(false, false, false);
			Data = fRead();
		break;
		case 24:
			setAdc(false, true, false);
			Data = fRead();
			AdcData |= (Data & 0x40) ? 0x01 : 0x00;
			AdcData <<= 1;
		break;
		case 25:
			setAdc(false, false, false);
			Data = fRead();
		break;
		case 26:
			setAdc(false, true, false);
			Data = fRead();
			AdcData |= (Data & 0x40) ? 0x01 : 0x00;
		break;
		case 27:
			setAdc(false, false, false);
			Data = fRead();
		break;
		case 28:
			setAdc(false, true, false);
			Data = fRead();
		break;
		case 29:
			setAdc(false, false, false);
			Data = fRead();
		break;
		case 30:
			setAdc(false, true, false);
			Data = fRead();
		break;
		case 31:
			setAdc(false, false, false);
			Data = fRead();
		break;
		case 32:
			setAdc(false, true, false);
			Data = fRead();
		break;
		case 33:
			setAdc(false, false, false);
			Data = fRead();
		break;
		case 34:
			setAdc(false, true, false);
			Data = fRead();
		break;
		case 35:
			setAdc(true, true, false);
			Data = fRead();
		break;
		default:
            if(Status > 40)
			{
                Status = -1;
				if(count != 0)
				{
                    Ufwd = AdcData;
                    AdcChanged(Ufwd, Uref);
				}
				else
                    Uref = AdcData;
				if(count == 0) count = 1;
				else count = 0;
			}
			Data = fRead();
		break;
	}
	Status++;
	return Data;
}

void sunCtrl::sendData()
{
	int  Val;
	LONG Len = 0;
	BYTE TxBuf[MAX_TX_LEN];
	while(1)
	{
		if(Opened)
		{
			Val = CheckCnt(produceIndex, consumeIndex);
			if(Val > 0)
			{
				if(Val > MAX_TX_LEN)
				{
					Len = MAX_TX_LEN;
					for(int i = 0; i < Len; i++)
						TxBuf[i] = circleBuf[consumeIndex++];
					fWrite(TxBuf, Len);
				}
				else
				{
					Len = Val;
					for(int i = 0; i < Len; i++)
						TxBuf[i] = circleBuf[consumeIndex++];
					fWrite(TxBuf, Len);
				}
			}
			else
				break;
		}
		else
		{
			consumeIndex = produceIndex;
			break;
		}
	}
}

void sunCtrl::run()
{
    BYTE Data = 0;
	
    while(1)
	{
        Data = adcProc();
		if(((Data & 0x80) ? true : false) != KeyPtt)
		{
			KeyPtt  = ((Data & 0x80) ? true : false);
            PttChanged(KeyPtt);
		}
		if(((Data & 0x10) ? true : false) != KeyDash)
		{
			KeyDash = ((Data & 0x10) ? true : false);
            DashChanged(KeyDash);
		}
		if(((Data & 0x20) ? true : false) != KeyDot)
		{
			KeyDot  = ((Data & 0x20) ? true : false);
            DotChanged(KeyDot);
		}
        msleep(2);
	}
}
