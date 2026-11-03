TEMPLATE = app
TARGET = ReactionGame
ICON = ReactionGame.icns

QT = core gui widgets

HEADERS += \
    login_window.h \
    reaction_game.h \
    settings_window.h \
    traffic_light.h

SOURCES += \
    login_window.cpp \
    main.cpp \
    reaction_game.cpp \
    settings_window.cpp \
    traffic_light.cpp

RESOURCES += \
    resource.qrc

DISTFILES += \
    README.md \
    users.txt \
    ReactionGame.icns