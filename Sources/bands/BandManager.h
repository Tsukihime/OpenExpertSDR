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
 * Copyright (C) 2015 Tsukihime
 */

#ifndef BANDMANAGER_H
#define BANDMANAGER_H

#include <QObject>
#include <QSettings>
#include "ModeOptions.h"

const int BPF160_START = 1800000;
const int BPF160_END = 2000000;

const int BPF80_START = 3500000;
const int BPF80_END = 3800000;

const int BPF60_START = 5100000;
const int BPF60_END = 5500000;

const int BPF40_START = 7000000;
const int BPF40_END = 7300000;

const int BPF30_START = 10100000;
const int BPF30_END = 10150000;

const int BPF20_START = 14000000;
const int BPF20_END = 14350000;

const int BPF17_START = 18068000;
const int BPF17_END = 18318000;

const int BPF15_START = 21000000;
const int BPF15_END = 21450000;

const int BPF12_START = 24890000;
const int BPF12_END = 25140000;

const int BPF10_START = 26970000;
const int BPF10_END = 29700000;

const int BPF6_START = 50000000;
const int BPF6_END = 54000000;

const int BPF2_START = 144000000;
const int BPF2_END = 146000000;

const int BPF07_START = 430000000;
const int BPF07_END = 440000000;

const int BPF160_F = 2500000;
const int BPF80_F = 4000000;
const int BPF60_F = 6000000;
const int BPF40_F = 7300000;
const int BPF30_F = 12000000;
const int BPF20_F = 14500000;
const int BPF17_F = 19000000;
const int BPF15_F = 21500000;
const int BPF12_F = 25200000;
const int BPF10_F = 30000000;
const int BPF6_F = 65000000;

const int NUM_BANDS = 14;
const int NUM_MODES = 12;

typedef enum {
    BAND160M,
    BAND80M,
    BAND60M,
    BAND40M,
    BAND30M,
    BAND20M,
    BAND17M,
    BAND15M,
    BAND12M,
    BAND10M,
    BAND6M,
    BAND2M,
    BAND07M,
    BANDGEN,
} BAND_MODE;

typedef enum {
    LSB,
    USB,
    DSB,
    CWL,
    CWU,
    FMN,
    AM,
    DIGU,
    SPEC,
    DIGL,
    SAM,
    DRM
} SDRMODE;

struct BandOptions {
    SDRMODE CurrentMode;
    int MainFreq;
    int Pitch;
    int Volume;
    int RfGain;
    double PowerCorrection;
    ModeOptions Mode[NUM_MODES];
};

class BandManager : public QObject {
Q_OBJECT
private:
    BandOptions optBands[NUM_BANDS];
    BAND_MODE currentBand;

    double DbToVal(double Db);

public:
    BandManager();

    void writeSettings(QSettings &settings);
    void readSettings(QSettings &settings);

    void setMainFreqExperimental(int Freq, int DdsFreq);

    BAND_MODE getCurrentBand();
    void setCurrentBand(BAND_MODE Mode);

    SDRMODE getCurrentBandMode();
    void setCurrentBandMode(SDRMODE Mode);

    ModeOptions &getCurrentBandModOptions();

    double getCurrentBandPowerCorrection();
    int getCurrentBandMainFrequency();
    void setCurrentBandMainFrequency(int Freq);
    int getCurrentBandVolume();
    void setCurrentBandVolume(int value);
    int getCurrentBandRfGain();
    void setCurrentBandRfGain(int value);
    int getCurrentBandPitch();
    void setCurrentBandPitch(int value);

public slots:
    void OnPowerCorrect160(int Val);
    void OnPowerCorrect80(int Val);
    void OnPowerCorrect60(int Val);
    void OnPowerCorrect40(int Val);
    void OnPowerCorrect30(int Val);
    void OnPowerCorrect20(int Val);
    void OnPowerCorrect17(int Val);
    void OnPowerCorrect15(int Val);
    void OnPowerCorrect12(int Val);
    void OnPowerCorrect10(int Val);
    void OnPowerCorrect6(int Val);
    void OnPowerCorrect2(int Val);
    void OnPowerCorrect07(int Val);

signals:
    void BandChanged(int);
};

#endif // BANDMANAGER_H
