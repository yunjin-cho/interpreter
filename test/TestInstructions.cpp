/// Yunjin Cho

#include "interpreter/Instructions.h"
#include <fstream>
#include <gtest/gtest.h>

class InsT : public ::testing::Test {
    void SetUp() override {
        std::ofstream outfile("test.txt");
        outfile << "test\n";
        outfile.close();
    
        state = {"test.txt"};
    }

    void TearDown() override {
        remove("test.txt");
    }

public:
    State state;
};

TEST_F(InsT, Halt) {
    EXPECT_EXIT(instructions[Halt](state), ::testing::ExitedWithCode(0), "");
}

TEST_F(InsT, Cmpe) {
    state.getStack().push({Type::Int, 5}).push({Type::Int, 5});
    instructions[Cmpe](state);
    EXPECT_EQ(state.getStack().top().getData(), 1);
    instructions[Cmpe](state);
    EXPECT_EQ(state.getStack().top().getData(), 0);
}

TEST_F(InsT, Cmlt) {
    state.getStack().push({Type::Int, 5}).push({Type::Int, 4});
    instructions[Cmplt](state);
    EXPECT_EQ(state.getStack().top().getData(), 0);
    state.getStack().push({Type::Int, 3});
    instructions[Cmplt](state);
    EXPECT_EQ(state.getStack().top().getData(), 1);
}

TEST_F(InsT, Cmpgt) {
    state.getStack().push({Type::Int, 6}).push({Type::Int, 5});
    instructions[Cmpgt](state);
    EXPECT_EQ(state.getStack().top().getData(), 1);
    instructions[Cmpgt](state);
    EXPECT_EQ(state.getStack().top().getData(), 1);
    instructions[Cmpgt](state);
    EXPECT_EQ(state.getStack().top().getData(), 0);
}
