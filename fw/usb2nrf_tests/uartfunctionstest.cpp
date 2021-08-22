#include "gtest/gtest.h"
#include "uartfunctionstest.h"
extern "C" {
    #include "../usb2nrf/UART functions.h"
    #include "../usb2nrf/UART protocol.h"
}
#include <exception>


namespace my {
namespace project {
namespace {


UARTFunctionsTest::UARTFunctionsTest()
{
    reinitialize_memory();
}

UARTFunctionsTest::~UARTFunctionsTest()
{

}

fUARTFunction UARTFunctionsTest::getFunctionPointer(eModemCommand command)
{
    for (tUARTCommandItem i: UARTFunctions) {
        uint8_t c_command = pgm_read_byte(reinterpret_cast<uint8_t*>(&(i.command)));
        fUARTFunction c_function = reinterpret_cast<fUARTFunction>(pgm_read_ptr(&i.function));
        if (command == c_command) {
            return c_function;
        }
    }
    throw NotFoundException();
}

TEST_F(UARTFunctionsTest, status_test) {
    fUARTFunction status = this->getFunctionPointer(mcStatus);
    sString request, response;
    response.data = reinterpret_cast<uint8_t*>(malloc(256));
    uint8_t result = status(reinterpret_cast<scString*>(&request), &response);
    ASSERT_EQ(eucOk, result);
    delete response.data;
}

}}}
