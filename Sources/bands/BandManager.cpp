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

    optBands[BAND160M].MainFreq = BPF160_START;
    optBands[BAND80M].MainFreq = BPF80_START;
    optBands[BAND60M].MainFreq = BPF60_START;
    optBands[BAND40M].MainFreq = BPF40_START;
    optBands[BAND30M].MainFreq = BPF30_START;
    optBands[BAND20M].MainFreq = BPF20_START;
    optBands[BAND17M].MainFreq = BPF17_START;
    optBands[BAND15M].MainFreq = BPF15_START;
    optBands[BAND12M].MainFreq = BPF12_START;
    optBands[BAND10M].MainFreq = BPF10_START;
    optBands[BAND6M].MainFreq = BPF6_START;
    optBands[BAND2M].MainFreq = 144000000;
    optBands[BAND07M].MainFreq = 433000000;
    optBands[BANDGEN].MainFreq = 1800000;
}

void BandManager::writeSettings(QSettings &settings) {
    QString Str;
    settings.beginGroup("BandOptions");
    settings.setValue("CurrentBand", getCurrentBand());
    for (int iBand = 0; iBand < NUM_BANDS; iBand++) {
        Str.setNum(iBand);
        settings.beginGroup("Band" + Str);
        settings.setValue("CurrentMode", optBands[iBand].CurrentMode);
        settings.setValue("BandFreq", optBands[iBand].MainFreq);
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
    if (tmpIValue < 0 || tmpIValue > 13) {
        qWarning() << "ExpertSDR: readSettings(): CurrentBand = " << tmpIValue;
        tmpIValue = 0;
    }
    currentBand = (BAND_MODE) tmpIValue;
    for (int iBand = 0; iBand < NUM_BANDS; iBand++) {
        int TmpBandFreq = 0;
        if (iBand == BAND160M) TmpBandFreq = BPF160_START;
        if (iBand == BAND80M) TmpBandFreq = BPF80_START;
        if (iBand == BAND60M) TmpBandFreq = BPF60_START;
        if (iBand == BAND40M) TmpBandFreq = BPF40_START;
        if (iBand == BAND30M) TmpBandFreq = BPF30_START;
        if (iBand == BAND20M) TmpBandFreq = BPF20_START;
        if (iBand == BAND17M) TmpBandFreq = BPF17_START;
        if (iBand == BAND15M) TmpBandFreq = BPF15_START;
        if (iBand == BAND12M) TmpBandFreq = BPF12_START;
        if (iBand == BAND10M) TmpBandFreq = BPF10_START;
        if (iBand == BAND6M) TmpBandFreq = BPF6_START;
        if (iBand == BAND2M) TmpBandFreq = BPF2_START;
        if (iBand == BAND07M) TmpBandFreq = BPF07_START;
        if (iBand == BANDGEN) TmpBandFreq = BPF160_START;

        QString Str;
        Str.setNum(iBand);
        settings.beginGroup("Band" + Str);
        tmpIValue = settings.value("CurrentMode", 0).toInt();
        if (tmpIValue < 0 || tmpIValue > 11) {
            qWarning() << "ExpertSDR: readSettings(): Band" + Str + ": CurrentMode = " << tmpIValue;
            tmpIValue = 0;
        }
        optBands[iBand].CurrentMode = (SDRMODE) tmpIValue;

        tmpIValue = settings.value("BandFreq", TmpBandFreq).toInt();
        switch ((BAND_MODE) iBand) {
            case BAND160M:
                if (tmpIValue < BPF160_START || tmpIValue > BPF160_END) {
                    qWarning() << "ExpertSDR: readSettings(): Band160: BandFreq = " << tmpIValue;
                    tmpIValue = BPF160_START;
                }
                break;
            case BAND80M:
                if (tmpIValue < BPF80_START || tmpIValue > BPF80_END) {
                    qWarning() << "ExpertSDR: readSettings(): Band80: BandFreq = " << tmpIValue;
                    tmpIValue = BPF80_START;
                }
                break;
            case BAND60M:
                if (tmpIValue < BPF60_START || tmpIValue > BPF60_END) {
                    qWarning() << "ExpertSDR: readSettings(): Band60: BandFreq = " << tmpIValue;
                    tmpIValue = BPF60_START;
                }
                break;
            case BAND40M:
                if (tmpIValue < BPF40_START || tmpIValue > BPF40_END) {
                    qWarning() << "ExpertSDR: readSettings(): Band40: BandFreq = " << tmpIValue;
                    tmpIValue = BPF40_START;
                }
                break;
            case BAND30M:
                if (tmpIValue < BPF30_START || tmpIValue > BPF30_END) {
                    qWarning() << "ExpertSDR: readSettings(): Band30: BandFreq = " << tmpIValue;
                    tmpIValue = BPF30_START;
                }
                break;
            case BAND20M:
                if (tmpIValue < BPF20_START || tmpIValue > BPF20_END) {
                    qWarning() << "ExpertSDR: readSettings(): Band20: BandFreq = " << tmpIValue;
                    tmpIValue = BPF20_START;
                }
                break;
            case BAND17M:
                if (tmpIValue < BPF17_START || tmpIValue > BPF17_END) {
                    qWarning() << "ExpertSDR: readSettings(): Band17: BandFreq = " << tmpIValue;
                    tmpIValue = BPF17_START;
                }
                break;
            case BAND15M:
                if (tmpIValue < BPF15_START || tmpIValue > BPF15_END) {
                    qWarning() << "ExpertSDR: readSettings(): Band15: BandFreq = " << tmpIValue;
                    tmpIValue = BPF15_START;
                }
                break;
            case BAND12M:
                if (tmpIValue < BPF12_START || tmpIValue > BPF12_END) {
                    qWarning() << "ExpertSDR: readSettings(): Band12: BandFreq = " << tmpIValue;
                    tmpIValue = BPF12_START;
                }
                break;
            case BAND10M:
                if (tmpIValue < BPF10_START || tmpIValue > BPF10_END) {
                    qWarning() << "ExpertSDR: readSettings(): Band10: BandFreq = " << tmpIValue;
                    tmpIValue = BPF10_START;
                }
                break;
            case BAND6M:
                if (tmpIValue < BPF6_START || tmpIValue > BPF6_END) {
                    qWarning() << "ExpertSDR: readSettings(): Band6: BandFreq = " << tmpIValue;
                    tmpIValue = BPF6_START;
                }
                break;
            case BAND2M:
                if (tmpIValue < BPF2_START || tmpIValue > BPF2_END) {
                    qWarning() << "ExpertSDR: readSettings(): Band2: BandFreq = " << tmpIValue;
                    tmpIValue = BPF2_START;
                }
                break;
            case BAND07M:
                if (tmpIValue < BPF07_START || tmpIValue > BPF07_END) {
                    qWarning() << "ExpertSDR: readSettings(): Band07: BandFreq = " << tmpIValue;
                    tmpIValue = BPF07_START;
                }
                break;
            case BANDGEN:
                if (tmpIValue < 1000000 || tmpIValue > 440000000) {
                    qWarning() << "ExpertSDR: readSettings(): Band60: BandFreq = " << tmpIValue;
                    tmpIValue = BPF160_START;
                }
                break;
            default:
                if (tmpIValue < 1000000 || tmpIValue > 440000000) {
                    qWarning() << "ExpertSDR: readSettings(): Band?: BandFreq = " << tmpIValue;
                    tmpIValue = BPF160_START;
                }
                break;
        }
        optBands[iBand].MainFreq = tmpIValue;
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
            Str.setNum(iMode);
            settings.beginGroup("Mode" + Str);
            optBands[iBand].Mode[iMode].readSettings(settings);
            settings.endGroup();
        }
        settings.endGroup();
    }
    settings.endGroup();
}

