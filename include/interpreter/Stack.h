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

    Type getType() const { return type; }
    uint32_t getData() const { return data; }

    bool operator==(Data other) const {
        return other.type == type && other.data == data;
    }

    bool operator>(Data other) const {
        return other.type == type && data > other.data;
    }

    bool operator<(Data other) const {
        return other.type == type && data < other.data;
    }
};

template <typename T>
class Stack : public std::vector<T> {
    using vec = std::vector<T>;

public:
    /// Returns i from the back (0 indexed)
    T& top(unsigned i) {
        return vec::at(vec::size() - 1 - i);
    }

    T& top() {
        return top(0);
    }

    T& pop() {
        T& d = top();
        vec::pop_back();
        return d;
    }

    Stack<T>& push(T&& t) {
        vec::push_back(std::move(t));
        return *this;
    }
};

#endif // INT_STACK_H
