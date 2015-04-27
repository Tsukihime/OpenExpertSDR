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

#include "ModeOptions.h"
#include <QDebug>

ModeOptions::ModeOptions() {
    cbPreampIndex = 0;
    cbAgcIndex = 0;
    BinEnable = false;
    NrEnable = false;
    AnfEnable = false;
    Nb1Enable = false;
    Nb2Enable = false;
    SqlEnable = false;
    SqlValue = 0;
    GateEnable = false;
    GateValue = 0;
    CompEnable = false;
    CompValue = 0;
    CpdrEnable = false;
    CpdrValue = 0;
    FilterIndex = 4;
}

void ModeOptions::readSettings(QSettings &settings) {
    int tmpIValue;
    tmpIValue = settings.value("PreampIndex", 0).toInt();
    if (tmpIValue < 0 || tmpIValue > 4) {
        qWarning() << "ModeOptions: readSettings(): PreampIndex = " << tmpIValue;
        tmpIValue = 0;
    }
    cbPreampIndex = tmpIValue;

    tmpIValue = settings.value("AgcIndex", 1).toInt();
    if (tmpIValue < 0 || tmpIValue > 4) {
        qWarning() << "ModeOptions: readSettings(): AgcIndex = " << tmpIValue;
        tmpIValue = 1;
    }
    cbAgcIndex = tmpIValue;
    BinEnable = settings.value("BinEnable", false).toBool();
    NrEnable = settings.value("NrEnable", false).toBool();
    AnfEnable = settings.value("AnfEnable", false).toBool();
    Nb1Enable = settings.value("Nb1Enable", false).toBool();
    Nb2Enable = settings.value("Nb2Enable", false).toBool();
    SqlEnable = settings.value("SqlEnable", false).toBool();
    tmpIValue = settings.value("SqlValue", 0).toInt();
    if (tmpIValue < -165 || tmpIValue > 0) {
        qWarning() << "ModeOptions: readSettings(): SqlValue = " << tmpIValue;
        tmpIValue = 0;
    }
    SqlValue = tmpIValue;
    GateEnable = settings.value("GateEnable", false).toBool();
    tmpIValue = settings.value("GateValue", 0).toInt();
    if (tmpIValue < -165 || tmpIValue > 0) {
        qWarning() << "ModeOptions: readSettings(): GateValue = " << tmpIValue;
        tmpIValue = 0;
    }
    GateValue = tmpIValue;
    CompEnable = settings.value("CompEnable", false).toBool();
    tmpIValue = settings.value("CompValue", 0).toInt();
    if (tmpIValue < 0 || tmpIValue > 20) {
        qWarning() << "ModeOptions: readSettings(): CompValue = " << tmpIValue;
        tmpIValue = 0;
    }
    CompValue = tmpIValue;
    CpdrEnable = settings.value("CpdrEnable", false).toBool();
    tmpIValue = settings.value("CpdrValue", 0).toInt();
    if (tmpIValue < 0 || tmpIValue > 10) {
        qWarning() << "ModeOptions: readSettings(): CpdrValue = " << tmpIValue;
        tmpIValue = 0;
    }
    CpdrValue = tmpIValue;
    tmpIValue = settings.value("FilterIndex", 4).toInt();
    if (tmpIValue < 0 || tmpIValue > 8) {
        qWarning() << "ModeOptions: readSettings(): FilterIndex = " << tmpIValue;
        tmpIValue = 4;
    }
    FilterIndex = tmpIValue;
    tmpIValue = settings.value("VarFilterH", 1000).toInt();
    if (tmpIValue < -20000 || tmpIValue > 20000) {
        qWarning() << "ModeOptions: readSettings(): VarFilterH = " << tmpIValue;
        tmpIValue = 1000;
    }
    VarFilterH = tmpIValue;
    tmpIValue = settings.value("VarFilterL", -1000).toInt();
    if (tmpIValue < -20000 || tmpIValue > 20000) {
        qWarning() << "ModeOptions: readSettings(): VarFilterL = " << tmpIValue;
        tmpIValue = 1000;
    }
    VarFilterL = tmpIValue;
}

