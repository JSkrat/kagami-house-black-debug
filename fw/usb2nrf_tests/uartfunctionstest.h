#ifndef UARTFUNCTIONSTEST_H
#define UARTFUNCTIONSTEST_H
#include "gtest/gtest.h"
extern "C" {
    #include "../usb2nrf/UART functions.h"
}


namespace my {
namespace project {
namespace {

class NotFoundException: public std::exception {

};

class UARTFunctionsTest : public ::testing::Test
{
public:
    UARTFunctionsTest();
    ~UARTFunctionsTest() override;

    fUARTFunction getFunctionPointer(eModemCommand command);
};

} //
} // project
} // my

#endif // UARTFUNCTIONSTEST_H
