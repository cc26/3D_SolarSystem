# 
# CS123 Final Project Starter Code
# Adapted from starter code graciously provided by CS195-U: 3D Game Engines
#

QT += core gui opengl

TARGET = final
TEMPLATE = app

# If you add your own folders, add them to INCLUDEPATH and DEPENDPATH, e.g.
# INCLUDEPATH += folder1 folder2
# DEPENDPATH += folder1 folder2

SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    resourceloader.cpp \
    targa.cpp \
    glm.cpp \
    camera.cpp \
    particleemitter.cpp \
    common.cpp \
    bcurve.cpp

HEADERS += mainwindow.h \
    view.h \
    camera.h \
    vector.h \
    resourceloader.h \
    targa.h \
    glm.h \
    particleemitter.h \
    common.h \
    bcurve.h

FORMS += mainwindow.ui

OTHER_FILES += \
    Shader/BumpMapping.vert \
    Shader/BumpMapping.frag

RESOURCES += \
    textures/textures.qrc
