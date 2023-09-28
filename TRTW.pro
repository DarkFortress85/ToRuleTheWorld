QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    area.cpp \
    areamanager.cpp \
    areamenu.cpp \
    battle.cpp \
    cardchangemenu.cpp \
    country.cpp \
    countrymenu.cpp \
    game.cpp \
    gameover.cpp \
    main.cpp \
    mainmenu.cpp \
    map.cpp \
    myview.cpp \
    player.cpp \
    playerinit.cpp \
    playermenu.cpp \
    setplayerunitsmenu.cpp

HEADERS += \
    area.h \
    areamanager.h \
    areamenu.h \
    battle.h \
    cardchangemenu.h \
    country.h \
    countrymenu.h \
    game.h \
    gameover.h \
    mainmenu.h \
    map.h \
    myview.h \
    player.h \
    playerinit.h \
    playermenu.h \
    setplayerunitsmenu.h

FORMS += \
    areamenu.ui \
    battle.ui \
    cardchangemenu.ui \
    countrymenu.ui \
    game.ui \
    gameover.ui \
    mainmenu.ui \
    playerinit.ui \
    playermenu.ui \
    setplayerunitsmenu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
