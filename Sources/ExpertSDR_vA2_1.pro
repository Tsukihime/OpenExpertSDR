TEMPLATE = app
TARGET = ExpertSDR

VERSION = 2.1.4.21

TRANSLATIONS = ExpertSDR_ru.ts

QT += core \
    gui \
    network \
    opengl

HEADERS += CalibrateSC/Calibrator/CalibrProc.h \
    CalibrateSC/Calibrator/Calibrator.h \
    Wav/fileList.h \
    Wav/wavSample.h \
    WavReader.h \
    SmeterGUI/ColorWgt.h \
    SmeterGUI/SMeterOptions.h \
    SmeterGUI/SMeterSDR.h \
    AudioThread.h \
    scale/Scale.h \
    scale/ScaleNum.h \
    CW/CwCore.h \
    CW/CwMacro.h \
    CW/CwTimer.h \
    CW/correctIQ.h \
    Vac.h \
    ringBuf.h \
    Cat/ExtSerialPort/qextserialbase.h \
    Cat/ExtSerialPort/qextserialenumerator.h \
    Cat/ExtSerialPort/qextserialport.h \
    Cat/ExtSerialPort/win_qextserialport.h \
    SdrPlugin/PortAudio/pa19.h \
    SdrPlugin/PortAudio/portaudio.h \
    Valcoder/HID/Ea1dev.h \
    Valcoder/Panel.h \
    S-Meter/Number2.h \
    S-Meter/Number.h \
    S-Meter/Draw.h \
    S-Meter/S_Meter.h \
    PanaramDefines.h \
    Panarama.h \
    PanoramOpt.h \
    ThreadFunc.h \
    About.h \
    CalibrateSC/progresscalibrate.h \
    Cat/CatManager.h \
    Defines.h \
    DttSP.h \
    ExpertSDR_vA2_1.h \
    FreqScale/Didgit.h \
    Options.h \
    SMeter.h \
    SdrPlugin/SdrPlugin.h \
    WdgAddStation.h \
    WdgGraph.h \
    WdgMem.h \
    Wav/VoiceRecorderWave.h \
    Logger/VLogger.h \
    Resampler.h \
    Timer/Timer.h \
    RingResampler/RingResampler.h \
    RingResampler/RingBuffer.h \
    SdrPlugin/PluginCtrl.h \
    SdrPlugin/extioplugin.h \
    dttsplibrary.h \
    GLFont/glfont.h \
    bands/BandManager.h \
    bands/ModeOptions.h

SOURCES += CalibrateSC/Calibrator/CalibrProc.cpp \
    CalibrateSC/Calibrator/Calibrator.cpp \
    Wav/fileList.cpp \
    Wav/wavSample.cpp \
    WavReader.cpp \
    SmeterGUI/ColorWgt.cpp \
    SmeterGUI/SMeterOptions.cpp \
    SmeterGUI/SMeterSDR.cpp \
    AudioThread.cpp \
    scale/Scale.cpp \
    scale/ScaleNum.cpp \
    CW/CwCore.cpp \
    CW/CwMacro.cpp \
    CW/CwTimer.cpp \
    CW/correctIQ.cpp \
    Vac.cpp \
    ringBuf.cpp \
    Cat/ExtSerialPort/qextserialbase.cpp \
    Cat/ExtSerialPort/qextserialenumerator.cpp \
    Cat/ExtSerialPort/qextserialport.cpp \
    Cat/ExtSerialPort/win_qextserialport.cpp \
    SdrPlugin/PortAudio/pa19.cpp \
    Valcoder/HID/Ea1dev.cpp \
    Valcoder/Panel.cpp \
    S-Meter/Number2.cpp \
    S-Meter/Number.cpp \
    S-Meter/Draw.cpp \
    S-Meter/S_Meter.cpp \
    ExpertSDR_vA2_1.cpp \
    Panarama.cpp \
    PanoramOpt.cpp \
    ThreadFunc.cpp \
    About.cpp \
    CalibrateSC/progresscalibrate.cpp \
    Cat/CatManager.cpp \
    DttSP.cpp \
    FreqScale/Didgit.cpp \
    Options.cpp \
    SMeter.cpp \
    SdrPlugin/SdrPlugin.cpp \
    WdgAddStation.cpp \
    WdgGraph.cpp \
    WdgMem.cpp \
    main.cpp \
    Wav/VoiceRecorderWave.cpp \
    Logger/VLogger.cpp \
    Resampler.cpp \
    Timer/Timer.cpp \
    RingResampler/RingResampler.cpp \
    RingResampler/RingBuffer.cpp \
    SdrPlugin/PluginCtrl.cpp \
    SdrPlugin/extioplugin.cpp \
    dttsplibrary.cpp \
    GLFont/glfont.cpp \
    bands/BandManager.cpp \
    bands/ModeOptions.cpp

RESOURCES += images.qrc \
    bg.qrc
FORMS += CalibrateSC/Calibrator/Calibrator.ui \
    Wav/fileList.ui \
    Wav/wavSample.ui \
    SmeterGUI/SMeterOptions.ui \
    scale/Scale.ui \
    CW/CwMacro.ui \
    Valcoder/Panel.ui \
    S-Meter/Number2.ui \
    S-Meter/Number.ui \
    S-Meter/S_Meter.ui \
    PanoramOpt.ui \
    About.ui \
    CalibrateSC/progresscalibrate.ui \
    ExpertSDR_vA2_1.ui \
    Options.ui \
    SMeter.ui \
    WdgAddStation.ui \
    WdgGraph.ui \
    WdgMem.ui
RC_FILE = browser.rc
LIBS += -lhid \
    -lwinmm \
    -lsetupapi \
    portaudio_x86.lib \
    libwsock32
LIBS += -lglu32
