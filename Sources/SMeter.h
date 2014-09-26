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

#ifndef SMETER_H
#define SMETER_H

#include <QtGui>
#include <QtGui/QWidget>
#include <QPainter>
#include <QFont>
#include "tmp/ui_SMeter.h"

#define SMETER_LEFT_POSITION 10
#define BUFFLEN 3

class SMeterNum;
class SMeterScale;
class QAction;
class QActionGroup;
class QMenu;

typedef enum
{
	SMETER = 0,
	LEVEL_METER,
	MIC_METER,
	PWR_METER,
	SWR_METER
}SMETER_TYPE, *pSMETER_TYPE;

class SMeter : public QWidget
{
    Q_OBJECT

public:
    typedef enum
    {
    	RX_SMETER = 0,
    	RX_LEVEL_METER
    }METER_RX_TYPE;

    typedef enum
    {
    	TX_MIC_METER = 0,
    	TX_PWR_METER,
    	TX_SWR_METER
    }METER_TX_TYPE;

private:
    SMeterNum *pSMeterNum;
    SMeterScale *pSMeterScale;

    int SMeterType;

    double Buff[BUFFLEN];

    double BuffSMeterVal[BUFFLEN];
    double BuffLevelMeterVal[BUFFLEN];
    double BuffMicMeterVal[BUFFLEN];
    double BuffPowerMeterVal[BUFFLEN];
    double BuffSwrMeterVal[BUFFLEN];

    int iS, iL, iM, iP, iSWR;
    double X;

    QColor ColorMainNum;
    QColor ColorNumLow;
    QColor ColorNumHigh;
    QColor ColorLineLow;
    QColor ColorLineHigh;
    QColor ColorLineSketch;
    QColor ColorLineBackLow;
    QColor ColorLineBackHigh;
    QColor ColorLevelView;
    QColor ColorMaxLevelView;


    QActionGroup *pRxActGroup;
    QAction *pRxSmeter;
    QAction *pRxLevelMeter;

    QActionGroup *pTxActGroup;
    QAction *pTxMicMeter;
    QAction *pTxPwrMeter;
    QAction *pTxSwrMeter;

    bool Tx;
    METER_RX_TYPE RxType;
    METER_TX_TYPE TxType;

public:
    SMeter(QWidget *parent = 0);
    ~SMeter();

    Q_PROPERTY(QColor ColorNumLow READ GetColorNumLow WRITE SetColorNumLow)
    Q_PROPERTY(QColor ColorNumHigh READ GetColorNumHigh WRITE SetColorNumHigh)
    Q_PROPERTY(QColor ColorLineLow READ GetColorLineLow WRITE SetColorLineLow)
    Q_PROPERTY(QColor ColorLineHigh READ GetColorLineHigh WRITE SetColorLineHigh)
    Q_PROPERTY(QColor ColorLineSketch READ GetColorLineSketch WRITE SetColorLineSketch)
    Q_PROPERTY(QColor ColorMainNum READ GetColorMainNum WRITE SetColorMainNum)
    Q_PROPERTY(QColor ColorLineBackLow READ GetColorLineBackLow WRITE SetColorLineBackLow)
    Q_PROPERTY(QColor ColorLineBackHigh READ GetColorLineBackHigh WRITE SetColorLineBackHigh)
    Q_PROPERTY(QColor ColorLevelView READ GetColorLevelView WRITE SetColorLevelView)
    Q_PROPERTY(QColor ColorMaxLevelView READ GetColorMaxLevelView WRITE SetColorMaxLevelView)
    Q_PROPERTY(int SMeterType READ GetTypeSMeter WRITE SetTypeSMeter)

    void SetSMeterVal(double Value);
    void SetLevelMeterVal(double Value);
    void SetMicMeterVal(double Value);
    void SetPowerMeterVal(double Value);
    void SetSwrMeterVal(double Value);

    void SetTypeSMeter(int type);
    int GetTypeSMeter();

    void SetColorMainNum(QColor color);
    void SetColorNumLow(QColor color);
    void SetColorNumHigh(QColor color);
    void SetColorLineLow(QColor color);
    void SetColorLineHigh(QColor color);
    void SetColorLineSketch(QColor color);
    void SetColorLineBackLow(QColor color);
    void SetColorLineBackHigh(QColor color);
    void SetColorLevelView(QColor color);
    void SetColorMaxLevelView(QColor color);
    QColor GetColorMainNum();
    QColor GetColorNumLow();
    QColor GetColorNumHigh();
    QColor GetColorLineLow();
    QColor GetColorLineHigh();
    QColor GetColorLineSketch();
    QColor GetColorLineBackLow();
    QColor GetColorLineBackHigh();
    QColor GetColorLevelView();
    QColor GetColorMaxLevelView();

    void SetTx(bool state);
    void SetTxType(int type);
    void SetRxType(int type);
    int GetTxType();
    int GetRxType();

private:
    Ui::SMeterClass ui;
    void resizeEvent(QResizeEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

    inline double Max();

    inline double MaxSMeter();
    inline double MaxLevel();
    inline double MaxMic();
    inline double MaxPower();
    inline double MaxSWR();

private slots:
	void GetRxType(QAction * action);
	void GetTxType(QAction * action);
};

class SMeterNum : public QWidget
{
    Q_OBJECT

    double Num;
    QFont Font;
    QPainter Painter;
    QString Str;

    QColor ColorNum;

public:
    SMeterNum(QWidget *parent = 0);

    void SetNum(double Value);
    void SetColorNum(QColor color);

private:
    void paintEvent(QPaintEvent *event);
};

class SMeterScale : public QWidget
{
    Q_OBJECT

    double Num, Num2, Val;
    QFont Font;
    QPainter Painter;
    QString Str;
    SMETER_TYPE SMeterType;

    double SMeterVal;
    double LevelMeterVal;
    double MicMeterVal;
    double PowerMeterVal;
    double SwrMeterVal;


    QColor ColorNumLow;
    QColor ColorNumHigh;
    QColor ColorLineLow;
    QColor ColorLineHigh;
    QColor ColorLineSketch;
    QColor ColorLineBackLow;
    QColor ColorLineBackHigh;
    QColor ColorLevelView;
    QColor ColorMaxLevelView;

public:
    SMeterScale(QWidget *parent = 0);

    void SetSMeterVal(double Value);
    void SetLevelMeterVal(double Value);
    void SetMicMeterVal(double Value);
    void SetPowerMeterVal(double Value);
    void SetSwrMeterVal(double Value);

    void SetNumMax(double Value);
    void SetTypeSMeter(int type);

    void SetColorNumLow(QColor color);
    void SetColorNumHigh(QColor color);
    void SetColorLineLow(QColor color);
    void SetColorLineHigh(QColor color);
    void SetColorLineSketch(QColor color);
    void SetColorLineBackLow(QColor color);
    void SetColorLineBackHigh(QColor color);
    void SetColorLevelView(QColor color);
    void SetColorMaxLevelView(QColor color);

private:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
};

#endif // SMETER_H
