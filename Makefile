# Yunjin Cho

SRCS = $(wildcard src/*.cpp)
HEADERS = $(wildcard include/interpreter/*.h)

TEST_SRCS = $(wildcard test/*.cpp)
TEST_OBJS = $(TEST_SRCS:%.cpp=%.o)

CXXFLAGS = -Wall -Iinclude -std=c++14 -g

OBJS = $(SRCS:%.cpp=%.o)

all: interpreter

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

interpreter: $(OBJS) $(HEADERS) main.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) main.cpp -o interpreter

.gtest.out: $(OBJS) $(TEST_OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) /usr/local/lib/libgtest_main.a $(OBJS) $(TEST_OBJS) -lgtest -o .gtest.out

test: .gtest.out FORCE
	./.gtest.out

FORCE:

clean: FORCE
	rm -rf $(OBJS) $(TEST_OBJS) *.dSYM .gtest.out
