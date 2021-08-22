cmake_minimum_required(VERSION 3.5)

#set(CMAKE_VERBOSE_MAKEFILE ON)

project(usb2nrf_tests LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set (CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall)

add_definitions(-D"UNIT_TESTING=1")

set(
    HEADERS
    ../usb2nrf/defines.h
    "../usb2nrf/RF protocol.h"
    "../usb2nrf/RF functions.h"
    "../usb2nrf/RF model.h"
    "../usb2nrf/RF info.h"
    "../usb2nrf/UART protocol.h"
    "../usb2nrf/UART functions.h"
    "../usb2nrf/messages.h"
    "uartfunctionstest.h"
    "uartprotocoltest.h"
    "RF model test.h"
)

add_executable(
    usb2nrf_tests
    ${HEADERS}
    pgmspace.c
    "nRF model.c"
    "../usb2nrf/RF functions.c"
    "../usb2nrf/RF protocol.c"
    "../usb2nrf/RF model.c"
    "../usb2nrf/RF info.c"
    "../usb2nrf/UART protocol.c"
    "../usb2nrf/UART functions.c"
    "../usb2nrf/messages.c"
    units_structure.cpp
    test_protocol.cpp
    test_functions.cpp
    uartfunctionstest.cpp
    uartprotocoltest.cpp
    "RF model test.cpp"
    main.cpp
)
include_directories(../../../googletest/googletest/include)
include_directories(../usb2nrf/)


find_library(
    TEST_LIBRARY
    gtestd
    ../../../googletest/build-googletest-Desktop_Qt_5_12_5_MinGW_64_bit-u041eu0442u043bu0430u0434u043au0430/lib
)

#find_library(
#    TEST_MAIN_LIBRARY
#    gtest_maind
#    ../../../googletest/build-googletest-Desktop_Qt_5_12_5_MinGW_64_bit-u041eu0442u043bu0430u0434u043au0430/lib
#)

target_link_libraries(usb2nrf_tests LINK_PUBLIC ${TEST_LIBRARY})
#target_link_libraries(usb2nrf_tests LINK_PUBLIC ${TEST_MAIN_LIBRARY})
