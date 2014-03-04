QT += core webkitwidgets testlib

TARGET = kancolle-viewer-test
CONFIG += testcase

include(../KCTViewer.pri)

win32: LIBS += -lws2_32 -lmswsock

SOURCES += TestKVTranslator.cpp
