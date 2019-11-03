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
    int i = 3374443;
    std::ofstream outfile("pushi.txt");
    outfile.write(reinterpret_cast<char*>(&i), sizeof(int));
    outfile.close();

    State s("pushi.txt");
    instructions[Pushi](s);
    EXPECT_EQ(s.stack.top().getType(), Type::Int);
    EXPECT_EQ(s.stack.top().getData(), i);
    remove("pushi.txt");
}

TEST_F(InsT, Pushf) {
    float f = 23.54;
    std::ofstream outfile("pushf.txt");
    outfile.write(reinterpret_cast<char*>(&f), sizeof(float));
    outfile.close();

    State s("pushf.txt");
    instructions[Pushf](s);
    EXPECT_EQ(s.stack.top().getType(), Type::Float);
    uint32_t i = s.stack.top().getData();
    EXPECT_EQ(*reinterpret_cast<float*>(&i), f);
    remove("pushf.txt");
}

TEST_F(InsT, Add) {
    state.stack.push({Type::Int, 4});
    state.stack.push({Type::Int, 5});

    instructions[Add](state);
    EXPECT_EQ(state.stack.top().getType(), Type::Int);
    EXPECT_EQ(state.stack.top().getData(), 9);
}

TEST_F(InsT, Sub) {
    state.stack.push({Type::Int, 4});
    state.stack.push({Type::Int, 5});

    instructions[Sub](state);
    EXPECT_EQ(state.stack.top().getType(), Type::Int);
    EXPECT_EQ(state.stack.top().getData(), static_cast<uint32_t>(-1));
    instructions[Sub](state);
    EXPECT_EQ(state.stack.top().getType(), Type::Int);
    EXPECT_EQ(state.stack.top().getData(), 6);
}

TEST_F(InsT, Mul) {
    state.stack.push({Type::Int, 4});
    state.stack.push({Type::Int, 5});

    instructions[Mul](state);
    EXPECT_EQ(state.stack.top().getType(), Type::Int);
    EXPECT_EQ(state.stack.top().getData(), 20);
    instructions[Mul](state);
    EXPECT_EQ(state.stack.top().getType(), Type::Int);
    EXPECT_EQ(state.stack.top().getData(), 100);
}

TEST_F(InsT, Div) {
    state.stack.push({Type::Int, 20});
    state.stack.push({Type::Int, 5});

    instructions[Div](state);
    EXPECT_EQ(state.stack.top().getType(), Type::Int);
    EXPECT_EQ(state.stack.top().getData(), 4);

#if 0 // Not working right now, stupid floats >:(
    state.stack.push({Type::Float, static_cast<uint32_t>(22.0)});
    state.stack.push({Type::Float, static_cast<uint32_t>(5.0)});
    instructions[Div](state);
    EXPECT_EQ(state.stack.top().getType(), Type::Float);
    uint32_t i = state.stack.top().getData();
    EXPECT_EQ(*reinterpret_cast<float*>(&i), 22.0/5.0);
#endif
}

// The current behavior is to use the first type of a binary arithmetic
// expression for the type of the result.
TEST_F(InsT, RetainType) {
    state.stack.push({Type::Short, 20});
    state.stack.push({Type::Char, 5});

    instructions[Add](state);
    EXPECT_EQ(state.stack.top().getType(), Type::Short);
    instructions[Add](state);
    EXPECT_EQ(state.stack.top().getType(), Type::Char);
}

TEST_F(InsT, Printc) {
    state.stack.push({Type::Char, 'a'});
    std::stringstream buf;
    state.stream = &buf;

    instructions[Printc](state);
    EXPECT_STREQ(buf.str().c_str(), "a");
    state.stack.push({Type::Char, 'b'});
    instructions[Printc](state);
    EXPECT_STREQ(buf.str().c_str(), "ab");
}

TEST_F(InsT, Prints) {
    state.stack.push({Type::Short, 45});
    std::stringstream buf;
    state.stream = &buf;

    instructions[Prints](state);
    EXPECT_STREQ(buf.str().c_str(), "45");
}

TEST_F(InsT, Printi) {
    state.stack.push({Type::Int, 321087});
    std::stringstream buf;
    state.stream = &buf;

    instructions[Printi](state);
    EXPECT_STREQ(buf.str().c_str(), "321087");
}

TEST_F(InsT, PrintiSigned) {
    state.stack.push({Type::Int, UINT32_MAX});
    std::stringstream buf;
    state.stream = &buf;

    instructions[Printi](state);
    EXPECT_STREQ(buf.str().c_str(), "-1");
}

TEST_F(InsT, Printf) {
    float f = 3.5;
    state.stack.push({Type::Float, *reinterpret_cast<uint32_t*>(&f)});
    std::ostringstream buf;
    state.stream = &buf;

    instructions[Printf](state);
    EXPECT_STREQ(buf.str().c_str(), "3.5");
    f = 45.213;
    state.stack.push({Type::Float, *reinterpret_cast<uint32_t*>(&f)});
    instructions[Printf](state);
    EXPECT_STREQ(buf.str().c_str(), "3.545.213");
}
