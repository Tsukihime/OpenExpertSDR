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

#include "SMeter.h"

SMeter::SMeter(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);

	Tx = false;
	pRxSmeter = new QAction(tr("&S-meter"), this);
	pRxSmeter->setStatusTip(tr("View S-meter"));
	pRxSmeter->setCheckable(true);
	pRxSmeter->setChecked(true);
	pRxLevelMeter = new QAction(tr("&Level-meter"), this);
	pRxLevelMeter->setStatusTip(tr("View Level-meter"));
	pRxLevelMeter->setCheckable(true);
	pTxMicMeter = new QAction(tr("&Mic-meter"), this);
	pTxMicMeter->setStatusTip(tr("View Mic-meter"));
	pTxMicMeter->setCheckable(true);
	pTxMicMeter->setChecked(true);
	pTxPwrMeter = new QAction(tr("&Power-meter"), this);
	pTxPwrMeter->setStatusTip(tr("View Power-meter"));
	pTxPwrMeter->setCheckable(true);
	pTxSwrMeter = new QAction(tr("&SWR-meter"), this);
	pTxSwrMeter->setStatusTip(tr("View SWR-meter"));
	pTxSwrMeter->setCheckable(true);
	pRxActGroup = new QActionGroup(this);
	pRxActGroup->addAction(pRxSmeter);
	pRxActGroup->addAction(pRxLevelMeter);
	pTxActGroup = new QActionGroup(this);
	pTxActGroup->addAction(pTxMicMeter);
	pTxActGroup->addAction(pTxPwrMeter);
	pTxActGroup->addAction(pTxSwrMeter);
	pSMeterNum = new SMeterNum(ui.wgtNum);
	pSMeterScale = new SMeterScale(ui.widget);
	pSMeterNum->SetNum(0);
	pSMeterScale->SetNumMax(0);
	SMeterType = 0;
	pSMeterScale->SetTypeSMeter((int)SMeterType);
	pSMeterScale->setFixedWidth(ui.widget->width());
	iS = 0;
	iL = 0;
	iM = 0;
	iP = 0;
	iSWR = 0;
	ColorMainNum = QColor(197, 197, 197, 255);
	ColorNumLow = Qt::white;
	ColorNumHigh = QColor(255, 100, 100, 255);
	ColorLineLow = Qt::white;
	ColorLineHigh = QColor(255, 100, 100, 255);
	ColorLineSketch = Qt::black;
	ColorLineBackLow = Qt::yellow;
	ColorLineBackHigh = QColor(218, 78, 18, 255);
	ColorLevelView = QColor(197, 197, 197, 255);
	ColorMaxLevelView = QColor(255, 135, 55, 255);
	pSMeterNum->SetColorNum(ColorMainNum);
	pSMeterScale->SetColorNumLow(ColorNumLow);
	pSMeterScale->SetColorNumHigh(ColorNumHigh);
	pSMeterScale->SetColorLineLow(ColorLineLow);
	pSMeterScale->SetColorLineHigh(ColorLineHigh);
	pSMeterScale->SetColorLineSketch(ColorLineSketch);
	pSMeterScale->SetColorLineBackLow(ColorLineBackLow);
	pSMeterScale->SetColorLineBackHigh(ColorLineBackHigh);
	pSMeterScale->SetColorLevelView(ColorLevelView);
	pSMeterScale->SetColorMaxLevelView(ColorMaxLevelView);
	connect(pRxActGroup, SIGNAL(triggered(QAction *)), this, SLOT(GetRxType(QAction *)));
	connect(pTxActGroup, SIGNAL(triggered(QAction *)), this, SLOT(GetTxType(QAction *)));
}

SMeter::~SMeter()
{
	delete pSMeterNum;
	delete pSMeterScale;
}

void SMeter::GetRxType(QAction *action)
{
	int num = pRxActGroup->actions().indexOf(action);
	if((SMETER_TYPE)num == SMETER)
	{
		pSMeterScale->SetTypeSMeter(num);
		RxType = RX_SMETER;
	}
	else if((SMETER_TYPE)num == LEVEL_METER)
	{
		pSMeterScale->SetTypeSMeter(num);
		RxType = RX_LEVEL_METER;
	}
}

void SMeter::GetTxType(QAction *action)
{
	int num = pTxActGroup->actions().indexOf(action);
	if(num == 0)
	{
		pSMeterScale->SetTypeSMeter(MIC_METER);
		TxType = TX_MIC_METER;
	}
	else if(num == 1)
	{
		pSMeterScale->SetTypeSMeter(PWR_METER);
		TxType = TX_PWR_METER;
	}
	else if(num == 2)
	{
		pSMeterScale->SetTypeSMeter(SWR_METER);
		TxType = TX_SWR_METER;
	}
}

