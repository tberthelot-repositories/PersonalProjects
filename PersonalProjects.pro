#-------------------------------------------------
#
# Project created by QtCreator
# Author - Janani H. Kehelwala
# Plymouth Identification Number - 10515930
# Documented using Doxygen
#
#-------------------------------------------------

QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SprintedApp
TEMPLATE = app

SOURCES += main.cpp\
        mainwindowwidget.cpp \
    editor.cpp \
    layoutmanager.cpp \
    itemmanager.cpp \
    project.cpp \
    listbase.cpp \
    activity.cpp \
    note.cpp \
    sprint.cpp \
    databasemgr.cpp \
    databasesettings.cpp \
    initializer.cpp \    
    alertmgr.cpp \
    projectwidget.cpp \
    listwidget.cpp \
    activitywidget.cpp \
    editactivitywidget.cpp \
    notewidget.cpp \
    editprojectwidget.cpp \
    editorwidget.cpp \
    activitychildrenwidget.cpp \
    activitynoteswidget.cpp

HEADERS  += mainwindowwidget.h \
    editor.h \
    layoutmanager.h \
    itemmanager.h \
    project.h \
    listbase.h \
    activity.h \
    note.h \
    sprint.h \
    databasemgr.h \
    databasesettings.h \
    initializer.h \
    alertmgr.h \
    projectwidget.h \
    listwidget.h \
    activitywidget.h \
    editactivitywidget.h \
    notewidget.h \
    editprojectwidget.h \
    editorwidget.h \
    activitychildrenwidget.h \
    activitynoteswidget.h

FORMS    += mainwindowwidget.ui \
    projectwidget.ui \
    listwidget.ui \
    activitywidget.ui \
    notewidget.ui \
    editactivitywidget.ui \
    editprojectwidget.ui \
    activitychildrenwidget.ui \
    activitynoteswidget.ui \
    editorwidget.ui

RESOURCES += \
    resources.qrc

DISTFILES +=
