TEMPLATE = app
CONFIG -= qt
CONFIG -= app_bundle
CONFIG += console

QMAKE_CXXFLAGS += -std=c++17


DEFINES += 'UNIT_TESTING=1'

SOURCES += \
    "RFParser.c" \
    ../usb2nrf/functions.c \
    main.cpp \
    ../usb2nrf/messages.c \
    tests.c

HEADERS += \
    "../usb2nrf/RF parser.h" \
    "../usb2nrf/UART parser.h" \
    ../usb2nrf/defines.h \
    ../usb2nrf/functions.h \
    ../usb2nrf/messages.h \
    ../usb2nrf/nrf24l01-mnemonics.h \
    ../usb2nrf/nrf24l01.h \
    tests.h

#OBJECTS = $$quote($$OBJECTS)
#OBJECTS += '"debug/RF parser.o"'
OBJECTS += '../../../googletest/build-googletest-Desktop_Qt_5_12_5_MinGW_64_bit-u041eu0442u043bu0430u0434u043au0430/lib/libgtestd.a'
LIBS += '-lgtest'
#OBJECTS += '../../../googletest/build-googletest-Desktop_Qt_5_12_5_MinGW_64_bit-u041eu0442u043bu0430u0434u043au0430/lib/libgtest_maind.a'
INCLUDEPATH += '../../../googletest/googletest/include'

