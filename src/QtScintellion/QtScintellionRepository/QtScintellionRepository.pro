QT       += core sql

CONFIG   += c++17 staticlib
TEMPLATE  = lib
TARGET    = QtScintellionRepository

SOURCES += \
    user.cpp \
    userrepository.cpp

HEADERS += \
    user.h \
    userrepository.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/lib
else: unix:!android: target.path = /opt/$${TARGET}/lib
!isEmpty(target.path): INSTALLS += target
