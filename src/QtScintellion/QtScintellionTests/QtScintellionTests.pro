QT += core sql testlib

CONFIG += c++17 console testcase
TEMPLATE = app
TARGET = QtScintellionTests

SOURCES += \
    tst_users.cpp

INCLUDEPATH += ../QtScintellionDatabase
INCLUDEPATH += ../QtScintellionRepository
INCLUDEPATH += ../QtScintellionUserService

# Link static libs built by sibling projects
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QtScintellionDatabase/release -lQtScintellionDatabase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QtScintellionDatabase/debug -lQtScintellionDatabase
unix: LIBS += -L$$OUT_PWD/../QtScintellionDatabase -lQtScintellionDatabase

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QtScintellionRepository/release -lQtScintellionRepository
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QtScintellionRepository/debug -lQtScintellionRepository
unix: LIBS += -L$$OUT_PWD/../QtScintellionRepository -lQtScintellionRepository

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QtScintellionUserService/release -lQtScintellionUserService
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QtScintellionUserService/debug -lQtScintellionUserService
unix: LIBS += -L$$OUT_PWD/../QtScintellionUserService -lQtScintellionUserService
