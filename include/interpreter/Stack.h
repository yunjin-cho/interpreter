/// Yunjin Cho
/// This file creates the run time stack for the interpreter, it defines
/// a stack capable of holding different data types.

#ifndef INT_STACK_H
#define INT_STACK_H

#include <cstddef>
#include <vector>
#include <utility>

enum class Type : uint32_t {
    Char,
    Short,
    Int,
    Float
};

class Data {
    Type type;
    uint32_t data;

public:
    Data(Type type, uint32_t data = 0) : type(type), data(data) {}

    Type getType() { return type; }
    uint32_t getData() { return data; }
};

class Stack : public std::vector<Data> {
    using vec = std::vector<Data>;

public:
    /// Returns i from the back (0 indexed)
    Data& top(unsigned i) {
        return vec::at(vec::size() - 1 - i);
    }

    Data& top() {
        return top(0);
    }

    Data& pop() {
        Data& d = top();
        vec::pop_back();
        return d;
    }

    void push(Data&& data) {
        push_back(std::move(data));
    }
};

#endif // INT_STACK_H