void ModeOptions::writeSettings(QSettings &settings) {
    settings.setValue("PreampIndex", cbPreampIndex);
    settings.setValue("AgcIndex", cbAgcIndex);
    settings.setValue("BinEnable", BinEnable);
    settings.setValue("NrEnable", NrEnable);
    settings.setValue("AnfEnable", AnfEnable);
    settings.setValue("Nb1Enable", Nb1Enable);
    settings.setValue("Nb2Enable", Nb2Enable);
    settings.setValue("SqlEnable", SqlEnable);
    settings.setValue("SqlValue", SqlValue);
    settings.setValue("GateEnable", GateEnable);
    settings.setValue("GateValue", GateValue);
    settings.setValue("CompEnable", CompEnable);
    settings.setValue("CompValue", CompValue);
    settings.setValue("CpdrEnable", CpdrEnable);
    settings.setValue("CpdrValue", CpdrValue);
    settings.setValue("FilterIndex", FilterIndex);
    settings.setValue("VarFilterH", VarFilterH);
    settings.setValue("VarFilterL", VarFilterL);
}

int ModeOptions::getCbPreampIndex() const {
    return cbPreampIndex;
}

void ModeOptions::setCbPreampIndex(int cbPreampIndex) {
    this->cbPreampIndex = cbPreampIndex;
}

int ModeOptions::getCbAgcIndex() const {
    return cbAgcIndex;
}

void ModeOptions::setCbAgcIndex(int cbAgcIndex) {
    this->cbAgcIndex = cbAgcIndex;
}

bool ModeOptions::isBinEnable() const {
    return BinEnable;
}

void ModeOptions::setBinEnable(bool BinEnable) {
    this->BinEnable = BinEnable;
}

bool ModeOptions::isNrEnable() const {
    return NrEnable;
}

void ModeOptions::setNrEnable(bool NrEnable) {
    this->NrEnable = NrEnable;
}

bool ModeOptions::isAnfEnable() const {
    return AnfEnable;
}

void ModeOptions::setAnfEnable(bool AnfEnable) {
    this->AnfEnable = AnfEnable;
}

bool ModeOptions::isNb1Enable() const {
    return Nb1Enable;
}

void ModeOptions::setNb1Enable(bool Nb1Enable) {
    this->Nb1Enable = Nb1Enable;
}

bool ModeOptions::isNb2Enable() const {
    return Nb2Enable;
}

void ModeOptions::setNb2Enable(bool Nb2Enable) {
    this->Nb2Enable = Nb2Enable;
}

bool ModeOptions::isSqlEnable() const {
    return SqlEnable;
}

void ModeOptions::setSqlEnable(bool SqlEnable) {
    this->SqlEnable = SqlEnable;
}

int ModeOptions::getSqlValue() const {
    return SqlValue;
}

void ModeOptions::setSqlValue(int SqlValue) {
    this->SqlValue = SqlValue;
}

bool ModeOptions::isGateEnable() const {
    return GateEnable;
}

void ModeOptions::setGateEnable(bool GateEnable) {
    this->GateEnable = GateEnable;
}

int ModeOptions::getGateValue() const {
    return GateValue;
}

void ModeOptions::setGateValue(int GateValue) {
    this->GateValue = GateValue;
}

bool ModeOptions::isCompEnable() const {
    return CompEnable;
}

void ModeOptions::setCompEnable(bool CompEnable) {
    this->CompEnable = CompEnable;
}

int ModeOptions::getCompValue() const {
    return CompValue;
}

void ModeOptions::setCompValue(int CompValue) {
    this->CompValue = CompValue;
}

bool ModeOptions::isCpdrEnable() const {
    return CpdrEnable;
}

void ModeOptions::setCpdrEnable(bool CpdrEnable) {
    this->CpdrEnable = CpdrEnable;
}

int ModeOptions::getCpdrValue() const {
    return CpdrValue;
}

void ModeOptions::setCpdrValue(int CpdrValue) {
    this->CpdrValue = CpdrValue;
}

int ModeOptions::getFilterIndex() const {
    return FilterIndex;
}

void ModeOptions::setFilterIndex(int FilterIndex) {
    this->FilterIndex = FilterIndex;
}

int ModeOptions::getVarFilterH() const {
    return VarFilterH;
}

void ModeOptions::setVarFilterH(int VarFilterH) {
    this->VarFilterH = VarFilterH;
}

int ModeOptions::getVarFilterL() const {
    return VarFilterL;
}

void ModeOptions::setVarFilterL(int VarFilterL) {
    this->VarFilterL = VarFilterL;
}
