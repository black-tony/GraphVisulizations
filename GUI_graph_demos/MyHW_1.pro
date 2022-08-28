QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arrayitemvisulizer.cpp \
    edgevisulizer.cpp \
    graphlinkvisulizer.cpp \
    inputwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    myarrowitem.cpp \
    mygraphvisulizer.cpp \
    nodevisulizer.cpp \
    queuevisulizer.cpp \
    stackvisulizer.cpp \
    visitwindow.cpp

HEADERS += \
    arrayitemvisulizer.h \
    edgevisulizer.h \
    graphlinkvisulizer.h \
    inputwindow.h \
    mainwindow.h \
    myarrowitem.h \
    mygraph.h \
    mygraphvisulizer.h \
    nodevisulizer.h \
    queuevisulizer.h \
    stackvisulizer.h \
    visitwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
