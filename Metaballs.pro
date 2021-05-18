QT       += widgets core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    abstractmetaball.cpp \
    cell.cpp \
    circle.cpp \
    grid.cpp \
    main.cpp \
    mainwindow.cpp \
    marchingsquaressolver.cpp \
    metaballslistmodel.cpp \
    murakamimetaball.cpp \
    openglscene.cpp

HEADERS += \
    abstractmetaball.h \
    cell.h \
    circle.h \
    grid.h \
    mainwindow.h \
    marchingsquaressolver.h \
    metaballslistmodel.h \
    murakamimetaball.h \
    openglscene.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    draw.frag \
    draw.vert

RESOURCES += \
    resources.qrc
