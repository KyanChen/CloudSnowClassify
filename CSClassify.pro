QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    svm.cpp \
    traindata.cpp \
    getfeatures.cpp \
    readData.cpp \
    testdata.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


HEADERS += \
    config.h \
    svm.h \
    traindata.h \
    getfeatures.h \
    readData.h \
    testdata.h

macx: LIBS += -L$$PWD/../../../../../../../usr/local/Cellar/gdal/2.3.1_2/lib/ -lgdal.20

INCLUDEPATH += $$PWD/../../../../../../../usr/local/Cellar/gdal/2.3.1_2/include
DEPENDPATH += $$PWD/../../../../../../../usr/local/Cellar/gdal/2.3.1_2/include


QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig
PKGCONFIG += opencv
