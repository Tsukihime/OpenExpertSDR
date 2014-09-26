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

#include "Timer.h"

Timer::Timer()
{
	init();
	calcLatency();
	start();
}

void Timer::init()
{
	available = QueryPerformanceFrequency(&frequency);
	startCount.QuadPart = 0;
	count.QuadPart = 0;
}

void Timer::calcLatency()
{
	minMicroSecondsLatency = 0;
	normMicroSecondsLatency = 0;

	if (!available)
		return;

	double minimumAccumulation = 0;
	int minimumAccumulations = 0;
	double normalAccumulation = 0;
	int normalAccumulations = 0;

	for (int i = 10; i > 0; i--)
	{
		LARGE_INTEGER tick1;
		LARGE_INTEGER tick2;
		QueryPerformanceCounter(&tick1);
		QueryPerformanceCounter(&tick2);
		double begin = tick1.QuadPart * (1000000.0 / frequency.QuadPart);
		double end = tick2.QuadPart * (1000000.0 / frequency.QuadPart);
		minimumAccumulation += end - begin;
		minimumAccumulations++;

		start();
		normalAccumulation += getMicroSeconds();
		normalAccumulations++;
	}

	if (minimumAccumulations > 0)
		minMicroSecondsLatency = minimumAccumulation / minimumAccumulations;

	if (normalAccumulations > 0)
		normMicroSecondsLatency = normalAccumulation / normalAccumulations;
}

void Timer::start()
{
	if (!available)
		return;

	QueryPerformanceCounter(&startCount);
}

double Timer::getMicroSeconds()
{
	if (!available)
		return 0;

	QueryPerformanceCounter(&count);
	double microSecondsStart = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
	double microSecondsEnd = count.QuadPart * (1000000.0 / frequency.QuadPart);

	double us = (microSecondsEnd - microSecondsStart) - normMicroSecondsLatency;
	if (us < 0)
		us = 0;

	return (us);
}

double Timer::getMilliSeconds()
{
	return (getMicroSeconds() * 0.001);
}

double Timer::getSeconds()
{
	return (getMilliSeconds() * 0.000001);
}
