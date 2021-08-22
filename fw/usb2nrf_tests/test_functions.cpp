#include "gtest/gtest.h"
extern "C" {
    #include "pgmspace.h"
    #include "../usb2nrf/RF functions.h"
    #include "../usb2nrf/RF protocol.h"
    #include "../usb2nrf/defines.h"
    #include "../usb2nrf/RF model.h"
    #include "../usb2nrf/RF info.h"
}
#include "units_structure.h"

namespace my {
namespace project {
namespace {

class FunctionsTest: public ::testing::Test {
protected:
    const unsigned int requestMaxSize = 256;
    const unsigned int responseMaxSize = 1030;
    sString request;
    sString response;
    uint8_t responseLength;
#define CONST_RQ (reinterpret_cast<scString*>(&this->request))
//#define RQ (reinterpret_cast<sRequest*>(this->request))
//#define RS (reinterpret_cast<sResponse*>(this->response))

    fRFFunction getFunction(eFunctions from) {
        return reinterpret_cast<fRFFunction>(
                    pgm_read_ptr(
                        const_cast<void*>(
                            reinterpret_cast<const void*>(&(RFFunctions[from]))
                        )
                    )
        );
    }

    FunctionsTest() {
        // set-up work for each test
        reinitialize_memory();
        this->request = {
            .length = 0,
            .data = reinterpret_cast<uint8_t*>(malloc(this->requestMaxSize))
        };
        this->response = {
            .length = 0,
            .data = reinterpret_cast<uint8_t*>(malloc(this->responseMaxSize))
        };
        memset(this->request.data, 0, this->requestMaxSize);
        memset(this->response.data, 0, this->responseMaxSize);
    }

