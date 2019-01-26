QT = core gui
unix {
    QT += gui-private
}
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UGlobalHotkey
TEMPLATE = lib
CONFIG += c++11

DESTDIR = $$PWD/_bin

# Switch ABI to export (vs import, which is default)
DEFINES += UGLOBALHOTKEY_LIBRARY

include(lib/uglobalhotkey-headers.pri)
include(lib/uglobalhotkey-sources.pri)
include(lib/uglobalhotkey-libs.pri)
