#include "gtest/gtest.h"
extern "C" {
    #include "pgmspace.h"
    #include "../usb2nrf/RF functions.h"
    #include "../usb2nrf/RF protocol.h"
    #include "../usb2nrf/defines.h"
}
#include "units_structure.h"
#include <string.h>
#include <stddef.h>

namespace my {
namespace project {
namespace {



class ProtocolTest: public ::testing::Test {
protected:
    uint8_t *request, *response, responseLength;
#define RQ (reinterpret_cast<sRequest*>(this->request))
#define RS (reinterpret_cast<sResponse*>(this->response))

    const std::unordered_map<int, std::string> returnCodes = {
        {ercOk, "ok"},
        {ercAddresBadLength, "addressBadLength"},

        {ercChBadChannels, "badChannel"},
        {ercChValidationFailed, "channelValidationFailed"},

        {ercNotImplemented, "notImplemented"},
        {ercBadVersion, "badVersion"},
        {ercBadUnitId, "badUnitId"},
        {ercNotConsecutiveTransactionId, "notConsecutiveTransaction"},
        {ercBadFunctionId, "badFunctionId"},
        {ercResponseTooBig, "responseTooBig"},
        {ercBadRequestData, "badRequestData"},
    };
    ProtocolTest() {
        // set-up work for each test
        reinitialize_memory();
        protocolInit();
        this->request = reinterpret_cast<uint8_t*>(malloc(256));
        this->response = reinterpret_cast<uint8_t*>(malloc(256));
        memset(this->request, 0, 256);
        RQ->rqVersion = PROTOCOL_VERSION;
        RQ->rqFunctionId = eFNOP;
        memset(this->response, 0, 256);
        responseLength = 0;
    }

    ~ProtocolTest() override {
        // clean-up work for each test
        free(this->request);
        free(this->response);
    }
};

TEST_F(ProtocolTest, TestProtocolVersion) {
    for (int i = 0; i < 256; i++) {
        RQ->rqVersion = static_cast<uint8_t>(i);
        this->responseLength = 0;
        generateResponse(REQUEST_HEADER_SIZE, this->request, &this->responseLength, this->response);
        ASSERT_GE(this->responseLength, RESPONSE_HEADER_SIZE)
                << "incorrect response length for request protocol version "
                << i;
        if (PROTOCOL_VERSION == i)
            ASSERT_EQ(ercOk, RS->rsCode) << "not ok response code for correct protocol version";
        else
            ASSERT_EQ(ercBadVersion, RS->rsCode) << "not BadVersion response code for incorrect protocol version";

    }
}

TEST_F(ProtocolTest, TestRequestLength) {
    // build a correct request, but wrong length
    for (uint8_t i = 0; i < REQUEST_HEADER_SIZE; i++) {
        this->responseLength = 0;
        generateResponse(i, this->request, &this->responseLength, this->response);
        ASSERT_GE(this->responseLength, RESPONSE_HEADER_SIZE) << "wrong response length for short correct request";
        ASSERT_EQ(ercBadRequestData, RS->rsCode) << "wrong result code for request length " << i;
    }
    // incorrect request with incorrect length
    for (int v = 0;  v < 256; v++) {
        RQ->rqVersion = static_cast<uint8_t>(v);
        this->responseLength = 0;
        generateResponse(1, this->request, &this->responseLength, this->response);
        ASSERT_GE(this->responseLength, RESPONSE_HEADER_SIZE) << "wrong response length for 1 byte long request";
        ASSERT_EQ(ercBadRequestData, RS->rsCode);
    }
}

TEST_F(ProtocolTest, TestTransactionId) {
    for (unsigned int i = 0; i < 256; i++) {
        RQ->rqVersion = PROTOCOL_VERSION;
        RQ->rqTransactionId = static_cast<uint8_t>(i);
        RQ->rqFunctionId = eFNOP;
        this->responseLength = 0;
        generateResponse(REQUEST_HEADER_SIZE, this->request, &this->responseLength, this->response);
//        ASSERT_
        ASSERT_EQ(ercOk, RS->rsCode) << "not ok response " << this->returnCodes.at(RS->rsCode)
                                     << "at transaction id " << i;
        ASSERT_EQ(i, RS->rsTransactionId) << "transaction id not equal to given one for correct request"
                                          << "at transaction id " << i
                                          << "returned " << static_cast<unsigned int>(RS->rsTransactionId);
        // check for bad request. if it works for wrong protocol version, it hopefully will work for all errors
        RQ->rqVersion = PROTOCOL_VERSION + 1;
        generateResponse(REQUEST_HEADER_SIZE, this->request, &this->responseLength, this->response);
        ASSERT_EQ(i, RS->rsTransactionId) << "transaction id not equal to given one for incorrect request"
                                          << "at transaction id " << i
                                          << "returned " << static_cast<unsigned int>(RS->rsTransactionId);
    }
}

TEST_F(ProtocolTest, TestUnitFunctionIds) {
    for (unsigned int unit = 0; unit < 256; unit++) {
        for (unsigned int function = 0; function < 256; function++) {
            RQ->rqUnitId = static_cast<uint8_t>(unit);
            RQ->rqFunctionId = static_cast<uint8_t>(function);
            generateResponse(REQUEST_HEADER_SIZE, this->request, &this->responseLength, this->response);
            if (UNITS_LENGTH > unit) {
                // correct unit
                if ((0 == unit) == (0x10 > function)) {
                    // correct function
                    // may return absolutely anything, except those two codes at least
                    EXPECT_NE(ercBadUnitId, RS->rsCode) << "response BadUnitId for existent unit " << unit << " correct function " << function;
                    // hardcoded check for valid function numbers
                    if (6 >= function || (0x10 <= function && 0x12 >= function) || (0x20 <= function && 0x21 >= function))
                        EXPECT_NE(ercBadFunctionId, RS->rsCode) << "response BadFunctionId for existent unit " << unit
                                                                << " correct function " << function;
                    else
                        EXPECT_EQ(ercBadFunctionId, RS->rsCode) << "response not BadFunctionId " << this->returnCodes.at(RS->rsCode)
                                                                << " for inexistent function " << function << " at existent unit " << unit;
                } else {
                    // incorrect function
                    EXPECT_EQ(ercBadFunctionId, RS->rsCode) << "response not BadFunctionId " << this->returnCodes.at(RS->rsCode)
                                                            << " for inexistent function " << function << " at existent unit " << unit;
                }
            } else {
                EXPECT_EQ(ercBadUnitId, RS->rsCode) << "response not BadUnitId " << this->returnCodes.at(RS->rsCode)
                                                    << " for inexistent unit " << unit << " function " << function;
            }
        }
    }
}


} //
} // project
} // my
