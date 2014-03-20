CONFIG += c++11

# This is updated by the version bump script
VERSION = 0.8.4

INCLUDEPATH += . ..
WARNINGS += -Wall

isEmpty(PREFIX): PREFIX=/usr

HEADERS += \
	../src/KVMainWindow.h \
	../src/KVSettingsDialog.h \
	../src/KVNetworkAccessManager.h \
	../src/KVNetworkReply.h \
	../src/KVTranslator.h \
	../src/KVUtil.h

SOURCES += \
	../src/KVMainWindow.cpp \
	../src/KVSettingsDialog.cpp \
	../src/KVNetworkAccessManager.cpp \
	../src/KVNetworkReply.cpp \
	../src/KVTranslator.cpp \
	../src/KVTranslator_p.cpp \
	../src/KVUtil.cpp

FORMS += ../resources/KVSettingsDialog.ui ../resources/KVMainWindow.ui

UI_DIR = ../build/uics
RCC_DIR = ../build/rccs
MOC_DIR = ../build/mocs
OBJECTS_DIR = ../build/objs
DESTDIR = ../bin
