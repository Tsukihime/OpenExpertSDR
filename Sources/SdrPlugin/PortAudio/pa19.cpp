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

#include "pa19.h"

int CallBack(const void *input,
             void *output,
             unsigned long frameCount,
             const PaStreamCallbackTimeInfo *timeInfo,
             unsigned long statusFlags,
             void *userData) {
    Q_UNUSED(timeInfo);
    Q_UNUSED(statusFlags);
    pa19 *obj = (pa19 *) userData;
    obj->callBack(input, output, frameCount);
    return paContinue;
}

void pa19::callBack(const void *input, void *output, unsigned long frameCount) {
    pCallbackFunc(input, output, frameCount, options.userData);
}

pa19::pa19(QObject *parent) : QThread(parent) {
    IsOpened = false;
    isStarted = false;
    outOffsetIndex = inOffsetIndex = 0;
    pCallbackFunc = 0;
    pStream1 = pStream2 = 0;
    memset(&options, 0, sizeof(PLUGIN_OPTIONS));
}

pa19::~pa19() {
}

void pa19::open() {
    if (!IsOpened) {
        IsOpened = true;
        Pa_Initialize();
    }
}

void pa19::close() {
    if (IsOpened) {
        IsOpened = false;
        Pa_Terminate();
    }
}

int pa19::start(StreamCallback *Func) {
    if (options.isTwoSoundCard)
        return startTwoSoundStream(Func);

    if (Func == 0) {
        qDebug() << "pa19: start( " << Func << " )";
        return -1;
    }

    QMessageBox msgBox;
    if (!isStarted) {
        int Err = 0;

        if (inParam.device < 0)
            return -1;
        if (outParam.device < 0)
            return -1;

        pCallbackFunc = Func;
        if (pStream1 != 0) {
            qDebug() << "pa19: start(StreamCallback): Pa_IsStreamStopped: " << Pa_GetErrorText(Err);
            Pa_AbortStream(pStream1);
            Pa_CloseStream(pStream1);
            pStream1 = 0;
            return -1;
        }

        Err = Pa_OpenStream(&pStream1, &inParam, &outParam, options.cbPaSampleRate, bufferSize, 0, CallBack, this);
        if (Err < 0) {
            qWarning() << "pa19::start(): " << Pa_GetErrorText(Err);
            msgBox.setText("PortAudio Error! Can't open stream.");
            msgBox.exec();
            return -1;
        }
        Err = Pa_StartStream(pStream1);
        if (Err < 0) {
            qWarning() << "pa19::start(): " << Pa_GetErrorText(Err);
            msgBox.setText("PortAudio Error! Can't start stream.");
            msgBox.exec();
            return -1;
        }
        isStarted = true;
        return 0;
    }
    return -2;
}

int pa19::startTwoSoundStream(StreamCallback *callback) {

    if (callback == NULL) {
        qDebug() << "pa19: startTwoSoundStream( NULL )";
        return -1;
    }

    QMessageBox msgBox;
    if (!isStarted) {
        if ((inParam.device < 0) || (outParam.device < 0) || (inParam2.device < 0) || (outParam2.device < 0))
            return -1;

        pCallbackFunc = callback;
        if (pStream1 != NULL) {
            qDebug() << "pa19: startTwoSoundStream(StreamCallback): Pa_IsStreamStopped";
            Pa_AbortStream(pStream1);
            Pa_CloseStream(pStream1);
            pStream1 = NULL;
            return -1;
        }

        if (pStream2 != NULL) {
            qDebug() << "pa19: startTwoSoundStream(StreamCallback): Pa_IsStreamStopped(2)";
            Pa_AbortStream(pStream2);
            Pa_CloseStream(pStream2);
            pStream2 = NULL;
            return -1;
        }

        int Err = Pa_OpenStream(&pStream1, &inParam, &outParam, options.cbPaSampleRate, bufferSize, 0, NULL, NULL);
        if (Err < 0) {
            qWarning() << "pa19::startTwoSoundStream(): " << Pa_GetErrorText(Err);
            msgBox.setText("PortAudio Error! Can't open stream. (1)");
            msgBox.exec();
            return -1;
        }

        Err = Pa_OpenStream(&pStream2, &inParam2, &outParam2, options.cbPaSampleRate, bufferSize, 0, NULL, NULL);
        if (Err < 0) {
            qWarning() << "pa19::startTwoSoundStream(): " << Pa_GetErrorText(Err);
            msgBox.setText("PortAudio Error! Can't open stream. (2)");
            msgBox.exec();
            return -1;
        }

        Err = Pa_StartStream(pStream1);
        if (Err < 0) {
            qWarning() << "pa19::startTwoSoundStream(): " << Pa_GetErrorText(Err);
            msgBox.setText("PortAudio Error! Can't start stream. (1)");
            msgBox.exec();
            return -1;
        }

        Err = Pa_StartStream(pStream2);
        if (Err < 0) {
            qWarning() << "pa19::startTwoSoundStream(): " << Pa_GetErrorText(Err);
            msgBox.setText("PortAudio Error! Can't start stream. (2)");
            msgBox.exec();
            return -1;
        }

        isStarted = true;
        QThread::start(QThread::HighestPriority);
        return 0;
    }
    return -2;
}

void pa19::stop() {
    if (isStarted) {
        isStarted = false;
        Pa_AbortStream(pStream1);
        Pa_CloseStream(pStream1);

        pStream1 = 0;

        if (pStream2 != NULL) {
            wait(2000);
            Pa_AbortStream(pStream2);
            Pa_CloseStream(pStream2);
            pStream2 = 0;
        }
    }
}