void SMeter::SetTx(bool state)
{
	Tx = state;
	if(Tx)
	{
		switch(TxType)
		{
		case TX_PWR_METER:
			pSMeterScale->SetTypeSMeter(PWR_METER);
			break;
		case TX_SWR_METER:
			pSMeterScale->SetTypeSMeter(SWR_METER);
			break;
		case TX_MIC_METER:
		default:
			pSMeterScale->SetTypeSMeter(MIC_METER);
			break;
		}
	}
	else
	{
		switch(RxType)
		{
		case RX_LEVEL_METER:
			pSMeterScale->SetTypeSMeter(LEVEL_METER);
			break;
		case RX_SMETER:
		default:
			pSMeterScale->SetTypeSMeter(SMETER);
			break;
		}
	}
}

void SMeter::SetTxType(int type)
{
	TxType = (METER_TX_TYPE)type;
	switch(TxType)
	{
	case TX_PWR_METER:
		pTxPwrMeter->setChecked(true);
		pSMeterScale->SetTypeSMeter(PWR_METER);
		break;
	case TX_SWR_METER:
		pTxSwrMeter->setChecked(true);
		pSMeterScale->SetTypeSMeter(SWR_METER);
		break;
	case TX_MIC_METER:
	default:
		pTxMicMeter->setChecked(true);
		pSMeterScale->SetTypeSMeter(MIC_METER);
		break;
	}
}

void SMeter::SetRxType(int type)
{
	RxType = (METER_RX_TYPE)type;

	switch(RxType)
	{
	case RX_LEVEL_METER:
		pRxLevelMeter->setChecked(true);
		pSMeterScale->SetTypeSMeter(LEVEL_METER);
		break;
	case RX_SMETER:
	default:
		pRxSmeter->setChecked(true);
		pSMeterScale->SetTypeSMeter(SMETER);
		break;
	}
}

int SMeter::GetTxType()
{
	return (int)TxType;
}

int SMeter::GetRxType()
{
	return (int)RxType;
}

void SMeter::SetTypeSMeter(int type)
{
	SMeterType = type;
	pSMeterScale->SetTypeSMeter(SMeterType);
}
int SMeter::GetTypeSMeter()
{
	return SMeterType;
}
void SMeter::resizeEvent(QResizeEvent *event)
{
	pSMeterScale->setFixedWidth(ui.widget->width());
}

void SMeter::SetSMeterVal(double Value)
{
	if(RxType == RX_SMETER && !Tx)
	{
		BuffSMeterVal[iS] = Value;
		pSMeterScale->SetSMeterVal(Value);
		pSMeterScale->SetNumMax(MaxSMeter());
		pSMeterNum->SetNum(Value);
		++iS = iS&BUFFLEN;
	}
}

void SMeter::SetLevelMeterVal(double Value)
{
	if(RxType == RX_LEVEL_METER && !Tx)
	{
		BuffSMeterVal[iL] = Value;
		pSMeterScale->SetLevelMeterVal(Value);
		pSMeterScale->SetNumMax(MaxLevel());
		pSMeterNum->SetNum(Value);
		++iL = iL&BUFFLEN;
	}
}

void SMeter::SetMicMeterVal(double Value)
{
	if(TxType == TX_MIC_METER && Tx)
	{
		BuffSMeterVal[iM] = Value;
		pSMeterScale->SetMicMeterVal(Value);
		pSMeterScale->SetNumMax(MaxMic());
		pSMeterNum->SetNum(Value);
		++iM = iM&BUFFLEN;
	}
}

void SMeter::SetPowerMeterVal(double Value)
{
	if(TxType == TX_PWR_METER && Tx)
	{
		BuffSMeterVal[iP] = Value;
		pSMeterScale->SetPowerMeterVal(Value);
		pSMeterScale->SetNumMax(MaxPower());
		pSMeterNum->SetNum(Value);
		++iP = iP&BUFFLEN;
	}
}

void SMeter::SetSwrMeterVal(double Value)
{
	if(TxType == TX_SWR_METER && Tx)
	{
		BuffSMeterVal[iSWR] = Value;
		pSMeterScale->SetSwrMeterVal(Value);
		pSMeterScale->SetNumMax(MaxSWR());
		pSMeterNum->SetNum(Value);
		++iSWR = iSWR&BUFFLEN;
	}
}

void SMeter::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);
	if(Tx)
	{
		menu.addAction(pTxMicMeter);
		menu.addAction(pTxPwrMeter);
		menu.addAction(pTxSwrMeter);
	}
	else
	{
		menu.addAction(pRxSmeter);
		menu.addAction(pRxLevelMeter);
	}
	menu.exec(event->globalPos());
}

