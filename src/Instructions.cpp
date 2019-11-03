/// Yunjin Cho

#include "interpreter/Instructions.h"
#include "interpreter/State.h"
#include <functional>
#include <cstdlib>

void halt(State& s) {
    exit(0);
}

template <typename T>
struct eq {
    bool operator()(const T& a, const T& b) {
        return a == b;
    }
};

template <class comparator>
void cmp(State& s) {
    Stack<Data>& stack = s.getStack();
    stack.push({Type::Int, comparator()(stack.top(1), stack.top())});
}

template <typename IntType, Type type>
void push(State& s) {
    IntType i = *reinterpret_cast<const IntType*>(s.text() + s.pc);
    s.pc += sizeof(IntType);
    s.stack.push({type, static_cast<uint32_t>(i)});
}

void pushf(State& s) {
    float f = *reinterpret_cast<const float*>(s.text() + s.pc);
    s.pc += sizeof(float);
    s.stack.push({Type::Float, *reinterpret_cast<uint32_t*>(&f)});
}

template <typename operation>
void arithmetic(State& s) {
    Stack<Data>& stack = s.getStack();
    stack.push(operation()(stack.top(1), stack.top()));
}

InstructionHandler instructions[Last] = {
    [Halt] = halt,

    [Cmpe] = cmp<eq<Data>>,
    [Cmplt] = cmp<std::less<Data>>,
    [Cmpgt] = cmp<std::greater<Data>>,

    [Pushc] = push<uint8_t, Type::Char>,
    [Pushs] = push<uint16_t, Type::Short>,
    [Pushi] = push<uint32_t, Type::Int>,
    [Pushf] = pushf,

    [Add] = arithmetic<std::plus<Data>>,
    [Sub] = arithmetic<std::minus<Data>>,
    [Mul] = arithmetic<std::multiplies<Data>>,
    [Div] = arithmetic<std::divides<Data>>,
};
