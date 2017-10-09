#-------------------------------------------------
#
# Project created by QtCreator 2015-12-07T20:25:59
#
#-------------------------------------------------


VERSION = 0.2.2

OBJECTS_DIR = build/obj/
#MOC_DIR = build/moc/
#RCC_DIR = build/rcc/
#UI_DIR = build/ui/

# This stuff is only used on Windows (in the Properties window).
QMAKE_TARGET_PRODUCT = QLambda
QMAKE_TARGET_COMPANY = Koi Science
QMAKE_TARGET_DESCRIPTION = DI-Lambda control software v$$VERSION
QMAKE_TARGET_COPYRIGHT = Copyright (C) 2016-2017


QT += core gui widgets serialport

TARGET = qlambda
TEMPLATE = app

unix {
    target.path=/usr/bin
    INSTALLS+=target
}

win32 {
    DESTDIR  = $$PWD/installer/packages/science.koi.qlambda/data
    RC_ICONS = icons/qlambda_96x96.ico

    # Copy over the required libs, and make a Windows installer
    # Followiung the example of UltraStar-Deluxe @ github.

    QMAKE_POST_LINK += windeployqt --release --no-translations --no-system-d3d-compiler --compiler-runtime --no-angle --no-opengl-sw $${DESTDIR} $$escape_expand(\\n\\t)
    # Clean up after running windeployqt, removing some superfluous Qt libraries
    QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/Qt5Svg.dll) $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/iconengines/qsvgicon.dll) $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $${QMAKE_DEL_DIR} $$shell_path($${DESTDIR}/iconengines) $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/imageformats/qicns.dll) $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/imageformats/qico.dll) $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/imageformats/qsvg.dll) $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/imageformats/qtga.dll) $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/imageformats/qtiff.dll) $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/imageformats/qwbmp.dll) $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$shell_path($${DESTDIR}/imageformats/qwebp.dll) $$escape_expand(\\n\\t)

    # Yes, we need version 2.0.3. 2.0.5 and 3.x don't work with Win XP.
    # You need to install QtIFW 2.0.3 in order to build an installer.
    QMAKE_POST_LINK += C:\Qt\QtIFW2.0.3\bin\binarycreator.exe --offline-only -c installer\config\config.xml -p installer\packages installer\qlambda-$${VERSION}-win32-installer.exe
}


SOURCES += main.cpp\
    mainwindow.cpp \
    serialportsmodel.cpp \
    dilambda.cpp

HEADERS  += mainwindow.h \
    serialportsmodel.h \
    dilambda.h

FORMS    += mainwindow.ui
