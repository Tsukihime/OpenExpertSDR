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

#ifndef OPENEXPERTSDR_MODEOPTIONS_H
#define OPENEXPERTSDR_MODEOPTIONS_H

#include <QSettings>

class ModeOptions {
private:
    int cbPreampIndex;
    int cbAgcIndex;
    bool BinEnable;
    bool NrEnable;
    bool AnfEnable;
    bool Nb1Enable;
    bool Nb2Enable;

    bool SqlEnable;
    int SqlValue;
    bool GateEnable;
    int GateValue;
    bool CompEnable;
    int CompValue;
    bool CpdrEnable;
    int CpdrValue;
    int FilterIndex;

    int VarFilterH;
    int VarFilterL;
public:
    ModeOptions();

    void readSettings(QSettings &settings);
    void writeSettings(QSettings &settings);

    int getCbPreampIndex() const;
    void setCbPreampIndex(int cbPreampIndex);
    int getCbAgcIndex() const;
    void setCbAgcIndex(int cbAgcIndex);
    bool isBinEnable() const;
    void setBinEnable(bool BinEnable);
    bool isNrEnable() const;
    void setNrEnable(bool NrEnable);
    bool isAnfEnable() const;
    void setAnfEnable(bool AnfEnable);
    bool isNb1Enable() const;
    void setNb1Enable(bool Nb1Enable);
    bool isNb2Enable() const;
    void setNb2Enable(bool Nb2Enable);
    bool isSqlEnable() const;
    void setSqlEnable(bool SqlEnable);
    int getSqlValue() const;
    void setSqlValue(int SqlValue);
    bool isGateEnable() const;
    void setGateEnable(bool GateEnable);
    int getGateValue() const;
    void setGateValue(int GateValue);
    bool isCompEnable() const;
    void setCompEnable(bool CompEnable);
    int getCompValue() const;
    void setCompValue(int CompValue);
    bool isCpdrEnable() const;
    void setCpdrEnable(bool CpdrEnable);
    int getCpdrValue() const;
    void setCpdrValue(int CpdrValue);
    int getFilterIndex() const;
    void setFilterIndex(int FilterIndex);
    int getVarFilterH() const;
    void setVarFilterH(int VarFilterH);
    int getVarFilterL() const;
    void setVarFilterL(int VarFilterL);
};


#endif //OPENEXPERTSDR_MODEOPTIONS_H
