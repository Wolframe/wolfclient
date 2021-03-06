TEMPLATE = lib

TARGET = picturechooser

CONFIG += qt warn_on plugin

DEFINES += X_EXPORT=Q_DECL_EXPORT BUILD_AS_PLUGIN

INCLUDEPATH += ../filechooser

QT += 

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += designer
} else {
	CONFIG += designer
}

# Aba: we get link trouble on Windows currently, must recheck and fix
# X_EXPORT first!

#unix:LIBS += -L../filechooser -lfilechooser
#win32:LIBS += ../filechooser/debug/filechooser.lib

#unix;PRE_TARGETDEPS += ../filechooser/filechooser.so
#win32:PRE_TARGETDEPS += ../filechooser/debug/filechooser.lib

win32|mac: CONFIG+= debug_and_release
QTDIR_build:DESTDIR = $$QT_BUILD_TREE/plugins/designer
contains(TEMPLATE, ".*lib"):TARGET = $$qtLibraryTarget($$TARGET)

# install
target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target


# only because wolfclient links in visitors directly to the plugins
# TODO: must be solved later, so far we just deploy the plugins twice:
# as plugin (above) and as shared library (here):
isEmpty(LIBDIR) {
  LIBDIR = $${PREFIX}/lib
}
temptarget.files = libpicturechooser.so
temptarget.path = $${LIBDIR}
temptarget.CONFIG = no_check_exist
INSTALLS += temptarget

unix:QMAKE_LFLAGS += -Wl,-rpath,$$[QT_INSTALL_PLUGINS]/designer

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# Input
SOURCES += \
	PictureChooserPlugin.cpp \
	PictureChooser.cpp \
	../filechooser/FileChooser.cpp

HEADERS += \
	PictureChooserPlugin.hpp \
	PictureChooser.hpp \
	../filechooser/FileChooser.hpp