    ~FunctionsTest() override {
        // clean-up work for each test
        free(request.data);
        free(response.data);
    }
};

/*TEST_F(FunctionsTest, Test_session_key) {
    int ret = (*(this->getFunction(eFSetSessionKey)))(0, nullptr, nullptr);
    EXPECT_EQ(C_NOT_IMPLEMENTED, ret);
}//*/

TEST_F(FunctionsTest, TestListOfUnits) {
    ASSERT_EQ(ercOk, (*(this->getFunction(eFGetListOfUnits)))(0, nullptr, &this->response));
    ASSERT_EQ(UNITS_LENGTH*4, this->response.length) << "response has wrong length" << this->response.length;
    // and check units one by one
    for (uint8_t unit = 0; unit < this->response.length/4; unit++) {
        uint8_t base = 0 + unit*4;
        /// @todo check if response equals the real data
        uint8_t type = this->response.data[base];
        EXPECT_TRUE(type < eUTSLast || ((type > eUTACustom) && (type < eUTALast))) << "wrong unit type" << type;
        EXPECT_TRUE(
                    (this->response.data[base+1] > 0) ||
                    (this->response.data[base+2] > 0) ||
                    (this->response.data[base+3] > 0) ||
                    (0 == unit) // until we change all global (0x00 --- 0x0F) functions to channels of unit 0
                ) << "unit can not be without any channel. Unit " << static_cast<int>(unit);
        // 16 channels max per access_type
        EXPECT_GT(16, this->response.data[base+1]);
        EXPECT_GT(16, this->response.data[base+2]);
        EXPECT_GT(16, this->response.data[base+3]);
    }
}

TEST_F(FunctionsTest, TestSetMacAddress) {
    auto testMac = [this](uint8_t mac_length, int result) {
        this->request.length = mac_length;
        this->response.length = 0;
        EXPECT_EQ(result, (*(this->getFunction(eFSetAddress)))(0, CONST_RQ, &this->response));
        EXPECT_EQ(0, this->response.length);
    };
    testMac(MAC_SIZE, ercOk);
    testMac(0, ercAddresBadLength);
    testMac(MAC_SIZE - 1, ercAddresBadLength);
    testMac(MAC_SIZE + 1, ercAddresBadLength);
}

TEST_F(FunctionsTest, TestResetTransactionId) {
    ASSERT_EQ(ercOk, (*(this->getFunction(eFResetTransactionId)))(0, nullptr, &this->response));
    ASSERT_EQ(1, this->response.length);
}

TEST_F(FunctionsTest, TestNOP) {
    ASSERT_EQ(ercOk, (*(this->getFunction(eFNOP)))(0, nullptr, &this->response));
    ASSERT_EQ(0, this->response.length);
}

TEST_F(FunctionsTest, TestGetStatistics) {
    total_requests = 0xFEDC;
    ok_responses = 0xEDCB;
    error_responses = 0xCDBA;
    missed_packets = 0xBCA9;
    ack_timeouts = 0xAB98;
    validation_errors = 0x7654;
    ASSERT_EQ(ercOk, (*(this->getFunction(eFGetStatistics)))(0, nullptr, &this->response));
#pragma pack()
    typedef struct {
        uint16_t requests;
        uint16_t responses;
        uint16_t errors;
        uint16_t trans_errors;
        uint16_t ack_to;
        uint16_t validation_errors;
    } tRFStatResponse;
    ASSERT_EQ(sizeof(tRFStatResponse), this->response.length);
//    tRFStatResponse *parsedResponse = reinterpret_cast<tRFStatResponse *>(&(response.data));
    EXPECT_EQ(total_requests, *reinterpret_cast<uint16_t*>(this->response.data));
    EXPECT_EQ(ok_responses, *reinterpret_cast<uint16_t*>(this->response.data+2));
    EXPECT_EQ(error_responses, *reinterpret_cast<uint16_t*>(this->response.data+4));
    EXPECT_EQ(missed_packets, *reinterpret_cast<uint16_t*>(this->response.data+6));
    EXPECT_EQ(ack_timeouts, *reinterpret_cast<uint16_t*>(this->response.data+8));
    EXPECT_EQ(validation_errors, *reinterpret_cast<uint16_t*>(this->response.data+10));
}

TEST_F(FunctionsTest, TestGetPropertiesOfUnit) {
    // first let's get list of all units
    // assuming that function is already tested and working
    ASSERT_EQ(ercOk, (*(this->getFunction(eFGetListOfUnits)))(0, nullptr, &this->response));
    ASSERT_LT(0, this->response.length) << "list of units response is empty";
//    for (int unit = 0; unit < 256; unit++) {
    for (uint8_t unit = 0; unit < this->response.length/4; unit++) {
        sString p_response = {
            .length = 0,
            .data = reinterpret_cast<uint8_t*>(malloc(256))
        };
        const uint8_t code = (*(this->getFunction(eFGetPropertiesOfUnit)))(unit & 0xFF, nullptr, &p_response);
        // and now let's check the properties of unit
        if (unit < this->response.data[0]) {
            const uint8_t u_base = 0 + 4*(unit&0xFF);
            const uint8_t type = this->response.data[u_base];
            const uint8_t num_ro = this->response.data[u_base+1];
            const uint8_t num_wo = this->response.data[u_base+2];
            const uint8_t num_rw = this->response.data[u_base+3];
            EXPECT_EQ(ercOk, code);
            ASSERT_EQ(1 + 4*3, p_response.length) << "response length should be 1 + 4x3 bytes long" << p_response.length;
            EXPECT_EQ(type, p_response.data[0]);
            // at least we could check if non-existent channels have type 00
            for (int byte_num = 0; byte_num < 4; byte_num++) {
                for (int j = 0; j < 4; j++) {
                    const int channel = byte_num*4+j;
                    if (num_ro <= channel) {
                        EXPECT_EQ(0, (p_response.data[1+(byte_num*1)] >> (j*2)) & 0b11);
                    }
                    if (num_wo <= channel) {
                        EXPECT_EQ(0, (p_response.data[1+(byte_num*2)] >> (j*2)) & 0b11);
                    }
                    if (num_rw <= channel) {
                        EXPECT_EQ(0, (p_response.data[1+(byte_num*3)] >> (j*2)) & 0b11);
                    }
                }
            }
        } else {
            // never happen, functions not checking for unit validity, it is in protocol
            ASSERT_EQ(ercChBadChannels, code);
        }
        free(p_response.data);
    }
}

TEST_F(FunctionsTest, TestReadSingleChannel) {
    // test every channel of each unit
//    for (int unit = 0; unit < 256; unit++) {
    for (int unit = 0; unit < UNITS_LENGTH; unit++) {
        for (uint8_t channel = 0; channel < 0x3F; channel++) {
            const uint8_t channelPerm = channel >> 4;
            for (uint8_t chType = 0; chType < 4; chType++) {
                #define MSG "TestReadSingleChannel unit " << static_cast<int>(unit) << \
                    ", channel " << static_cast<int>(channel) << " (" << static_cast<int>(channelPerm) << ", " << \
                    static_cast<int>(channel & 0x0F) << "), testing type " << static_cast<int>(chType) << "\n"
                this->request.length = 1;
                memset(this->request.data, 0, this->requestMaxSize);
                this->request.data[0] = ((chType << 6) & 0xFF) | channel;
                this->response.length = 0;
                memset(this->response.data, 0, this->responseMaxSize);
                uint8_t code = (*(this->getFunction(eFReadUnitChannels)))(unit & 0xFF, CONST_RQ, &this->response);

                int testIndex = unit * CHANNELS_PER_UNIT + channel;
                if (channel < CHANNELS_PER_UNIT && testChannels.count(testIndex)) {
                    // test channel exists
                    const sChannel ch = testChannels.at(testIndex);
                    if (1 != channelPerm) {
                        if (ch.dataType == chType) {
                            ASSERT_EQ(ercOk, code) << MSG << "code not OK";
                            ASSERT_GT(this->response.length, 1) << MSG << "response length less than 2";
                            ASSERT_EQ(((chType << 6) & 0xFF) | channel, this->response.data[0]) << MSG << "response channel type not as requested";
                            switch (ch.dataType) {
                            case eCDTBit: {
                                bool should_be = *(ch.value.tBit);
                                bool is = *(reinterpret_cast<bool*>(&(this->response.data[1])));
                                EXPECT_EQ(should_be, is)
                                        << MSG << "boolean value not equals";
                                break;
                            }
                            case eCDTSigned: {
                                EXPECT_EQ(*(ch.value.tInt), *(reinterpret_cast<int32_t*>(&(this->response.data[1]))))
                                        << MSG << "int32 value not equals";
                                break;
                            }
                            case eCDTUnsigned: {
                                EXPECT_EQ(*(ch.value.tUInt), *(reinterpret_cast<uint32_t*>(&(this->response.data[1]))))
                                        << MSG << "uint32 value not equals";
                                break;
                            }
                            }
                        } else {
                            EXPECT_EQ(ercChValidationFailed, code) << MSG << "code should be VALIDATION_FAILED";
                            EXPECT_EQ(0, this->response.length) << MSG << "response length should be 0 at VALIDATION FAILED";
                        }
                    } else {
                        EXPECT_EQ(ercChBadPermissions, code) << MSG << "code should be BAD PERMISSIONS";
                        EXPECT_EQ(0, this->response.length) << MSG << "response length should be 0 at BAD PERMISSIONS";
                    }
                } else {
                    if (UNITS_LENGTH > unit && 0x10 <= channel && 0x20 > channel) {
                        EXPECT_EQ(ercChBadPermissions, code) << MSG "code should be BAD PERMISSIONS";
                        EXPECT_EQ(0, this->response.length) << MSG << "response length should be 0 at BAD PERMISSIONS";
                    } else {
                        EXPECT_EQ(ercChBadChannels, code) << MSG "code should be BAD CHANNELS";
                        EXPECT_EQ(0, this->response.length) << MSG << "response length should be 0 at BAD CHANNELS";
                    }
                }
            }
        }
    }
}

} //
} // project
} // my
