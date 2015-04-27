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
 *
 * edited by Tsukihime
 */

#ifndef PA19_H_
#define PA19_H_

#include <QtGui>
#include "portaudio.h"
#include "../../Defines.h"

struct PLUGIN_OPTIONS
{
	int cbPaDriverIndex;
	int cbPaInIndex;
	int cbPaOutIndex;
	int cbPaMicIndex;
	int cbPaSpeakerIndex;
	bool isTwoSoundCard;
	int cbPaChannelsIndex;
	int cbPaSampleRate;
	int cbPaBufferSizeIndex;
	int sbPaLattency;
	int sbDdsMul;
	void* userData;
};

typedef void StreamCallback(const void *Input, void *Output, unsigned long FrameCount, void *pUserData);

class pa19 : public QThread
{
	Q_OBJECT
	public:
		pa19(QObject * parent = 0);
		~pa19();

		void open();
		void close();

		int start(StreamCallback *Func);
		void stop();
		bool isStart();
		void setParam(PLUGIN_OPTIONS &options);

		void callBack(const void *input, void *output, unsigned long frameCount);

		QString getVersion();
		QStringList driverName();
		QStringList outDevName(int Host);
		QStringList inDevName(int Host);
	private:
		int startTwoSoundStream(StreamCallback *callback);
		StreamCallback *pCallbackFunc;
		PLUGIN_OPTIONS options;

		bool IsOpened, isStarted;
		int outOffsetIndex, inOffsetIndex;
		unsigned long bufferSize;

		PaStream	*pStream1;
		PaStream	*pStream2;

		PaStreamParameters inParam, outParam;
		PaStreamParameters inParam2, outParam2;
protected:
		void run();
};

#endif /* PA19_H_ */
