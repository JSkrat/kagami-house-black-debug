#ifndef UARTPROTOCOLTEST_H
#define UARTPROTOCOLTEST_H
#include "gtest/gtest.h"

extern "C" {
    #include "../usb2nrf/UART protocol.h"
}

#define EXPECTED_PROTOCOL_VERSION 0

namespace my {
namespace project {
namespace {


class ProcessNextByteError: public std::exception {

};

class UARTProtocolTest : public ::testing::Test
{
public:
    UARTProtocolTest();

    ::testing::AssertionResult processPacket(std::vector<uint8_t> request, uPackage *response);
};

}
}
}

#endif // UARTPROTOCOLTEST_H
