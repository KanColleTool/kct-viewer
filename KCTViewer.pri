QT     += core gui webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32: {
	QT += winextras
	LIBS += -lws2_32 -lmswsock
}

CONFIG += c++11

# This is updated by the version bump script
VERSION = 0.9.0

INCLUDEPATH += . ..
WARNINGS += -Wall

isEmpty(PREFIX): PREFIX=/usr

HEADERS += \
	../src/KVMainWindow.h \
	../src/KVWebView.h \
	../src/KVSettingsDialog.h \
	../src/KVNetworkAccessManager.h \
	../src/KVNetworkReply.h \
	../src/KVTranslator.h \
	../src/KVUtil.h \
	../src/KVDefaults.h

SOURCES += \
	../src/KVMainWindow.cpp \
	../src/KVSettingsDialog.cpp \
	../src/KVWebView.cpp \
	../src/KVNetworkAccessManager.cpp \
	../src/KVNetworkReply.cpp \
	../src/KVTranslator.cpp \
	../src/KVTranslator_p.cpp \
	../src/KVUtil.cpp

OTHER_FILES += \
	../resources/report_blacklist.json

FORMS += ../resources/KVSettingsDialog.ui ../resources/KVMainWindow.ui

RESOURCES += ../resources/resources.qrc

UI_DIR = ../build/uics
RCC_DIR = ../build/rccs
MOC_DIR = ../build/mocs
OBJECTS_DIR = ../build/objs
DESTDIR = ../bin
