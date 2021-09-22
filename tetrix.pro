QT += widgets

HEADERS       = tetrixboard.h \
                gpio_pin.h \
                tetrixpiece.h \
                tetrixwindow.h
SOURCES       = main.cpp \
                gpio_pin.cpp \
                tetrixboard.cpp \
                tetrixpiece.cpp \
                tetrixwindow.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/tetrix
INSTALLS += target

DISTFILES += \
    a.png \
    logo.png
