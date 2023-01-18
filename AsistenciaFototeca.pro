QT       += core gui uitools sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    checar.cpp \
    datos.cpp \
    dbmanager.cpp \
    horario.cpp \
    main.cpp \
    personal.cpp \
    ventanaprincipal.cpp

HEADERS += \
    checar.h \
    datos.h \
    dbmanager.h \
    horario.h \
    personal.h \
    ventanaprincipal.h

FORMS += \
    checar.ui \
    datos.ui \
    horario.ui \
    personal.ui \
    ventanaprincipal.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

