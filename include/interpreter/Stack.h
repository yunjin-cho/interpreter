/// Yunjin Cho
/// This file creates the run time stack for the interpreter, it defines
/// a stack capable of holding different data types.

#ifndef INT_STACK_H
#define INT_STACK_H

#include <cstddef>
#include <vector>
#include <utility>
#include <stdexcept>

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
    Data(uint32_t data = 0) : type(Type::Int), data(data) {}
    Data(Type type, uint32_t data = 0) : type(type), data(data) {}

    Type getType() const { return type; }
    uint32_t getData() const { return data; }

    bool operator==(Data other) const {
        return other.data == data;
    }

    bool operator>(Data other) const {
        return data > other.data;
    }

    bool operator<(Data other) const {
        return data < other.data;
    }

    Data operator+(Data other) const {
        return {type, data + other.data};
    }

    Data operator-(Data other) const {
        return {type, data - other.data};
    }

    Data operator*(Data other) const {
        return {type, data * other.data};
    }

    Data operator/(Data other) const {
        return {type, data / other.data};
    }
};

template <typename T>
class Stack : public std::vector<T> {
    using vec = std::vector<T>;

public:
    /// Returns i from the back (0 indexed)
    T& top(unsigned i = 0) {
        return vec::at(vec::size() - 1 - i);
    }

    T& pop() {
        T& d = top();
        vec::pop_back();
        return d;
    }

    void pop(uint32_t count) {
        for (int i = 0; i < count; i++)
            vec::pop_back();
    }

    Stack<T>& push(T&& t) {
        vec::push_back(std::move(t));
        return *this;
    }

    void resize(uint64_t new_size) {
        if (new_size >= vec::size())
            throw std::invalid_argument("Cannot resize to be larger");
        vec::resize(new_size);
    }
};

#endif // INT_STACK_H
