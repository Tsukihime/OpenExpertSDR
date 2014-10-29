/* correctIQ.c

This routine restores quadrature between arms of an analytic signal
possibly distorted by ADC hardware.

This file is part of a program that implements a Software-Defined Radio.

Copyright (C) 2004, 2005, 2006 by Frank Brickle, AB2KT and Bob McGwier, N4HY

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

The authors can be reached by email at

ab2kt@arrl.net
or
rwmcgwier@comcast.net

or by paper mail at

The DTTS Microwave Society
6 Kathleen Place
Bridgewater, NJ 08807
*/

#include <common.h>

#define COEFF_SIZE	100
#define MAX_REJ_COUNT	1


BOOL isTx = 0;
int rxCorrEna = 1;
int rejCounter = 0;
static float a[COEFF_SIZE];
static float p[COEFF_SIZE];
static unsigned char cntK = 0;

IQ
newCorrectIQ (REAL phase, REAL gain)
{
  int i;

  IQ iq = (IQ) safealloc (1, sizeof (iqstate), "IQ state");
  iq->phase = phase;
  iq->gain = gain;

  for(i = 0; i < 100; i++)
  {
    a[i] = 1.0f;
	p[i] = 0.0f;
  }

  return iq;
}

void
delCorrectIQ (IQ iq)
{
  safefree ((char *) iq);
}

void
correctIQ (CXB sigbuf, IQ iq)
{

	// this part modified by Valery Mikhaylovsky mail maksimus1210@gmail.com

	int i;
	register float tmp1 = 0, tmp2 = 0, tmp3 = 0, gain = 1, P = 0;

	if(cntK > (COEFF_SIZE-1)) cntK = 0;

	if(isTx == 0)	// receive mode
	{
		if(rxCorrEna != 0)
		{
			if(rejCounter == (MAX_REJ_COUNT-1))
			{
				for (i = 0; i < CXBhave (sigbuf); i++)
				{
					tmp1 += CXBreal(sigbuf, i)*CXBreal(sigbuf, i);
    				tmp2 += CXBimag(sigbuf, i)*CXBimag(sigbuf, i);
    				tmp3 += CXBreal(sigbuf, i)*CXBimag(sigbuf, i);
				}
				if(tmp1 == 0)
    				tmp1 = 0.000001;

				gain = sqrt(tmp2/tmp1);

				if(gain > 1.5)
    				gain = 1.5;
				else if(gain < 0.5)
    				gain = 0.5;

				a[cntK] = gain;
				p[cntK] = tmp3/tmp1;
			}

				tmp1 = 0;
				tmp3 = 0;
				for(i = 0; i < COEFF_SIZE; i++)
				{
					tmp1 += a[i];
					tmp3 += p[i];
				}
				gain = tmp1/COEFF_SIZE;
				P = tmp3/COEFF_SIZE;

				if(P > 0.2)
    				P = 0.2;
				else if(P < -0.2)
    				P = -0.2;

				tmp1 = sqrt(1-P*P);
				if(tmp1 == 0)
    				tmp1 = 0.000001;
			if(++rejCounter >= MAX_REJ_COUNT) rejCounter = 0;

			for(i = 0; i < CXBhave (sigbuf); i++)
    			CXBimag(sigbuf, i) = (1.0/tmp1)*(CXBimag(sigbuf, i) - P*CXBreal(sigbuf, i))/gain;
			++cntK;
		}
	}
	else			// transmit mode
	{
		for (i = 0; i < CXBhave (sigbuf); i++)
		{
			CXBimag (sigbuf, i) += iq->phase * CXBreal (sigbuf, i);
			CXBreal (sigbuf, i) *= iq->gain;
		}
	}
}
