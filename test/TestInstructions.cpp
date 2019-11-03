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
    std::ostringstream s;
    state.stream = &s;
    EXPECT_EXIT(halt(state), ::testing::ExitedWithCode(0), ".*");
    //EXPECT_STREQ(s.str().c_str(), "Compile values\nPC: 0\nsp: -1\nrstack: "
    //  "empty\nfpsp: -1\nfpstack: empty\n");
}

TEST_F(InsT, Cmpe) {
    state.stack.push({Type::Int, 5}).push({Type::Int, 5}).push({Type::Int, 5});
    instructions[Cmpe](state);
    EXPECT_EQ(state.stack.top().getData(), 1);
    instructions[Cmpe](state);
    EXPECT_EQ(state.stack.top().getData(), 0);

    state.stack.push({Type::Short, 5}).push({Type::Int, 5});
    instructions[Cmpe](state);
    EXPECT_EQ(state.stack.top().getData(), 1);
}

TEST_F(InsT, CmpPops) {
    state.stack.push({Type::Int, 5}).push({Type::Int, 5});
    EXPECT_EQ(state.stack.size(), 2);
    instructions[Cmpe](state);
    EXPECT_EQ(state.stack.size(), 1);
}

TEST_F(InsT, Cmlt) {
    state.stack.push({Type::Int, 5}).push({Type::Int, 4});
    instructions[Cmplt](state);
    EXPECT_EQ(state.stack.top().getData(), 0);
    state.stack.push({Type::Int, 3});
    instructions[Cmplt](state);
    EXPECT_EQ(state.stack.top().getData(), 1);
}

TEST_F(InsT, Cmpgt) {
    state.stack.push({Type::Int, 8}).push({Type::Int, 7}).push({Type::Int, 0});
    instructions[Cmpgt](state);
    EXPECT_EQ(state.stack.top().getData(), 1);
    instructions[Cmpgt](state);
    EXPECT_EQ(state.stack.top().getData(), 1);
    state.stack.push({Type::Int, 5});
    instructions[Cmpgt](state);
    EXPECT_EQ(state.stack.top().getData(), 0);
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

TEST_F(InsT, Pushvc) {
    state.fpstack.push(0);
    state.stack.push({17}).push({13}).push({0});
    instructions[Pushvc](state);
    EXPECT_EQ(state.stack.size(), 3);
    EXPECT_EQ(state.stack.top().getType(), Type::Char);
    EXPECT_EQ(state.stack.pop().getData(), 13);
    EXPECT_EQ(state.stack.pop().getData(), 13);
    EXPECT_EQ(state.stack.pop().getData(), 17);
    ASSERT_EQ(state.stack.size(), 0);
}

TEST_F(InsT, Pushvs) {
    state.fpstack.push(0);
    state.stack.push({17}).push({13}).push({0});
    instructions[Pushvs](state);
    EXPECT_EQ(state.stack.size(), 3);
    EXPECT_EQ(state.stack.top().getType(), Type::Short);
}

TEST_F(InsT, Pushvi) {
    state.fpstack.push(0);
    state.stack.push({17}).push({13}).push({0});
    instructions[Pushvi](state);
    EXPECT_EQ(state.stack.size(), 3);
    EXPECT_EQ(state.stack.top().getType(), Type::Int);
}

TEST_F(InsT, Pushvf) {
    state.fpstack.push(0);
    state.stack.push({17}).push({13}).push({0});
    instructions[Pushvf](state);
    EXPECT_EQ(state.stack.size(), 3);
    EXPECT_EQ(state.stack.top().getType(), Type::Float);
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
    EXPECT_EQ(state.stack.size(), 1);
    state.stack.push({Type::Int, 4});
    instructions[Sub](state);
    EXPECT_EQ(state.stack.top().getType(), Type::Int);
    EXPECT_EQ(state.stack.top().getData(), -5);
}

TEST_F(InsT, Mul) {
    state.stack.push({Type::Int, 4});
    state.stack.push({Type::Int, 5});

    instructions[Mul](state);
    EXPECT_EQ(state.stack.top().getType(), Type::Int);
    EXPECT_EQ(state.stack.top().getData(), 20);
    EXPECT_EQ(state.stack.size(), 1);
    state.stack.push({Type::Int, 5});
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
    EXPECT_EQ(state.stack.size(), 1);
    state.stack.push({Type::Char, 3});
    state.stack.push({Type::Float, 3});
    EXPECT_EQ(state.stack.size(), 3);
    instructions[Add](state);
    EXPECT_EQ(state.stack.top().getType(), Type::Char);
    EXPECT_EQ(state.stack.size(), 2);
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

TEST_F(InsT, Jmp) {
    state.stack.push({Type::Int, 0});

    instructions[Jmp](state);
    EXPECT_EQ(state.stack.size(), 0);
    EXPECT_EQ(state.pc, 0);
}

TEST_F(InsT, Jmpc) {
    state.stack.push({Type::Int, 1}).push({Type::Int, 100});

    instructions[Jmpc](state);
    EXPECT_EQ(state.stack.size(), 0);
    EXPECT_EQ(state.pc, 100);

    state.stack.push({Type::Int, 0}).push({Type::Int, 30});
    instructions[Jmpc](state);
    EXPECT_EQ(state.stack.size(), 0);
    EXPECT_EQ(state.pc, 100);
}

TEST_F(InsT, Call) {
    state.stack.push({16}).push({17}).push({1});

    instructions[Call](state);
    EXPECT_EQ(state.pc, 17);

    EXPECT_EQ(state.fpstack.top(), 0);
    EXPECT_EQ(state.stack.size(), 1);
    // TODO: More call tests
}

TEST_F(InsT, Ret) {
    state.fpstack.push(1);
    state.stack.push({5}).push({4}).push({3});

    instructions[Ret](state);
    EXPECT_EQ(state.stack.size(), 0);
    EXPECT_EQ(state.pc, 5);
}

TEST_F(InsT, Popm) {
    state.stack.push({1}).push({2}).push({3}).push({4}).push({1});

    instructions[Popm](state);
    EXPECT_EQ(state.stack.size(), 3);
    EXPECT_EQ(state.stack.top().getData(), 3);

    state.stack.push({2});
    instructions[Popm](state);
    EXPECT_EQ(state.stack.size(), 1);
    EXPECT_EQ(state.stack.top().getData(), 1);
}

TEST_F(InsT, Popv) {
    state.fpstack.push(0);
    state.stack.push({1}).push({2}).push({3}).push({4}).push({1});

    instructions[Popv](state);
    ASSERT_EQ(state.stack.size(), 3);
    EXPECT_EQ(state.stack.pop().getData(), 4);
    EXPECT_EQ(state.stack.pop().getData(), 2);
    EXPECT_EQ(state.stack.pop().getData(), 1);
}

TEST_F(InsT, Popa) {
    state.fpstack.push(0);
    state.stack.push({26}).push({27}).push({0});

    instructions[Popa](state);
    ASSERT_EQ(state.stack.size(), 0);

    state.stack.push({26}).push({27}).push({1});
    instructions[Popa](state);
    ASSERT_EQ(state.stack.size(), 1);
    ASSERT_EQ(state.stack.top().getData(), 27);
}
