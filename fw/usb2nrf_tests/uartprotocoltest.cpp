#include "gtest/gtest.h"
#include "uartprotocoltest.h"
extern "C" {
    #include "../usb2nrf/UART functions.h"
    #include "../usb2nrf/UART protocol.h"
}
#include <exception>
#include <stdbool.h>


namespace my {
namespace project {
namespace {


UARTProtocolTest::UARTProtocolTest()
{
    reinitialize_memory();
    UARTProtocolInit();
}

/**
  * sends given packet to a uart protocol multiplexor
  * and performs protocol version check of the response
  */
::testing::AssertionResult UARTProtocolTest::processPacket(std::vector<uint8_t> request, uPackage *response)
{
    UARTBeginTransaction();
    for (unsigned int i = 0; i < request.size(); i++) {
        if ((i == ((request.size()-1))) != UARTProcessNextByte(request.at(i), response)) {
            return ::testing::AssertionFailure() << "request byte " << i << ", "
                                                 << "request length " << request.size() << ", "
                                                 << std::endl;
        }
    }
    if (response->pkg.protocolVersion != EXPECTED_PROTOCOL_VERSION) {
        return ::testing::AssertionFailure() << "response protocol version incorrect";
    }
    return ::testing::AssertionSuccess();
}

TEST_F(UARTProtocolTest, basic_format_test) {
    uPackage response;
    ASSERT_TRUE(this->processPacket({0, mcStatus, 0}, &response));
    ASSERT_EQ(mcStatus | 0x80, response.pkg.command) << "wrong command field in the response";
    ASSERT_EQ(eucOk, response.pkg.code) << "response code is not Ok";
    ASSERT_LT(0, response.pkg.payloadSize) << "no payload in the response";
}

TEST_F(UARTProtocolTest, negative_test) {
    uPackage response;
    ASSERT_TRUE(this->processPacket({255}, &response));
    EXPECT_EQ(mcNoFunction | 0x80, response.pkg.command);
    ASSERT_EQ(eucBadVersion, response.pkg.code);
    ASSERT_EQ(0, response.pkg.payloadSize);

    ASSERT_TRUE(this->processPacket({0, mcEcho | mcResponseFlag}, &response));
    EXPECT_EQ(mcNoFunction | 0x80, response.pkg.command);
    ASSERT_EQ(eucBadCommand, response.pkg.code);
    ASSERT_EQ(0, response.pkg.payloadSize);

    ASSERT_TRUE(this->processPacket({0, mcNoFunction, 0}, &response));
    EXPECT_EQ(mcNoFunction | 0x80, response.pkg.command);
    ASSERT_EQ(eucBadCommand, response.pkg.code);
    ASSERT_EQ(0, response.pkg.payloadSize);
}

}
}
}
