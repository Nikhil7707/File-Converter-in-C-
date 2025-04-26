QT += widgets

CONFIG += c++17

SOURCES += \
    src/qt_main.cpp \
    src/FileConverter.cpp \
    src/MainWindow.cpp

HEADERS += \
    include/FileConverter.h \
    src/MainWindow.h

FORMS += \
    src/MainWindow.ui

INCLUDEPATH += include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target