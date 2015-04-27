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
 *
 * edited by Tsukihime
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#include <QString>

const int MAX_DSP_BUFFER_SIZE = 4096;

inline bool NUMBER_IS_2_POW_K(int x) {
	return ((!((x) & ((x) - 1))) && ((x) > 1));
}

const double PI = 3.141592653589793238462643383279;
const float LEVEL_SMETER = 33.3f;

typedef enum _trxmode
{
	RX,
	TX
} TRXMODE;

enum {
	LOCK_DIDABLE        = 0x00000000,
	LOCK_DDS            = 0x00000001,
	LOCK_FILTER         = 0x00000002,
	LOCK_FILTER_BAND    = 0x00000004,
	LOCK_BAND           = 0x00000008,
	LOCK_MODE           = 0x00000010,
	LOCK_MOX            = 0x00000020,
	LOCK_TONE           = 0x00000040,
	LOCK_LOCK           = 0x00000080,
	LOCK_RIT            = 0x00000100,
	LOCK_SPLIT          = 0x00000200,
	LOCK_PREAMP         = 0x00000400,
	LOCK_VOICE          = 0x00000800,
	LOCK_ALL            = 0xFFFFFFFF,
};

typedef enum
{
  FILTER0,
  FILTER1,
  FILTER2,
  FILTER3,
  FILTER4,
  FILTER5,
  FILTER6,
  FILTER7,
  FILTER8,
} FILTER;

typedef enum
{
  RADIO,
  SINE,
  SINE_TWO_TONE,
  NOISE,
  TRIANGLE,
  SAWTOOTH,
  SILENCE
} RX_INPUT_SIGNAL;


#endif /* DEFINES_H_ */
