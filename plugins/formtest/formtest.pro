TEMPLATE = lib

TARGET = formtest

CONFIG += qt warn_on plugin

DEFINES += LIBWOLFRAMECLIENT_VISIBILITY=Q_DECL_IMPORT X_EXPORT=Q_DECL_EXPORT BUILD_AS_PLUGIN

INCLUDEPATH += ../../wolfclient ../../libqtwolfclient

unix:!macx:LIBS += -L../../libqtwolfclient -lqtwolfclient
win32:LIBS += ../../libqtwolfclient/debug/qtwolfclient0.lib
macx:LIBS += ../../libqtwolfclient/build/Debug/libqtwolfclient.dylib

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets concurrent
}

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += designer
} else {
	CONFIG += designer
}

win32|mac: CONFIG+= debug_and_release
QTDIR_build:DESTDIR = $$QT_BUILD_TREE/plugins/designer
contains(TEMPLATE, ".*lib"):TARGET = $$qtLibraryTarget($$TARGET)

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# Input
SOURCES += \
    FormTestPlugin.cpp

HEADERS += \
    FormTestPlugin.hpp
