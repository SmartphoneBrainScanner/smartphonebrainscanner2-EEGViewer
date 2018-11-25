QT += opengl
QT += core gui
QT += widgets

win32:LIBS += opengl32.lib   
SOURCES += main.cpp \
    mycallback.cpp \
    scalpmap.cpp \
    mainwindow.cpp \
    glwidget.cpp


include(../smartphonebrainscanner2-core/src/sbs2.pri)

HEADERS += \
    mycallback.h \
    scalpmap.h \
    mainwindow.h \
    glwidget.h

RESOURCES += \
    resources.qrc

target.path = /bin/
INSTALLS += target
win32:RC_ICONS += eegviewer.ico
