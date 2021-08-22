#include "gtest/gtest.h"

extern "C" {
    #include "pgmspace.h"
}


int main(int argc, char **argv) {
  initialize();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
