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

#include "BandManager.h"
#include "qmath.h"
#include <QDebug>

struct BandLimit {
    int start;
    int end;
    BAND_MODE band;
};

const int bandLimitsCount = NUM_BANDS - 1; // exclude BANDGEN
const BandLimit bandLimits[bandLimitsCount] =
        {
                {BPF160_START, BPF160_END, BAND160M},
                {BPF80_START,  BPF80_END,  BAND80M},
                {BPF60_START,  BPF60_END,  BAND60M},
                {BPF40_START,  BPF40_END,  BAND40M},
                {BPF30_START,  BPF30_END,  BAND30M},
                {BPF20_START,  BPF20_END,  BAND20M},
                {BPF17_START,  BPF17_END,  BAND17M},
                {BPF15_START,  BPF15_END,  BAND15M},
                {BPF12_START,  BPF12_END,  BAND12M},
                {BPF10_START,  BPF10_END,  BAND10M},
                {BPF6_START,   BPF6_END,   BAND6M},
                {BPF2_START,   BPF2_END,   BAND2M},
                {BPF07_START,  BPF07_END,  BAND07M}
        };

double BandManager::DbToVal(double Db) {
    return (qPow(10.0, 0.1 * (48 - Db)));
}

BandManager::BandManager() {
    currentBand = BAND160M;
    for (int iBand = 0; iBand < NUM_BANDS; iBand++) {
        optBands[iBand].CurrentMode = LSB;
        optBands[iBand].Pitch = 750;
        optBands[iBand].Volume = 100;
        optBands[iBand].RfGain = 80;
        optBands[iBand].PowerCorrection = 0.1;
    }

    optBands[BAND160M].Frequency = BPF160_START;
    optBands[BAND80M].Frequency = BPF80_START;
    optBands[BAND60M].Frequency = BPF60_START;
    optBands[BAND40M].Frequency = BPF40_START;
    optBands[BAND30M].Frequency = BPF30_START;
    optBands[BAND20M].Frequency = BPF20_START;
    optBands[BAND17M].Frequency = BPF17_START;
    optBands[BAND15M].Frequency = BPF15_START;
    optBands[BAND12M].Frequency = BPF12_START;
    optBands[BAND10M].Frequency = BPF10_START;
    optBands[BAND6M].Frequency = BPF6_START;
    optBands[BAND2M].Frequency = BPF2_START;
    optBands[BAND07M].Frequency = BPF07_START;
    optBands[BANDGEN].Frequency = BPF160_START;
}

void BandManager::writeSettings(QSettings &settings) {
    QString Str;
    settings.beginGroup("BandOptions");
    settings.setValue("CurrentBand", getCurrentBand());
    for (int iBand = 0; iBand < NUM_BANDS; iBand++) {
        Str.setNum(iBand);
        settings.beginGroup("Band" + Str);
        settings.setValue("CurrentMode", optBands[iBand].CurrentMode);
        settings.setValue("BandFreq", optBands[iBand].Frequency);
        settings.setValue("Pitch", optBands[iBand].Pitch);
        settings.setValue("slVolume", optBands[iBand].Volume);
        settings.setValue("slAgc", optBands[iBand].RfGain);
        for (int iMode = 0; iMode < NUM_MODES; iMode++) {
            Str.setNum(iMode);
            settings.beginGroup("Mode" + Str);
            optBands[iBand].Mode[iMode].readSettings(settings);
            settings.endGroup();
        }
        settings.endGroup();
    }
    settings.endGroup();
}

void BandManager::readSettings(QSettings &settings) {
    int tmpIValue;
    settings.beginGroup("BandOptions");
    tmpIValue = settings.value("CurrentBand", BAND160M).toInt();
    if (tmpIValue < 0 || tmpIValue > NUM_BANDS - 1) {
        qWarning() << "ExpertSDR: readSettings(): CurrentBand = " << tmpIValue;
        tmpIValue = 0;
    }
    currentBand = (BAND_MODE) tmpIValue;

    for (int iBand = 0; iBand < NUM_BANDS; iBand++) {

        BandLimit bandLimit = {BPF160_START, BPF07_END, BANDGEN};
        for (int i = 0; i < bandLimitsCount; i++) {
            if (bandLimits[i].band == iBand) {
                bandLimit = bandLimits[i];
                break;
            }
        }

        settings.beginGroup(QString("Band%1").arg(iBand));
        tmpIValue = settings.value("CurrentMode", LSB).toInt();
        if (tmpIValue < 0 || tmpIValue > NUM_MODES - 1) {
            qWarning() << "ExpertSDR: readSettings(): Band" << iBand << ": CurrentMode = " << tmpIValue;
            tmpIValue = 0;
        }
        optBands[iBand].CurrentMode = (SDRMODE) tmpIValue;

        tmpIValue = settings.value("BandFreq", bandLimit.start).toInt();

        if (tmpIValue < bandLimit.start || tmpIValue > bandLimit.end) {
            qWarning() << "ExpertSDR: readSettings(): Band " << bandLimit.band << ": BandFreq = " <<
            tmpIValue;
            tmpIValue = bandLimit.start;
        }
        optBands[iBand].Frequency = tmpIValue;

        tmpIValue = settings.value("Pitch", 750).toInt();
        if (tmpIValue < 50 || tmpIValue > 2500) {
            qWarning() << "ExpertSDR: readSettings(): Pitch = " << tmpIValue;
            tmpIValue = 750;
        }
        optBands[iBand].Pitch = tmpIValue;

        tmpIValue = settings.value("slVolume", 70).toInt();
        if (tmpIValue < 0 || tmpIValue > 100) {
            qWarning() << "ExpertSDR: readSettings(): slVolume = " << tmpIValue;
            tmpIValue = 70;
        }
        optBands[iBand].Volume = tmpIValue;

        tmpIValue = settings.value("slAgc", 80).toInt();
        if (tmpIValue < -20 || tmpIValue > 120) {
            qWarning() << "ExpertSDR: readSettings(): slAgc = " << tmpIValue;
            tmpIValue = 80;
        }
        optBands[iBand].RfGain = tmpIValue;

        for (int iMode = 0; iMode < NUM_MODES; iMode++) {
            settings.beginGroup(QString("Mode%1").arg(iMode));
            optBands[iBand].Mode[iMode].readSettings(settings);
            settings.endGroup();
        }
        settings.endGroup();
    }
    settings.endGroup();
}

