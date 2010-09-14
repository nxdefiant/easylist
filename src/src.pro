#-------------------------------------------------
#
# Project created by QtCreator 2010-09-07T13:50:39
#
#-------------------------------------------------

# This needs to be removed in order for the binary
# to be chmod to 755 in the debian package.
#QT       += core gui
QT += dbus
TARGET = easylist
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .
DESTDIR = ../

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui \
    listwindow.ui

CONFIG += mobility
MOBILITY = 

unix {
    #VARIABLES
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    BINDIR = $$PREFIX/bin
    DATADIR = $$PREFIX/share

    DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

    #MAKE INSTALL
    INSTALLS += target desktop icon26 icon40 icon48 icon64

    target.path =$$BINDIR

    desktop.path = $$DATADIR/applications/hildon
    desktop.files += $${TARGET}.desktop

    #iconxpm.path = $$DATADIR/pixmap
    #iconxpm.files += data/maemo/$${TARGET}.xpm

    icon26.path = $$DATADIR/icons/hicolor/26x26/apps
    icon26.files += data/26x26/$${TARGET}.png

    icon40.path = $$DATADIR/icons/hicolor/40x40/apps
    icon40.files += data/40x40/$${TARGET}.png

    icon48.path = $$DATADIR/icons/hicolor/48x48/apps
    icon48.files += data/48x48/$${TARGET}.png

    icon64.path = $$DATADIR/icons/hicolor/64x64/apps
    icon64.files += data/64x64/$${TARGET}.png

}