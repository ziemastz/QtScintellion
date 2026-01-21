QT       += core sql

CONFIG   += c++17 staticlib
TEMPLATE  = lib
TARGET    = QtScintellionUserService

SOURCES += \
    userservice.cpp

HEADERS += \
    userservice.h

INCLUDEPATH += ../QtScintellionDatabase
INCLUDEPATH += ../QtScintellionRepository

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/lib
else: unix:!android: target.path = /opt/$${TARGET}/lib
!isEmpty(target.path): INSTALLS += target
