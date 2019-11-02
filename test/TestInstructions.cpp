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

TEST_F(InsT, Pushc) {
    std::ofstream outfile("pushc.txt");
    outfile.put(45);
    outfile.close();

    State s("pushc.txt");
    instructions[Pushc](s);
    EXPECT_EQ(s.stack.top().getType(), Type::Char);
    EXPECT_EQ(s.stack.top().getData(), 45);
    remove("pushc.txt");
}

TEST_F(InsT, Pushs) {
    short srt = 10893;
    std::ofstream outfile("pushs.txt");
    outfile.put(reinterpret_cast<char*>(&srt)[0]);
    outfile.put(reinterpret_cast<char*>(&srt)[1]);
    outfile.close();

    State s("pushs.txt");
    instructions[Pushs](s);
    EXPECT_EQ(s.stack.top().getType(), Type::Short);
    EXPECT_EQ(s.stack.top().getData(), srt);
    remove("pushs.txt");
}

TEST_F(InsT, Pushi) {
    char arr[4] = {34, 76, 98, 127};
    std::ofstream outfile("pushi.txt");
    outfile.put(arr[0]);
    outfile.put(arr[1]);
    outfile.put(arr[2]);
    outfile.put(arr[3]);
    outfile.close();

    State s("pushi.txt");
    instructions[Pushi](s);
    EXPECT_EQ(s.stack.top().getType(), Type::Int);
    EXPECT_EQ(s.stack.top().getData(), *reinterpret_cast<int*>(arr));
    remove("pushi.txt");
}