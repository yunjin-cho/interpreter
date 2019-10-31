/// Yunjin Cho
/// This file creates the run time stack for the interpreter, it defines
/// a stack capable of holding different data types.

#include <cstddef>

class Data {
    Type type;
    uint32_t data;

public:
    enum Type : uint32_t {
        Char,
        Short,
        Int,
        Float
    };

    Data(Type type, uint32_t data = 0) : type(type), data(data) {}
};

class Stack : public std::vector<Data> {
    using vec = std::vector<Data>;

public:
    Data& top() {
        return vec::back();
    }
};
