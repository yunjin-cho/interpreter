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

template <Type type>
void pushv(State& s) {
    uint32_t d = s.stack.at(s.fpstack.top() + s.stack.top().getData() + 1).getData();
    s.stack.pop();
    s.stack.push({type, d});
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

void popm(State& s) {
    uint32_t num = s.stack.pop().getData();
    s.stack.pop(num);
}

void popv(State& s) {
    s.stack.at(s.fpstack.top() + s.stack.top().getData() + 1) = s.stack.top(1);
    s.stack.pop(2);
}

void popa(State& s) {
    uint32_t data = s.stack.pop();
    std::stack<Data> stack;
    for (int i = 0; i < data; i++)
        stack.push(s.stack.pop());
    while (s.sp() != s.fpstack.top() - 1)
        s.stack.pop();
    for (; !stack.empty(); stack.pop())
        s.stack.push(std::move(stack.top()));

    #if 0
    for (int i = 0; i < s.stack.top().getData(); i++)
        s.stack.at(s.fpstack.top() + i + 1) =
          s.stack.at(s.sp() - s.stack.top() + i);
    s.stack.at(s.fpstack.top() + s.stack.top()) = s.stack.top(1);
    s.stack.resize(s.fpstack.top() + s.stack.top());
    #endif
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
    
    [Pushvc] = pushv<Type::Char>,
    [Pushvs] = pushv<Type::Short>,
    [Pushvi] = pushv<Type::Int>,
    [Pushvf] = pushv<Type::Float>,

    [Popm] = popm,
    [Popv] = popv,
    [Popa] = popa,

    [Add] = arithmetic<std::plus<Data>>,
    [Sub] = arithmetic<std::minus<Data>>,
    [Mul] = arithmetic<std::multiplies<Data>>,
    [Div] = arithmetic<std::divides<Data>>,

    [Printc] = print<char>,
    [Prints] = print<short>,
    [Printi] = print<int>,
    [Printf] = print<float>
};
