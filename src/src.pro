TARGET = KCTViewer
TEMPLATE = app

include(../KCTViewer.pri)

ICON = ../resources/KCTViewer.ico
RC_FILE = ../KCTViewer.rc

TARGET = kancolletool-viewer

target.path = $$PREFIX/bin
INSTALLS += target

SOURCES += main.cpp
