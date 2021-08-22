#include "gtest/gtest.h"
#include "RF model test.h"
#include "defines.h"
extern "C" {
    #include "nRF model.h"
    #include "../usb2nrf/RF model.h"
}
#include <exception>


namespace my {
namespace project {
namespace {


RFModelTest::RFModelTest()
{
//    reinitialize_memory();
    nRF_init_for_tests();
    rf_init();
}

RFModelTest::~RFModelTest()
{

}

TEST_F(RFModelTest, listen_test) {
    t_address a;
    for (int i = 0; i < MAC_SIZE; i++) a[i] = static_cast<uint8_t>(i);
    listenCalls = 0;
    RFListen(&a[0]);
    ASSERT_EQ(1, listenCalls) << "there should've been one call to nrf listen command in RFListen function";
    ASSERT_EQ(MAC_SIZE, lastListenAddress.length);
    for (int i = 0; i < MAC_SIZE; i++) {
        ASSERT_EQ(a[i], *(lastListenAddress.data+i))
            << "listen address in command to nrf does not match given to RFListen function";
    }
}

TEST_F(RFModelTest, switch_mode_test) {
    /// @todo test all possible modes
    ASSERT_EQ(rmIdle, switchRFMode(rmIdle));
}

TEST_F(RFModelTest, buffer_test) {
    switchRFMode(rmIdle);
    ASSERT_EQ(nullptr, nextRFBufferElement()) << "buffer shall be empty right after init";
    uint8_t addressBytes[MAC_SIZE];
    sString address = {0, &addressBytes[0]};
    for (int i = 0; i < 256; i++) {
        cNRF_TransmissionFailed(&address, nullptr);
        ASSERT_NE(nullptr, nextRFBufferElement())
                << i << "th iteration: "
                << "buffer shall not be empty right after putting there something";
        ASSERT_EQ(nullptr, nextRFBufferElement())
                << i << "th iteration: "
                << "buffer shall be empty after popping single element from it";
    }
    // now test multiple packets in buffer
    /// @fixme determine the buffer size by missed_packets counter
    for (int i = 0; i < 7; i++) cNRF_DataTransmitted(&address, nullptr);
    cNRF_TransmissionFailed(&address, nullptr);

    for (int i = 0; i < 7; i++) {
        tRfPacket *packet = nextRFBufferElement();
        ASSERT_NE(nullptr, packet) << i << "th packet in buffer is missing";
        ASSERT_EQ(eptAckOk, packet->type) << i << "th packet in buffer has wrong type";
    }
    tRfPacket *packet = nextRFBufferElement();
    ASSERT_NE(nullptr, packet) << "last packet in buffer is missing";
    ASSERT_EQ(eptAckTimeout, packet->type) << "last packet in buffer has wrong type";
    ASSERT_EQ(nullptr, nextRFBufferElement());
}



TEST_F(RFModelTest, data_received_callback_test_idle) {
    // prepare all for the tests
    switchRFMode(rmIdle);
    uint8_t addressBytes[MAC_SIZE];
    sString address;
    uint8_t payloadBytes[PAYLOAD_SIZE];
    sString payload;
    address.length = MAC_SIZE;
    address.data = &addressBytes[0];
    for (int i = 0; i < address.length; i++) address.data[i] = static_cast<uint8_t>(i);
    payload.length = 10;
    payload.data = &payloadBytes[0];
    for (int i = 0; i < payload.length; i++) payload.data[i] = static_cast<uint8_t>(i);
    // now testing data received callback
    cNRF_DataReceived(&address, &payload);
    tRfPacket *receive = nextRFBufferElement();
    ASSERT_NE(nullptr, receive) << "after receiving eptData packet buffer should not be empty";
    ASSERT_EQ(eptData, receive->type);
    for (int i = 0; i < address.length; i++) ASSERT_EQ(address.data[i], receive->address[i])
            << i << "th byte: "
            << "eptData packet address in buffer does not match address given to the callback";
    ASSERT_EQ(payload.length, receive->payloadLength) << "packet payload length in buffer is wrong";
    for (int i = 0; i < payload.length; i++) ASSERT_EQ(payload.data[i], receive->payloadData[i])
            << i << "th byte: "
            << "eptData packet payload in buffer does not match payload, given to the callback";
    // now testing data sent callback
    cNRF_DataTransmitted(&address, nullptr);
    receive = nextRFBufferElement();
    ASSERT_NE(nullptr, receive) << "after receiving eptAckOk packet buffer should not be empty";
    ASSERT_EQ(eptAckOk, receive->type);
    for (int i = 0; i < address.length; i++) ASSERT_EQ(address.data[i], receive->address[i])
            << i << "th byte: "
            << "eptAckOk packet address in buffer does not match address given to the callback";
    // and now ack timeout
    cNRF_TransmissionFailed(&address, nullptr);
    receive = nextRFBufferElement();
    ASSERT_NE(nullptr, receive) << "after receiving eptAckTimeout packet buffer should not be empty";
    ASSERT_EQ(eptAckTimeout, receive->type);
    for (int i = 0; i < address.length; i++) ASSERT_EQ(address.data[i], receive->address[i])
            << i << "th byte: "
            << "eptAckTimeout packet address in buffer does not match address given to the callback";
}

}}}
