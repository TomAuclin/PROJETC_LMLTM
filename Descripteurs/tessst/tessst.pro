QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
INCLUDEPATH += /usr/include/opencv4
LIBS += `pkg-config --cflags --libs opencv4`
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GestionUtilisateur.cpp \
    Image.cpp \
    Library.cpp \
    GestionUtilisateur.cpp \
    Traitement.cpp \
    bibliowindow.cpp \
    connexion.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    GestionUtilisateur.hpp \
    Image.hpp \
    Library.hpp \
    GestionUtilisateur.hpp \
    Traitement.hpp \
    bibliowindow.h \
    connexion.h \
    mainwindow.h

FORMS += \
    bibliowindow.ui \
    connexion.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