void BandManager::setMainFreqExperimental(int Freq, int DdsFreq) {
    // todo edit this shit!
    if (DdsFreq <= BPF160_F) {
        if ((Freq >= BPF160_START) && (Freq <= BPF160_END)) {
            if (currentBand != BAND160M) {
                optBands[BAND160M].MainFreq = Freq;
                setCurrentBand(BAND160M);
            }
        }
        else {
            if (currentBand != BANDGEN) {
                optBands[BANDGEN].MainFreq = Freq;
                setCurrentBand(BANDGEN);
            }
        }
    }
    else if ((DdsFreq > BPF160_F) && (DdsFreq <= BPF80_F)) {
        if ((Freq >= BPF80_START) && (Freq <= BPF80_END)) {
            if (currentBand != BAND80M) {
                optBands[BAND80M].MainFreq = Freq;
                setCurrentBand(BAND80M);
            }
        }
        else {
            if (currentBand != BANDGEN) {
                optBands[BANDGEN].MainFreq = Freq;
                setCurrentBand(BANDGEN);
            }
        }
    }
    else if ((DdsFreq > BPF80_F) && (DdsFreq <= BPF60_F)) {
        if ((Freq >= BPF60_START) && (Freq <= BPF60_END)) {
            if (currentBand != BAND60M) {
                optBands[BAND60M].MainFreq = Freq;
                setCurrentBand(BAND60M);
            };
        }
        else {
            if (currentBand != BANDGEN) {
                optBands[BANDGEN].MainFreq = Freq;
                setCurrentBand(BANDGEN);
            }
        }
    }
    else if ((DdsFreq > BPF60_F) && (DdsFreq <= BPF40_F)) {
        if ((Freq >= BPF40_START) && (Freq <= BPF40_END)) {
            if (currentBand != BAND40M) {
                optBands[BAND40M].MainFreq = Freq;
                setCurrentBand(BAND40M);
            }
        }
        else {
            if (currentBand != BANDGEN) {
                optBands[BANDGEN].MainFreq = Freq;
                setCurrentBand(BANDGEN);
            }
        }
    }
    else if ((DdsFreq > BPF40_F) && (DdsFreq <= BPF30_F)) {
        if ((Freq >= BPF30_START) && (Freq <= BPF30_END)) {
            if (currentBand != BAND30M) {
                optBands[BAND30M].MainFreq = Freq;
                setCurrentBand(BAND30M);
            }
        }
        else {
            if (currentBand != BANDGEN) {
                optBands[BANDGEN].MainFreq = Freq;
                setCurrentBand(BANDGEN);
            }
        }
    }
    else if ((DdsFreq > BPF30_F) && (DdsFreq <= BPF20_F)) {
        if ((Freq >= BPF20_START) && (Freq <= BPF20_END)) {
            if (currentBand != BAND20M) {
                optBands[BAND20M].MainFreq = Freq;
                setCurrentBand(BAND20M);
            }
        }
        else {
            if (currentBand != BANDGEN) {
                optBands[BANDGEN].MainFreq = Freq;
                setCurrentBand(BANDGEN);
            }
        }
    }
    else if ((DdsFreq > BPF20_F) && (DdsFreq <= BPF17_F)) {
        if ((Freq >= BPF17_START) && (Freq <= BPF17_END)) {
            if (currentBand != BAND17M) {
                optBands[BAND17M].MainFreq = Freq;
                setCurrentBand(BAND17M);
            }
        }
        else {
            if (currentBand != BANDGEN) {
                optBands[BANDGEN].MainFreq = Freq;
                setCurrentBand(BANDGEN);
            }
        }
    }
    else if ((DdsFreq > BPF17_F) && (DdsFreq <= BPF15_F)) {
        if ((Freq >= BPF15_START) && (Freq <= BPF15_END)) {
            if (currentBand != BAND15M) {
                optBands[BAND15M].MainFreq = Freq;
                setCurrentBand(BAND15M);
            }
        }
        else {
            if (currentBand != BANDGEN) {
                optBands[BANDGEN].MainFreq = Freq;
                setCurrentBand(BANDGEN);
            }
        }
    }
    else if ((DdsFreq > BPF15_F) && (DdsFreq <= BPF12_F)) {
        if ((Freq >= BPF12_START) && (Freq <= BPF12_END)) {
            if (currentBand != BAND12M) {
                optBands[BAND12M].MainFreq = Freq;
                setCurrentBand(BAND12M);
            }
        }
        else {
            if (currentBand != BANDGEN) {
                optBands[BANDGEN].MainFreq = Freq;
                setCurrentBand(BANDGEN);
            }
        }
    }
    else if ((DdsFreq > BPF12_F) && (DdsFreq <= BPF10_F)) {
        if ((Freq >= BPF10_START) && (Freq <= BPF10_END)) {
            if (currentBand != BAND10M) {
                optBands[BAND10M].MainFreq = Freq;
                setCurrentBand(BAND10M);
            }
        }
        else {
            if (currentBand != BANDGEN) {
                optBands[BANDGEN].MainFreq = Freq;
                setCurrentBand(BANDGEN);
            }
        }
    }
    else if ((DdsFreq > BPF10_F) && (DdsFreq <= BPF6_F)) {
        if ((Freq >= BPF6_START) && (Freq <= BPF6_END)) {
            if (currentBand != BAND6M) {
                optBands[BAND6M].MainFreq = Freq;
                setCurrentBand(BAND6M);
            }
        }
        else {
            if (currentBand != BANDGEN) {
                optBands[BANDGEN].MainFreq = Freq;
                setCurrentBand(BANDGEN);
            }
        }
    }
    else if ((Freq >= BPF2_START) && (Freq <= BPF2_END)) {
        if ((Freq >= BPF2_START) && (Freq <= BPF2_END)) {
            if (currentBand != BAND2M) {
                optBands[BAND2M].MainFreq = Freq;
                setCurrentBand(BAND2M);
            }
        }
        else {
            // todo: unrechable statement
            if (currentBand != BANDGEN) {
                optBands[BANDGEN].MainFreq = Freq;
                setCurrentBand(BANDGEN);
            }
        }
    }
    else {
        if ((Freq >= BPF07_START) && (Freq <= BPF07_END)) {
            if (currentBand != BAND07M) {
                optBands[BAND07M].MainFreq = Freq;
                setCurrentBand(BAND07M);
            }
        }
        else {
            if (currentBand != BANDGEN) {
                optBands[BANDGEN].MainFreq = Freq;
                setCurrentBand(BANDGEN);
            }
        }
    }
    optBands[currentBand].MainFreq = Freq;
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

int BandManager::getCurrentBandMainFrequency() {
    return optBands[currentBand].MainFreq;
}

void BandManager::setCurrentBandMainFrequency(int Freq) {
    optBands[currentBand].MainFreq = Freq;
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
