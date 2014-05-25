QT += core webkitwidgets testlib

TARGET = kancolle-viewer-test
CONFIG += testcase
CONFIG -= app_bundle

include(../KCTViewer.pri)

win32: LIBS += -lws2_32 -lmswsock

HEADERS += KCTViewerTest.h
SOURCES += KCTViewerTest.cpp