bool pa19::isStart() {
    return isStarted;
}

void pa19::setParam(PLUGIN_OPTIONS &options) {
    this->options = options;
    int ChannelCount;
    switch (options.cbPaChannelsIndex) {
        case 1:
            ChannelCount = 4;
            break;
        default:
            ChannelCount = 2;
            break;
    }

    outParam.device = Pa_HostApiDeviceIndexToDeviceIndex(options.cbPaDriverIndex,
                                                         options.cbPaOutIndex + outOffsetIndex);
    outParam.channelCount = ChannelCount;
    outParam.sampleFormat = paFloat32 | paNonInterleaved;
    outParam.suggestedLatency = options.sbPaLattency / 1000.0;
    outParam.hostApiSpecificStreamInfo = 0;

    inParam.device = Pa_HostApiDeviceIndexToDeviceIndex(options.cbPaDriverIndex,
                                                        options.cbPaInIndex + inOffsetIndex);
    inParam.channelCount = ChannelCount;
    inParam.sampleFormat = paFloat32 | paNonInterleaved;
    inParam.suggestedLatency = options.sbPaLattency / 1000.0;
    inParam.hostApiSpecificStreamInfo = 0;

    outParam2.device = Pa_HostApiDeviceIndexToDeviceIndex(options.cbPaDriverIndex,
                                                          options.cbPaSpeakerIndex + outOffsetIndex);
    outParam2.channelCount = ChannelCount;
    outParam2.sampleFormat = paFloat32 | paNonInterleaved;
    outParam2.suggestedLatency = options.sbPaLattency / 1000.0;
    outParam2.hostApiSpecificStreamInfo = 0;

    inParam2.device = Pa_HostApiDeviceIndexToDeviceIndex(options.cbPaDriverIndex,
                                                         options.cbPaMicIndex + inOffsetIndex);
    inParam2.channelCount = ChannelCount;
    inParam2.sampleFormat = paFloat32 | paNonInterleaved;
    inParam2.suggestedLatency = options.sbPaLattency / 1000.0;
    inParam2.hostApiSpecificStreamInfo = 0;

    bufferSize = 0x100 << options.cbPaBufferSizeIndex;
}

QString pa19::getVersion() {
    return QString(Pa_GetVersionText());
}

QStringList pa19::driverName() {
    QStringList lst;
    for (int i = 0; i < Pa_GetHostApiCount(); i++) {
        const PaHostApiInfo *pInfo = Pa_GetHostApiInfo(i);
        lst << QString::fromLocal8Bit(pInfo->name);
    }
    return lst;
}

QStringList pa19::outDevName(int Host) {
    bool set = false;
    QStringList lst;

    if (Host < 0) Host = 0;

    const PaHostApiInfo *pInfo = Pa_GetHostApiInfo(Host);

    for (int i = 0; i < pInfo->deviceCount; i++) {
        int DeviceIndex = Pa_HostApiDeviceIndexToDeviceIndex(Host, i);
        const PaDeviceInfo *pDevInfo = Pa_GetDeviceInfo(DeviceIndex);
        if (pDevInfo->maxOutputChannels > 0) {
            lst << QString::fromLocal8Bit(pDevInfo->name);
            if (!set) {
                set = true;
                outOffsetIndex = i;
            }
        }
    }
    return lst;
}

QStringList pa19::inDevName(int Host) {
    bool set = false;
    QStringList lst;

    if (Host < 0) Host = 0;

    const PaHostApiInfo *pInfo = Pa_GetHostApiInfo(Host);

    for (int i = 0; i < pInfo->deviceCount; i++) {
        int DeviceIndex = Pa_HostApiDeviceIndexToDeviceIndex(Host, i);
        const PaDeviceInfo *pDevInfo = Pa_GetDeviceInfo(DeviceIndex);
        if (pDevInfo->maxInputChannels > 0) {
            if (!set) {
                set = true;
                inOffsetIndex = i;
            }
            lst << QString::fromLocal8Bit(pDevInfo->name);
        }
    }
    return lst;
}

void pa19::run() {
    int bytesNeeded = (int) bufferSize * sizeof(float);

    QByteArray fromTrxL(bytesNeeded, 0);
    QByteArray fromTrxR(bytesNeeded, 0);
    QByteArray toTrxL(bytesNeeded, 0);
    QByteArray toTrxR(bytesNeeded, 0);
    QByteArray fromUserL(bytesNeeded, 0);
    QByteArray fromUserR(bytesNeeded, 0);
    QByteArray toUserL(bytesNeeded, 0);
    QByteArray toUserR(bytesNeeded, 0);

    void *input[4] = {
            fromTrxL.data(),
            fromTrxR.data(),
            fromUserL.data(),
            fromUserR.data()
    };
    void *output[4] = {
            toUserL.data(),
            toUserR.data(),
            toTrxL.data(),
            toTrxR.data()
    };

    void *trxRead[2] = {fromTrxL.data(), fromTrxR.data()};
    void *userRead[2] = {fromUserL.data(), fromUserR.data()};
    void *trxWrite[2] = {toTrxL.data(), toTrxR.data()};
    void *userWrite[2] = {toUserL.data(), toUserR.data()};

    while (isStarted) {
        Pa_ReadStream(pStream1, trxRead, bufferSize);
        Pa_ReadStream(pStream2, userRead, bufferSize);
        pCallbackFunc(input, output, bufferSize, options.userData);
        Pa_WriteStream(pStream1, trxWrite, bufferSize);
        Pa_WriteStream(pStream2, userWrite, bufferSize);
    }
}
