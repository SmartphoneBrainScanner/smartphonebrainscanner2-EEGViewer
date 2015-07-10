QT += opengl
QT += core gui
QT += widgets

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
# CONFIG += qdeclarative-boostable

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    mycallback.cpp \
    scalpmap.cpp \
    mainwindow.cpp \
    glwidget.cpp



include(../smartphonebrainscanner2-core/src/sbs2_binary_decryptor.pri)

HEADERS += \
    mycallback.h \
    scalpmap.h \
    mainwindow.h \
    glwidget.h

include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/version.xml \
    android/res/values-ro/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/drawable-hdpi/icon.png \
    android/res/values-ja/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-id/strings.xml \
    android/res/drawable/logo.png \
    android/res/drawable/icon.png \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/drawable-mdpi/icon.png \
    android/res/values-et/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/layout/splash.xml \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \






win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../sbs2-emotiv-decryptor-source/sbs2emotivdecryptor-build-desktop-Qt_4_7_4_in_PATH__System__Release/release/ -lsbs2emotivdecryptor
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../sbs2-emotiv-decryptor-source/sbs2emotivdecryptor-build-desktop-Qt_4_7_4_in_PATH__System__Release/debug/ -lsbs2emotivdecryptor
else:unix: LIBS += -L$$PWD/../../sbs2-emotiv-decryptor-source/sbs2emotivdecryptor-build-desktop-Qt_4_7_4_in_PATH__System__Release/ -lsbs2emotivdecryptor

INCLUDEPATH += $$PWD/../../sbs2-emotiv-decryptor-source/sbs2emotivdecryptor-build-desktop-Qt_4_7_4_in_PATH__System__Release
DEPENDPATH += $$PWD/../../sbs2-emotiv-decryptor-source/sbs2emotivdecryptor-build-desktop-Qt_4_7_4_in_PATH__System__Release

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../sbs2-emotiv-decryptor-source/sbs2emotivdecryptor-build-desktop-Qt_4_7_4_in_PATH__System__Release/release/sbs2emotivdecryptor.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../sbs2-emotiv-decryptor-source/sbs2emotivdecryptor-build-desktop-Qt_4_7_4_in_PATH__System__Release/debug/sbs2emotivdecryptor.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../sbs2-emotiv-decryptor-source/sbs2emotivdecryptor-build-desktop-Qt_4_7_4_in_PATH__System__Release/libsbs2emotivdecryptor.a