inline double SMeter::Max()
{
	double x = -200;
	for(int j = 0; j < BUFFLEN; j++)
	{
		if(Buff[j] > x)
			x = Buff[j];
	}
	return x;
}

inline double SMeter::MaxSMeter()
{
	double x = -200;
	for(int j = 0; j < BUFFLEN; j++)
	{
		if(BuffSMeterVal[j] > x)
			x = BuffSMeterVal[j];
	}
	return x;
}
inline double SMeter::MaxLevel()
{
	double x = -200;
	for(int j = 0; j < BUFFLEN; j++)
	{
		if(BuffLevelMeterVal[j] > x)
			x = BuffLevelMeterVal[j];
	}
	return x;
}
inline double SMeter::MaxMic()
{
	double x = -200;
	for(int j = 0; j < BUFFLEN; j++)
	{
		if(BuffMicMeterVal[j] > x)
			x = BuffMicMeterVal[j];
	}
	return x;
}
inline double SMeter::MaxPower()
{
	double x = 0;
	for(int j = 0; j < BUFFLEN; j++)
	{
		if(BuffPowerMeterVal[j] > x)
			x = BuffPowerMeterVal[j];
	}
	return x;
}
inline double SMeter::MaxSWR()
{
	double x = 1;
	for(int j = 0; j < BUFFLEN; j++)
	{
		if(BuffSwrMeterVal[j] > x)
			x = BuffSwrMeterVal[j];
	}
	return x;
}

void SMeter::SetColorNumLow(QColor color)
{
	ColorNumLow = color;
	pSMeterScale->SetColorNumLow(ColorNumLow);
}
QColor SMeter::GetColorNumLow()
{
	return ColorNumLow;
}
void SMeter::SetColorNumHigh(QColor color)
{
	ColorNumHigh = color;
	pSMeterScale->SetColorNumHigh(ColorNumHigh);
}
QColor SMeter::GetColorNumHigh()
{
	return ColorNumHigh;
}
void SMeter::SetColorLineLow(QColor color)
{
	ColorLineLow = color;
	pSMeterScale->SetColorLineLow(ColorLineLow);
}
QColor SMeter::GetColorLineLow()
{
	return ColorLineLow;
}
void SMeter::SetColorLineHigh(QColor color)
{
	ColorLineHigh = color;
	pSMeterScale->SetColorLineHigh(ColorLineHigh);
}
QColor SMeter::GetColorLineHigh()
{
	return ColorLineHigh;
}
void SMeter::SetColorLineSketch(QColor color)
{
	ColorLineSketch = color;
	pSMeterScale->SetColorLineSketch(ColorLineSketch);
}
QColor SMeter::GetColorLineSketch()
{
	return ColorLineSketch;
}
void SMeter::SetColorMainNum(QColor color)
{
	ColorMainNum = color;
	pSMeterNum->SetColorNum(ColorMainNum);
}
QColor SMeter::GetColorMainNum()
{
	return ColorMainNum;
}
void SMeter::SetColorLineBackLow(QColor color)
{
	ColorLineBackLow = color;
	pSMeterScale->SetColorLineBackLow(ColorLineBackLow);
}
QColor SMeter::GetColorLineBackLow()
{
	return ColorLineBackLow;
}
void SMeter::SetColorLineBackHigh(QColor color)
{
	ColorLineBackHigh = color;
	pSMeterScale->SetColorLineBackHigh(ColorLineBackHigh);
}
QColor SMeter::GetColorLineBackHigh()
{
	return ColorLineBackHigh;
}
void SMeter::SetColorLevelView(QColor color)
{
	ColorLevelView = color;
	pSMeterScale->SetColorLevelView(ColorLevelView);
}
QColor SMeter::GetColorLevelView()
{
	return ColorLevelView;
}
void SMeter::SetColorMaxLevelView(QColor color)
{
	ColorMaxLevelView = color;
	pSMeterScale->SetColorMaxLevelView(ColorMaxLevelView);
}
QColor SMeter::GetColorMaxLevelView()
{
	return ColorMaxLevelView;
}

SMeterNum::SMeterNum(QWidget *parent)
	: QWidget(parent)
{
	Num = 0.0;
	Str =  QString("%L1").arg(Num, 2, 'x', 2);
	Font.setPointSize(18);
	setFont(Font);
}

void SMeterNum::paintEvent(QPaintEvent *event)
{
	Painter.begin(this);
		Painter.setFont(Font);
		Painter.setPen(ColorNum);
		Painter.setBrush(QBrush(ColorNum));
		Painter.drawText(QPoint(2, height()-5), Str);
	Painter.end();
}

