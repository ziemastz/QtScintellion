QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Dialog/logindialog.cpp \
    Dialog/registerdialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Dialog/logindialog.h \
    Dialog/registerdialog.h \
    mainwindow.h

FORMS += \
    Dialog/logindialog.ui \
    Dialog/registerdialog.ui \
    mainwindow.ui

INCLUDEPATH += ../QtScintellionRepository
INCLUDEPATH += ../QtScintellionDatabase
INCLUDEPATH += ../QtScintellionUserService

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QtScintellionDatabase/release -lQtScintellionDatabase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QtScintellionDatabase/debug -lQtScintellionDatabase

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QtScintellionRepository/release -lQtScintellionRepository
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QtScintellionRepository/debug -lQtScintellionRepository

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QtScintellionUserService/release -lQtScintellionUserService
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QtScintellionUserService/debug -lQtScintellionUserService

unix: LIBS += -L$$OUT_PWD/../QtScintellionUserService -lQtScintellionUserService
unix: LIBS += -L$$OUT_PWD/../QtScintellionRepository -lQtScintellionRepository
unix: LIBS += -L$$OUT_PWD/../QtScintellionDatabase -lQtScintellionDatabase

TRANSLATIONS += \
    QtScintellionUI_en_US.ts \
    QtScintellionUI_pl_PL.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
