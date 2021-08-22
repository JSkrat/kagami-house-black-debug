#ifndef RFMODELTEST_H
#define RFMODELTEST_H
#include "gtest/gtest.h"
extern "C" {
    #include "../usb2nrf/RF model.h"
}


namespace my {
namespace project {
namespace {


class RFModelTest : public ::testing::Test
{
public:
    RFModelTest();
    ~RFModelTest() override;

//    fUARTFunction getFunctionPointer(eModemCommand command);
};

} //
} // project
} // my

#endif // RFMODELTEST_H
