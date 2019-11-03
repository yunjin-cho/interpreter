/// Yunjin Cho

#include "interpreter/Instructions.h"
#include "interpreter/State.h"
#include <functional>
#include <cstdlib>
#include <iostream>

template <typename StackT>
static void printStack(StackT stack, std::ostream& os) {
    if (stack.empty()) {
        os << "empty";
        return;
    }
    
    // Don't use reference, the Stack<T>'s use small T's.
    auto last = stack.end()--;
    for (auto i = stack.begin(); i != last; i++)
        os << static_cast<uint32_t>(*i) << ' ';
    os << static_cast<uint32_t>(*last);
}

void halt(State& s) {
    s.os() << "Compile values:\nPC: " << s.pc << "\nsp: " << s.sp()
      << "\nrstack: ";
    printStack(s.stack, s.os());
    s.os() << "\nfpsp: " << s.fpsp() << "\nfpstack: ";
    printStack(s.fpstack, s.os());
    s.os() << '\n';
    std::exit(0);
}

static void bad_halt(State&) {
    throw std::invalid_argument("Should not call halt from instruction table");
}

template <typename T>
struct eq {
    bool operator()(const T& a, const T& b) {
        return a == b;
    }
};

template <class comparator>
static void cmp(State& s) {
    Stack<Data>& stack = s.getStack();
    Data right = stack.pop(), left = stack.pop();
    stack.push({Type::Int, comparator()(left, right)});
}

template <typename IntType, Type type>
static void push(State& s) {
    IntType i = *reinterpret_cast<const IntType*>(s.text() + s.pc);
    s.pc += sizeof(IntType);
    s.stack.push({type, static_cast<uint32_t>(i)});
}

static void pushf(State& s) {
    float f = *reinterpret_cast<const float*>(s.text() + s.pc);
    s.pc += sizeof(float);
    s.stack.push({Type::Float, *reinterpret_cast<uint32_t*>(&f)});
}

template <Type type>
static void pushv(State& s) {
    uint32_t d = s.stack.at(s.fpstack.top() + s.stack.top().getData() + 1).getData();
    s.stack.pop();
    s.stack.push({type, d});
}

template <typename operation>
static void arithmetic(State& s) {
    Stack<Data>& stack = s.getStack();
    Data right = stack.pop(), left = stack.pop();
    stack.push(operation()(left, right));
}

template <typename type>
static void print(State& s) {
    uint32_t i = s.getStack().top().getData();
    s.os() << *reinterpret_cast<type*>(&i);
}

static void jmp(State& s) {
    s.pc = s.stack.pop().getData();
}

static void jmpc(State& s) {
    if (s.stack.top(1).getData())
        s.pc = s.stack.top().getData();
    s.stack.pop(2);
}

static void call(State& s) {
    s.fpstack.push(s.sp() - s.stack.top().getData() - 1);
    s.stack.pop();
    s.pc = s.stack.pop().getData();
}

static void ret(State& s) {
    s.stack.resize(s.fpstack.pop());
    s.pc = s.stack.pop().getData();
}

static void popm(State& s) {
    uint32_t num = s.stack.pop().getData();
    s.stack.pop(num);
}

static void popv(State& s) {
    s.stack.at(s.fpstack.top() + s.stack.top().getData() + 1) = s.stack.top(1);
    s.stack.pop(2);
}

static void popa(State& s) {
    uint32_t data = s.stack.pop();
    std::stack<Data> stack;
    for (int i = 0; i < data; i++)
        stack.push(s.stack.pop());
    while (s.sp() != s.fpstack.top() - 1)
        s.stack.pop();
    for (; !stack.empty(); stack.pop())
        s.stack.push(std::move(stack.top()));
}

static void peek(State& s) {
    s.stack.at(s.fpstack.top() + s.stack.top(1) + 1)
      = s.stack.at(s.fpstack.top() + s.stack.top() + 1);
    s.stack.pop(2);
}

static void poke(State& s) {
    s.stack.at(s.fpstack.top() + s.stack.top() + 1)
      = s.stack.at(s.fpstack.top() + s.stack.top(1) + 1);
    s.stack.pop(2);
}

InstructionHandler instructions[Last] = {
    [Halt] = bad_halt,

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

    [Peekc] = peek,
    [Peeks] = peek,
    [Peeki] = peek,
    [Peekf] = peek,

    [Pokec] = poke,
    [Pokes] = poke,
    [Pokei] = poke,
    [Pokef] = poke,

    [Add] = arithmetic<std::plus<Data>>,
    [Sub] = arithmetic<std::minus<Data>>,
    [Mul] = arithmetic<std::multiplies<Data>>,
    [Div] = arithmetic<std::divides<Data>>,

    [Printc] = print<char>,
    [Prints] = print<short>,
    [Printi] = print<int>,
    [Printf] = print<float>
};
