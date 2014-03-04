QT       += core gui webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KCTViewer
TEMPLATE = app

include(../KCTViewer.pri)

win32: LIBS += -lws2_32 -lmswsock

RC_FILE = ../KCTViewer.rc

macx: TARGET = "KCT Viewer"
unix:!macx: TARGET = kancolletool-viewer

binaries.path = $PREFIX/usr/bin
binaries.files = kancolletool-viewer
INSTALLS += binaries

SOURCES += main.cpp

RESOURCES += ../resources.qrc

OTHER_FILES += \
	../KCTViewer.rc \
	../resources/index.html