void SMeterNum::SetNum(double Value)
{
	Num = Value;
	Str.setNum(Value, 'f', 1);
	update();
}
void SMeterNum::SetColorNum(QColor color)
{
	ColorNum = color;
	update();
}

SMeterScale::SMeterScale(QWidget *parent)
	: QWidget(parent)
{
	Num = 0.0;
	Num2 = 0.0;
	Val = 0;
	Str =  QString("%L1").arg(Num, 2, 'x', 0);
	Font.setPointSize(7.5);
	setFont(Font);
	ColorNumLow = Qt::white;
	SMeterVal = 0;
	LevelMeterVal = 0;
	MicMeterVal = 0;
	PowerMeterVal = 0;
	SwrMeterVal = 0;
}

void SMeterScale::paintEvent(QPaintEvent *event)
{
	Num = Val;
	if(SMeterType == (int)SMETER)
	{
		Num = SMeterVal;
		if(Num < -140)
			Num = -140;
		else if(Num > 20)
			Num = 20;
		Num += 140;

		double step = (width()-SMETER_LEFT_POSITION)/16.0;
		Painter.begin(this);
			Painter.setBrush(Qt::NoBrush);
			Painter.setPen(ColorLineLow);
			Painter.drawLine(QLine(SMETER_LEFT_POSITION, 13, width()-SMETER_LEFT_POSITION, 13));
			Painter.drawLine(QLine(SMETER_LEFT_POSITION, 18, width()-SMETER_LEFT_POSITION, 18));

			Painter.setPen(ColorNumLow);
			Painter.drawText(QPointF(0, 9),"dBm");
			Painter.drawText(QPointF(0, 30),"S");
			int Value = -130;
			int j = 0;
			for(double i = SMETER_LEFT_POSITION; i <= width()-10; i += step, j++)
			{
				Painter.setPen(ColorLineLow);
				if(i > SMETER_LEFT_POSITION+(140/160.0)*(width()-(2*SMETER_LEFT_POSITION)))
				{
					Painter.setPen(ColorLineHigh);
				}
				Painter.drawRect(QRect(i,13, 1, -3));
				if(j == 0)
					continue;

				if(width() < 300)
				{
					if(j%2 == 1)
					{
						Value += 10;
						continue;
					}
				}

				Str.setNum(Value);

				Painter.setPen(ColorNumLow);
				if(i > SMETER_LEFT_POSITION+(140/160.0)*(width()-(2*SMETER_LEFT_POSITION)))
					Painter.setPen(ColorNumHigh);

				if(Value < -99)
					Painter.drawText(QPointF(i-10, 9),Str);
				else if(Value < -9)
					Painter.drawText(QPointF(i-7, 9),Str);
				else if(Value < 10)
					Painter.drawText(QPointF(i-1.5, 9),Str);
				else if(Value < 100)
					Painter.drawText(QPointF(i-5, 9),Str);
				Value += 10;
			}
			Painter.setPen(QColor(197,197,197,255));
			double beg = (67/160.0)*(width()- SMETER_LEFT_POSITION);
			j = 0;
			Value = 9;
			for(double i = SMETER_LEFT_POSITION+beg; i >= SMETER_LEFT_POSITION + (15/160.0)*(width()-SMETER_LEFT_POSITION); i -= (12/160.0)*(width()-(2*SMETER_LEFT_POSITION)), j++)
			{
				Painter.setPen(ColorLineLow);
				Painter.drawRect(QRectF(i,18, -1, 3));
				Painter.setPen(ColorNumLow);
				Str.setNum(Value);
				Painter.drawText(QPointF(i-3, 30.5),Str);
				Value -= 2;
			}

			step = (20/160.0)*(width()-(2*SMETER_LEFT_POSITION));
			Value = 20;
			for(double i = SMETER_LEFT_POSITION+beg+step; i <= (width()-SMETER_LEFT_POSITION); i += step)
			{
				Painter.setPen(ColorLineHigh);
				Painter.drawRect(QRectF(i,18, -1, 3));

				Painter.setPen(ColorNumHigh);
				Str.setNum(Value);
				Painter.drawText(QPointF(i-10, 30.5),"+"+Str);
				Value += 20;
			}
			Painter.setPen(ColorLineHigh);
			Painter.drawLine(QLine(SMETER_LEFT_POSITION+beg+1, 18, width()-SMETER_LEFT_POSITION, 18));
			Painter.drawLine(QLine(SMETER_LEFT_POSITION+(140/160.0)*(width()-(SMETER_LEFT_POSITION)), 13, width()-SMETER_LEFT_POSITION, 13));
			Painter.setBrush(ColorLineBackLow);
			Painter.setPen(Qt::NoPen);
			Painter.drawRect(QRectF(SMETER_LEFT_POSITION,15, beg+1, 2));
			Painter.setBrush(ColorLineBackHigh);
			Painter.setPen(Qt::NoPen);
			Painter.drawRect(QRectF(width()-9,15, -(width()-(2*SMETER_LEFT_POSITION))+(beg), 2));
			Painter.setPen(Qt::NoPen);
			Painter.setBrush(QBrush(ColorLineSketch));
			float x = (SMETER_LEFT_POSITION + (160/160.0)*(width()-SMETER_LEFT_POSITION)) - (SMETER_LEFT_POSITION + ((Num)/160.0)*(width()-SMETER_LEFT_POSITION));
			Painter.drawRect(QRectF(SMETER_LEFT_POSITION + ((Num)/160.0)*(width()-SMETER_LEFT_POSITION), 14, x - SMETER_LEFT_POSITION+1, 4));
			Painter.setPen(ColorMaxLevelView);
			Painter.setBrush(ColorMaxLevelView);
			if(Num > Num2)
				Painter.drawRect(QRectF(SMETER_LEFT_POSITION + ((Num)/160.0)*(width()-SMETER_LEFT_POSITION), 7, 1, 17));
			else
				Painter.drawRect(QRectF(SMETER_LEFT_POSITION + ((Num2)/160.0)*(width()-SMETER_LEFT_POSITION), 7, 1, 17));
			Painter.setPen(ColorLevelView);
			Painter.setBrush(ColorLevelView);
			Painter.drawRect(QRectF(SMETER_LEFT_POSITION + ((Num)/160.0)*(width()-SMETER_LEFT_POSITION), 7, 1, 17));
		Painter.end();
	}
	else if(SMeterType == MIC_METER)
	{
		Num = MicMeterVal;
		Font.setPointSize(9);
		Painter.begin(this);
			Painter.setFont(Font);
			Painter.setBrush(ColorLineLow);
			Painter.setPen(ColorLineLow);
			Painter.drawRect(QRectF(SMETER_LEFT_POSITION - 2, 27, 0.7*(width()-2*SMETER_LEFT_POSITION) +2, 2));
			Painter.setBrush(ColorLineHigh);
			Painter.setPen(ColorLineHigh);
			Painter.drawRect(QRectF(SMETER_LEFT_POSITION+0.7*(width()-2*SMETER_LEFT_POSITION), 27, 0.3*(width()-2*SMETER_LEFT_POSITION), 2));
			float stp = (0.7*(width()-2.0*SMETER_LEFT_POSITION))/3.0;
			Painter.setBrush(ColorLineLow);
			Painter.setPen(ColorLineLow);
			int Data = 0;
			QString Str;
			Str.setNum(Data);
			for(float i = SMETER_LEFT_POSITION+0.7*(width()-2*SMETER_LEFT_POSITION); i > 0; i -= stp)
			{
				Painter.setBrush(ColorLineLow);
				Painter.setPen(ColorLineLow);
				Painter.drawRect(QRectF(i-1, 27, -1, -7));
				Painter.drawRect(QRectF(i-1 - stp/2, 27, -1, -3));
				Str.setNum(Data);
				Painter.setBrush(ColorNumLow);
				Painter.setPen(ColorNumLow);
				if(Data < -99)
					Painter.drawText(QPointF(i-10, 18),Str);
				else if(Data < -9)
					Painter.drawText(QPointF(i-12, 18),Str);
				else if(Data < 10)
					Painter.drawText(QPointF(i-5, 18),Str);
				else if(Data < 100)
					Painter.drawText(QPointF(i-9, 18),Str);

				Str.setNum(Data-10);
				if(Data-10 < -99)
					Painter.drawText(QPointF(i-10 - stp/2, 18),Str);
				else if(Data-10 < -9)
					Painter.drawText(QPointF(i-12 - stp/2, 18),Str);
				else if(Data-10 < 10)
					Painter.drawText(QPointF(i-5 - stp/2, 18),Str);
				else if(Data-10 < 100)
					Painter.drawText(QPointF(i-9 - stp/2, 18),Str);

					Data -= 20;
			}
			Painter.setBrush(ColorLineHigh);
			Painter.setPen(ColorLineHigh);
			stp = (0.3*(width()-2.0*SMETER_LEFT_POSITION))/3.0;
			Data = 1;
			for(float i = SMETER_LEFT_POSITION+0.7*(width()-2*SMETER_LEFT_POSITION) + stp; i < width(); i += stp)
			{
				Painter.setBrush(ColorLineHigh);
				Painter.setPen(ColorLineHigh);
				Painter.drawRect(QRectF(i, 27, -1, -7));
				Painter.drawRect(QRectF(i - stp/2, 27, -1, -3));
				Painter.setBrush(ColorNumHigh);
				Painter.setPen(ColorNumHigh);
				Str.setNum(Data);
				if(Data < -99)
					Painter.drawText(QPointF(i-10, 18),Str);
				else if(Data < -9)
					Painter.drawText(QPointF(i-12, 18),Str);
				else if(Data < 10)
					Painter.drawText(QPointF(i-3, 18),Str);
				else if(Data < 100)
					Painter.drawText(QPointF(i-9, 18),Str);

				Data += 1;
			}
			Painter.setPen(ColorLevelView);
			Painter.setBrush(ColorLevelView);
			if(Num <= 0)
			{
				if(Num < -60)
					Num = -60;
				else if(Num >0)
					Num = 0;
				Num += 60;
				Num *= 100.0/60;
				Painter.drawRect(QRectF((SMETER_LEFT_POSITION + 0.7*(width()-2*SMETER_LEFT_POSITION)*(Num/100.0)) - 2, 5, 1, 24));
			}
			else if(Num > 0)
			{
				if(Num > 3)
					Num = 3;
				Num *= 100.0/3;
				Painter.drawRect(QRectF((SMETER_LEFT_POSITION + 0.7*(width()-2*SMETER_LEFT_POSITION) + 0.3*(width()-2*SMETER_LEFT_POSITION)*(Num/100.0)) - 1, 5, 1, 24));
			}
		Painter.end();
	}
	else if(SMeterType == LEVEL_METER)
	{
		Num = LevelMeterVal;
		float stp = (width()-2*SMETER_LEFT_POSITION)/6.0;
		Font.setPointSize(9);
		Painter.begin(this);
			Painter.setFont(Font);
			Painter.setBrush(ColorLineLow);
			Painter.setPen(ColorLineLow);
			Painter.drawRect(QRectF(SMETER_LEFT_POSITION, 27, width()-2*SMETER_LEFT_POSITION-2, 2));
			int Data = -120;
			for(float i = SMETER_LEFT_POSITION; i < (SMETER_LEFT_POSITION+(width()-2*SMETER_LEFT_POSITION) - stp/2); i += stp)
			{
				Painter.setBrush(ColorLineLow);
				Painter.setPen(ColorLineLow);
				Painter.drawRect(QRectF(i, 27, 1, -7));
				Painter.drawRect(QRectF(i - stp/2, 27, 1, -3));
				Painter.setBrush(ColorNumLow);
				Painter.setPen(ColorNumLow);
				if(i == SMETER_LEFT_POSITION)
				{
					Data += 20;
					continue;
				}
				Str.setNum(Data);
				if(Data < -99)
					Painter.drawText(QPointF(i-13, 18),Str);
				else if(Data < -9)
					Painter.drawText(QPointF(i-10, 18),Str);
				else if(Data < 10)
					Painter.drawText(QPointF(i-3, 18),Str);
				else if(Data < 100)
					Painter.drawText(QPointF(i-9, 18),Str);
				if(width() > 350)
				{
					Str.setNum(Data-10);
					if(Data+10 < -99)
						Painter.drawText(QPointF(i-13 - stp/2, 18),Str);
					else if(Data-10 < -9)
						Painter.drawText(QPointF(i-10 - stp/2, 18),Str);
					else if(Data-10 < 10)
						Painter.drawText(QPointF(i-3 - stp/2, 18),Str);
					else if(Data-10 < 100)
						Painter.drawText(QPointF(i-9 - stp/2, 18),Str);
				}

				Data += 20;
			}
			Painter.setBrush(ColorLineHigh);
			Painter.setPen(ColorLineHigh);
			Painter.drawRect(QRectF(SMETER_LEFT_POSITION+(width()-2*SMETER_LEFT_POSITION) - stp + 2, 27, stp - 3, 2));
			for(float i = SMETER_LEFT_POSITION+(width()-2*SMETER_LEFT_POSITION)-1; i < width();i += stp)
			{
				Painter.setBrush(ColorLineHigh);
				Painter.setPen(ColorLineHigh);
				Painter.drawRect(QRectF(i+1, 27, 1, -7));
				Painter.drawRect(QRectF(i - stp/2+1, 27, 1, -3));
				Painter.setBrush(ColorNumHigh);
				Painter.setPen(ColorNumHigh);
				Str.setNum(Data);
				if(Data < -99)
					Painter.drawText(QPointF(i-13, 18),Str);
				else if(Data < -9)
					Painter.drawText(QPointF(i-10, 18),Str);
				else if(Data < 10)
					Painter.drawText(QPointF(i-3, 18),Str);
				else if(Data < 100)
					Painter.drawText(QPointF(i-9, 18),Str);
				if(width() > 350)
				{
					Str.setNum(Data-10);
					if(Data+10 < -99)
						Painter.drawText(QPointF(i-13 - stp/2, 18),Str);
					else if(Data-10 < -9)
						Painter.drawText(QPointF(i-10 - stp/2, 18),Str);
					else if(Data-10 < 10)
						Painter.drawText(QPointF(i-3 - stp/2, 18),Str);
					else if(Data-10 < 100)
						Painter.drawText(QPointF(i-9 - stp/2, 18),Str);
				}
			}
			Painter.setPen(ColorLevelView);
			Painter.setBrush(ColorLevelView);
			if(Num < -120)
				Num = 120;
			else if(Num > 0)
				Num = 0;
			Num += 120;
			Painter.drawRect(QRectF((SMETER_LEFT_POSITION + (width()-2*SMETER_LEFT_POSITION)*(Num/120.0)), 5, 1, 24));
		Painter.end();
	}
	else if(SMeterType == PWR_METER)
	{
		Num = PowerMeterVal;
		float stp = (width()-2*SMETER_LEFT_POSITION)/10.0;
		Font.setPointSize(9);
		Painter.begin(this);
			Painter.setFont(Font);
			Painter.setBrush(ColorLineLow);
			Painter.setPen(ColorLineLow);
			Painter.drawRect(QRectF(SMETER_LEFT_POSITION, 27, width()-2*SMETER_LEFT_POSITION-2, 2));
			int Data = 0;
			int j = 0;
			int tmpX = 0;
			for(float i = SMETER_LEFT_POSITION; i <= (SMETER_LEFT_POSITION+(width()-2*SMETER_LEFT_POSITION) - 3.5*stp); i += stp, j++)
			{
				Painter.setBrush(ColorLineLow);
				Painter.setPen(ColorLineLow);
				Painter.drawRect(QRectF(i, 27, 1, -7));
				if(width() <= 300 && j%2 == 0)
				{
					Data += 5;
					continue;
				}
				Painter.setBrush(ColorNumLow);
				Painter.setPen(ColorNumLow);
				Str.setNum(Data);
				if(Data < -99)
					Painter.drawText(QPointF(i-13, 18),Str);
				else if(Data < -9)
					Painter.drawText(QPointF(i-10, 18),Str);
				else if(Data < 10)
					Painter.drawText(QPointF(i-3, 18),Str);
				else if(Data < 100)
					Painter.drawText(QPointF(i-7, 18),Str);

				Data += 5;
				tmpX = i;
			}
			j = 0;
			Painter.setBrush(ColorLineHigh);
			Painter.setPen(ColorLineHigh);
			float ftmp = (SMETER_LEFT_POSITION + (width()-2*SMETER_LEFT_POSITION)*(35.0/50.0));
			Painter.drawRect(QRectF(ftmp, 27, width()-ftmp, 2));
			for(float i = (SMETER_LEFT_POSITION+(width()-2*SMETER_LEFT_POSITION - 3*stp))-1; i <= width(); i += stp, j++)
			{
				Painter.setBrush(ColorLineHigh);
				Painter.setPen(ColorLineHigh);
				Painter.drawRect(QRectF(i+1, 27, 1, -7));
				if(width() <= 300 && j%2 == 0)
				{
					Data += 5;
					continue;
				}

				Painter.setBrush(ColorNumHigh);
				Painter.setPen(ColorNumHigh);
				Str.setNum(Data);
				if(Data < -99)
					Painter.drawText(QPointF(i-13, 18),Str);
				else if(Data < -9)
					Painter.drawText(QPointF(i-10, 18),Str);
				else if(Data < 10)
					Painter.drawText(QPointF(i-3, 18),Str);
				else if(Data < 100)
					Painter.drawText(QPointF(i-7, 18),Str);

				Data += 5;
			}
			Painter.setPen(ColorLevelView);
			Painter.setBrush(ColorLevelView);
			if(Num < 0)
				Num = 0;
			else if(Num > 50)
				Num = 50;

			Painter.drawRect(QRectF((SMETER_LEFT_POSITION + (width()-2*SMETER_LEFT_POSITION)*(Num/50.0)), 5, 1, 24));
		Painter.end();
	}
	else if(SMeterType == SWR_METER)
	{
		Num = SwrMeterVal;
		if(Num < 1)
			Num = 1;
		else if(Num > 11)
			Num = 11;
		QPen pen, pen2;
		pen.setStyle(Qt::SolidLine);
		pen.setWidth(2);
		pen.setBrush(ColorLineLow);
		pen2 = pen;
		pen2.setBrush(ColorLineHigh);

		Font.setPointSize(9);
		QFontMetrics fontM(Font);
		Painter.begin(this);
			Painter.setBrush(ColorLineLow);
			Painter.setPen(Qt::NoPen);
			Painter.setFont(Font);
			Painter.drawRect(QRectF(SMETER_LEFT_POSITION, 27, width()/2.0, 2));
			float Data = 0.5;
			QString strData;
			float Step = (width()/2.0 - SMETER_LEFT_POSITION)/4.0;
			float tmpX;
			for(float begin = SMETER_LEFT_POSITION; begin <= width(); begin += Step)
			{
				if(begin <= width()/2.0)
				{
					Data += 0.5;
					Painter.setPen(pen);
					Painter.drawLine(begin, 27, begin, 20);
					strData = QString::number((double)Data);
					int FontPixW = fontM.width(strData);
					Painter.setBrush(Qt::NoBrush);
					Painter.setPen(ColorNumLow);
					Painter.drawText(begin - FontPixW/2.0, 18, strData);
					tmpX = begin;
				}
				else
				{
					Painter.setPen(pen2);
					float k = (width()/2.0 - SMETER_LEFT_POSITION)/2.0;
					float zero = SMETER_LEFT_POSITION + k*2 - 1;
					Painter.drawLine(zero + k*(5 - 3)/4.0, 27, begin, 20);
					strData = QString::number((double)Data);
					int FontPixW = fontM.width("5");
					Painter.setPen(ColorNumHigh);
					Painter.drawText(zero + k*(5 - 3)/4.0 - FontPixW/2.0, 18, "5");
					Painter.drawLine(zero + k*(10 - 3)/4.0, 27, zero + k*(10 - 3)/4.0, 20);
					strData = QString::number((double)Data);
					FontPixW = fontM.width("10");
					Painter.setPen(ColorNumHigh);
					Painter.drawText(zero + k*(10 - 3)/4.0 - FontPixW/2.0, 18, "10");
					break;
				}
			}
			Painter.setBrush(ColorLineHigh);
			Painter.setPen(Qt::NoPen);
			Painter.drawRect(QRectF(width()/2.0, 27, width()/2.0 + 2, 2));
			Painter.setPen(ColorLevelView);
			Painter.setBrush(ColorLevelView);
			if(Num <= 3)
			{
				float k = (width()/2.0 - SMETER_LEFT_POSITION)/2.0;
				Painter.drawRect(SMETER_LEFT_POSITION + k*(Num-1) -1, 5, 1, 24);
			}
			else
			{
				float k = (width()/2.0 - SMETER_LEFT_POSITION)/2.0;
				float zero = SMETER_LEFT_POSITION + k*2 - 1;
				Painter.drawRect(zero + k*(Num - 3)/4.0, 5, 1, 24);
			}
		Painter.end();
	}
}