BAND_MODE BandManager::getBandByFrequency(int Frequency) {
    for (int i = 0; i < bandLimitsCount - 1; i++) {
        if ((Frequency > bandLimits[i].start) && (Frequency <= bandLimits[i].end)) {
            return bandLimits[i].band;
        }
    }
    return BANDGEN;
}

void BandManager::setFrequency(int Frequency) {
    BAND_MODE band = getBandByFrequency(Frequency);

    if (currentBand != band) {
        optBands[band].Frequency = Frequency;
        setCurrentBand(band);
    }

    optBands[currentBand].Frequency = Frequency;
}

BAND_MODE BandManager::getCurrentBand() {
    return currentBand;
}

void BandManager::setCurrentBand(BAND_MODE band) {
    if (currentBand != band) {
        if (band < BAND160M || band > BANDGEN) {
            band = BAND160M;
        }
        currentBand = band;
        emit BandChanged(band);
    }
}

SDRMODE BandManager::getCurrentBandMode() {
    return optBands[currentBand].CurrentMode;
}

ModeOptions &BandManager::getCurrentBandModOptions() {
    return optBands[currentBand].Mode[getCurrentBandMode()];
}

void BandManager::setCurrentBandMode(SDRMODE Mode) {
    optBands[currentBand].CurrentMode = Mode;
}

int BandManager::getCurrentBandRfGain() {
    return optBands[currentBand].RfGain;
}

void BandManager::setCurrentBandRfGain(int value) {
    optBands[currentBand].RfGain = value;
}

int BandManager::getCurrentBandPitch() {
    return optBands[currentBand].Pitch;
}

void BandManager::setCurrentBandPitch(int value) {
    optBands[currentBand].Pitch = value;
}

int BandManager::getCurrentBandVolume() {
    return optBands[currentBand].Volume;
}

void BandManager::setCurrentBandVolume(int value) {
    optBands[currentBand].Volume = value;
}

int BandManager::getCurrentBandFrequency() {
    return optBands[currentBand].Frequency;
}

double BandManager::getCurrentBandPowerCorrection() {
    return optBands[currentBand].PowerCorrection;
}

void BandManager::OnPowerCorrect160(int Val) {
    optBands[BAND160M].PowerCorrection = DbToVal(Val);
}

void BandManager::OnPowerCorrect80(int Val) {
    optBands[BAND80M].PowerCorrection = DbToVal(Val);
}

void BandManager::OnPowerCorrect60(int Val) {
    optBands[BAND60M].PowerCorrection = DbToVal(Val);
}

void BandManager::OnPowerCorrect40(int Val) {
    optBands[BAND40M].PowerCorrection = DbToVal(Val);
}

void BandManager::OnPowerCorrect30(int Val) {
    optBands[BAND30M].PowerCorrection = DbToVal(Val);
}

void BandManager::OnPowerCorrect20(int Val) {
    optBands[BAND20M].PowerCorrection = DbToVal(Val);
}

void BandManager::OnPowerCorrect17(int Val) {
    optBands[BAND17M].PowerCorrection = DbToVal(Val);
}

void BandManager::OnPowerCorrect15(int Val) {
    optBands[BAND15M].PowerCorrection = DbToVal(Val);
}

void BandManager::OnPowerCorrect12(int Val) {
    optBands[BAND12M].PowerCorrection = DbToVal(Val);
}

void BandManager::OnPowerCorrect10(int Val) {
    optBands[BAND10M].PowerCorrection = DbToVal(Val);
}

void BandManager::OnPowerCorrect6(int Val) {
    optBands[BAND6M].PowerCorrection = DbToVal(Val);
}

void BandManager::OnPowerCorrect2(int Val) {
    optBands[BAND2M].PowerCorrection = DbToVal(Val);
}

void BandManager::OnPowerCorrect07(int Val) {
    optBands[BAND07M].PowerCorrection = DbToVal(Val);
}
