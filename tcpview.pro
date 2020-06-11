QT += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -luuid #The libuuid library is used to generate unique identifiers for objects that may be accessible beyond the local system.

TARGET = tcpview
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    source/buffer.cpp \
    source/datasource.cpp \
    source/rootmodule.cpp \
    source/cconnectionstree.cpp \
    source/ccustomproxymodel.cpp \
    source/ccfontsize.cpp \
    source/cfilesavewrapper.cpp \
    source/updatethread.cpp \
    source/cportservicenames.cpp \
    source/cusername.cpp \
    source/clanguage.cpp

HEADERS  += mainwindow.h \
    source/buffer.h \
    source/datasource.h \
    source/rootmodule.h \
    source/cconnectionstree.h \
    source/tableheadercaption.h \
    source/ccustomproxymodel.h \
    source/ccfontsize.h \
    source/cfilesavewrapper.h \
    source/updatethread.h \
    source/cportservicenames.h \
    source/cusername.h \
    source/clanguage.h \
    source/defined.h

FORMS    += mainwindow.ui

DISTFILES += \
    data/tcpvieww.svg \
    data/tcpview.desktop \
    README.md

RESOURCES += \
    tcpview.qrc

TRANSLATIONS += \ #Language pack - Spanish and Russian - incomplete
    translations/language_en.ts \
    translations/language_ru.ts \
    translations/language_es.ts