void SMeterScale::SetSMeterVal(double Value)
{
	SMeterVal = Value;
	update();
}
void SMeterScale::SetLevelMeterVal(double Value)
{
	LevelMeterVal = Value;
	update();
}
void SMeterScale::SetMicMeterVal(double Value)
{
	MicMeterVal = Value;
	update();
}
void SMeterScale::SetPowerMeterVal(double Value)
{
	PowerMeterVal = Value;
	update();
}
void SMeterScale::SetSwrMeterVal(double Value)
{
	SwrMeterVal = Value;
	update();
}

void SMeterScale::resizeEvent(QResizeEvent *event)
{
	update();
}
void SMeterScale::SetNumMax(double Value)
{
	if(Value < -140)
		Num2 = -140;
	else if(Value > 20)
		Num2 = 20;
	else
		Num2 = Value;

	Num2 += 140;
	update();
}
void SMeterScale::SetColorNumLow(QColor color)
{
	ColorNumLow = color;
	update();
}
void SMeterScale::SetColorNumHigh(QColor color)
{
	ColorNumHigh = color;
	update();
}
void SMeterScale::SetColorLineLow(QColor color)
{
	ColorLineLow = color;
	update();
}
void SMeterScale::SetColorLineHigh(QColor color)
{
	ColorLineHigh = color;
	update();
}
void SMeterScale::SetColorLineSketch(QColor color)
{
	ColorLineSketch = color;
	update();
}
void SMeterScale::SetColorLineBackLow(QColor color)
{
	ColorLineBackLow = color;
	update();
}
void SMeterScale::SetColorLineBackHigh(QColor color)
{
	ColorLineBackHigh = color;
	update();
}
void SMeterScale::SetColorLevelView(QColor color)
{
	ColorLevelView = color;
	update();
}
void SMeterScale::SetColorMaxLevelView(QColor color)
{
	ColorMaxLevelView = color;
	update();
}
void SMeterScale::SetTypeSMeter(int type)
{
	SMeterType = (SMETER_TYPE)type;
	update();
}



