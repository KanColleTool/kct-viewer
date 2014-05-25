TARGET = KCTViewer
TEMPLATE = app

include(../KCTViewer.pri)

ICON = ../resources/KCTViewer.ico
RC_FILE = ../KCTViewer.rc

macx: TARGET = "KCT Viewer"
unix:!macx: TARGET = kancolletool-viewer

target.path = $$PREFIX/bin
INSTALLS += target

SOURCES += main.cpp

RESOURCES += ../resources/resources.qrc
