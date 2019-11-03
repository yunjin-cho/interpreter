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
    Data right = stack.pop(), left = stack.pop();
    stack.push({Type::Int, comparator()(left, right)});
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
    Data right = stack.pop(), left = stack.pop();
    stack.push(operation()(left, right));
}

template <typename type>
void print(State& s) {
    uint32_t i = s.getStack().top().getData();
    s.os() << *reinterpret_cast<type*>(&i);
}

void jmp(State& s) {
    s.pc = s.stack.pop().getData();
}

void jmpc(State& s) {
    if (s.stack.top(1).getData())
        s.pc = s.stack.top().getData();
    s.stack.pop(2);
}

void call(State& s) {
    s.fpstack.push(s.sp() - s.stack.top().getData() - 1);
    s.stack.pop();
    s.pc = s.stack.pop().getData();
}

void ret(State& s) {
    s.stack.resize(s.fpstack.pop());
    s.pc = s.stack.pop().getData();
}

InstructionHandler instructions[Last] = {
    [Halt] = halt,

    [Cmpe] = cmp<eq<Data>>,
    [Cmplt] = cmp<std::less<Data>>,
    [Cmpgt] = cmp<std::greater<Data>>,

    [Jmp] = jmp,
    [Jmpc] = jmpc,
    [Call] = call,
    [Ret] = ret,

    [Pushc] = push<uint8_t, Type::Char>,
    [Pushs] = push<uint16_t, Type::Short>,
    [Pushi] = push<uint32_t, Type::Int>,
    [Pushf] = pushf,

    [Add] = arithmetic<std::plus<Data>>,
    [Sub] = arithmetic<std::minus<Data>>,
    [Mul] = arithmetic<std::multiplies<Data>>,
    [Div] = arithmetic<std::divides<Data>>,

    [Printc] = print<char>,
    [Prints] = print<short>,
    [Printi] = print<int>,
    [Printf] = print<float>
};
