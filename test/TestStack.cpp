/// Yunjin Cho

#include "interpreter/Stack.h"
#include <gtest/gtest.h>

TEST(Stack, PushPop) {
    Stack s;
    EXPECT_EQ(s.size(), 0);
    s.push({Type::Int, 0});
    EXPECT_EQ(s.size(), 1);
    s.push({Type::Float, static_cast<uint32_t>(0.0)});
    EXPECT_EQ(s.size(), 2);
    s.pop();
    EXPECT_EQ(s.size(), 1);
    s.pop();
    EXPECT_EQ(s.size(), 0);
}

TEST(Stack, Top) {
    Stack s;
    EXPECT_EQ(s.size(), 0);
    EXPECT_ANY_THROW(s.top());
    s.push({Type::Int, 0});
    s.push({Type::Int, 17});
    s.push({Type::Short, 100});
    EXPECT_ANY_THROW(s.top(3));
    EXPECT_EQ(s.top(2).getData(), 0);
    EXPECT_EQ(s.top().getData(), 100);
    EXPECT_EQ(s.top().getType(), Type::Short);
}
