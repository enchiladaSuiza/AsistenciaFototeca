QT       += core gui uitools widgets sql multimedia multimediawidgets

CONFIG += c++17

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cameramanager.cpp \
    checar.cpp \
    comboboxdelegate.cpp \
    datos.cpp \
    dbmanager.cpp \
    escaner.cpp \
    historial.cpp \
    horario.cpp \
    main.cpp \
    opciones.cpp \
    personal.cpp \
    spinboxdelegate.cpp \
    timeeditdelegate.cpp \
    ventanaprincipal.cpp

HEADERS += \
    cameramanager.h \
    checar.h \
    comboboxdelegate.h \
    datos.h \
    dbmanager.h \
    escaner.h \
    historial.h \
    horario.h \
    opciones.h \
    personal.h \
    spinboxdelegate.h \
    timeeditdelegate.h \
    ventanaprincipal.h

FORMS += \
    checar.ui \
    datos.ui \
    escaner.ui \
    historial.ui \
    horario.ui \
    opciones.ui \
    personal.ui \
    ventanaprincipal.ui

RESOURCES += \
    resources.qrc

RC_ICONS = Calendario.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DEFINES += DISABLE_LIBRARY_FEATURES

INCLUDEPATH += $$PWD/QZXing/
unix: LIBS += -lQZXing
else: LIBS += -L$$PWD/QZXing/ -lQZXing
