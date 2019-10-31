/// Yunjin Cho

#include "interpreter/Instructions.h"
#include <fstream>
#include <gtest/gtest.h>

TEST(Instructions, Halt) {
    std::ofstream outfile("test.txt");
    outfile << "test\n";
    outfile.close();
    State s("test.txt");
    EXPECT_EXIT(instructions[Halt](s), ::testing::ExitedWithCode(0), "");
    remove("test.txt");
}
