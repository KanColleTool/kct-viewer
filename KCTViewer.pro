#-------------------------------------------------
#
# Project created by QtCreator 2013-12-22T00:17:48
#
#-------------------------------------------------

QT       += core gui webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KCTViewer
TEMPLATE = app
CONFIG += c++11
win32: {
	QT += winextras
	LIBS += -lws2_32 -lmswsock
}

RC_FILE = KCTViewer.rc

# This is updated by the version bump script
VERSION = 0.9.0

macx: TARGET = "KCT Viewer"
unix:!macx: TARGET = kancolletool-viewer

binaries.path = $$PREFIX/bin
binaries.files = kancolletool-viewer
INSTALLS += binaries

SOURCES += main.cpp \
	KVMainWindow.cpp \
	KVSettingsDialog.cpp \
	KVWebView.cpp \
	KVNetworkAccessManager.cpp \
	KVNetworkReply.cpp \
	KVTranslator.cpp \
	KVUtil.cpp

HEADERS  += KVMainWindow.h \
	KVSettingsDialog.h \
	KVWebView.h \
	KVNetworkAccessManager.h \
	KVNetworkReply.h \
	KVTranslator.h \
	KVUtil.h \
	KVDefaults.h

FORMS += \
	KVSettingsDialog.ui \
    KVMainWindow.ui

RESOURCES += resources.qrc

OTHER_FILES += KCTViewer.rc \
	index.html
