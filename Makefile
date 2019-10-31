# Yunjin Cho

SRCS = $(wildcard src/*.cpp)
HEADERS = $(wildcard include/interpreter/*.h)

CXXFLAGS = -Wall

OBJS = $(SRCS:%.cpp=%.o)

.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@ -Iinclude

interpreter: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o interpreter

clean:
	rm -rf $(OBJS) *.dSYM
