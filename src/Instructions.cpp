/// Yunjin Cho

#include "interpreter/Instructions.h"
#include "interpreter/State.h"
#include <functional>
#include <cstdlib>
#include <iostream>

static void printStack(Stack<Data> stack, std::ostream& os) {
    if (stack.empty()) {
        os << "empty";
        return;
    }

    auto printData = [&](Data d) {
        if (d.getType() == Type::Float) {
            os << d.getFloat();
        } else {
            os << static_cast<uint32_t>(d);
        }
    };
    
    for (int i = 0; i < stack.size() - 1; i++) {
        printData({stack[i]});
        os << ' ';
    }
    printData(stack.back());
}

static void printStack(Stack<uint64_t> stack, std::ostream& os) {
    if (stack.empty()) {
        os << "empty";
        return;
    }

    for (int i = 0; i < stack.size() - 1; i++)
        os << stack[i] << ' ';
    os << stack.back();
}

void printState(State& s) {
    s.os() << "Compile values:\nPC: " << s.pc << "\nsp: " << s.sp()
      << "\nrstack: ";
    printStack(s.stack, s.os());
    s.os() << "\nfpsp: " << s.fpsp() << "\nfpstack: ";
    printStack(s.fpstack, s.os());
    s.os() << '\n';
}

void halt(State& s) {
    printState(s);
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
    Data d(Type::Float, 0);
    d.setFloat(f);
    s.stack.push(std::move(d));
}

template <Type type>
static void pushv(State& s) {
    uint32_t d = s.stack.at(s.fpstack.top() + s.stack.top().getData() + 1).getData();
    s.stack.pop();
    s.stack.push({type, d});
}

void pushvf(State& s) {
    if (s.stack.top().getType() == Type::Float) {
        float f = s.stack.at(s.fpstack.top() + s.stack.top().getData() + 1).getFloat();
        s.stack.pop();
        Data d(Type::Float, 0);
        d.setFloat(f);
        s.stack.push(std::move(d));
    } else {
        pushv<Type::Int>(s);
    }
}

template <typename fo, typename no>
static void arithmetic(State& s) {
    Stack<Data>& stack = s.getStack();
    Data right = stack.pop(), left = stack.pop();
    if (right.getType() == Type::Float || left.getType() == Type::Float) {
        stack.push(fo()(left.getFloat(), right.getFloat()));
    } else {
        stack.push({left.getType(), no()(left.getData(), right.getData())});
    }
}

template <typename type>
static void print(State& s) {
    uint32_t i = s.getStack().pop().getData();
    s.os() << *reinterpret_cast<type*>(&i) << '\n';
}

static void printf(State& s) {
    if (s.stack.top().getType() == Type::Float) {
        float f = s.stack.pop().getFloat();
        s.os() << f << '\n';
    } else {
        uint32_t i = s.getStack().pop().getData();
        s.os() << i << '\n';
    }
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
    s.fpstack.push(s.sp() - s.stack.pop() - 1);
    //s.stack.pop();
    s.pc = s.stack.pop().getData();
}

static void ret(State& s) {
    uint32_t new_pc = s.stack.top().getData();
    s.stack.resize(s.fpstack.pop());
    s.pc = new_pc;
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
    while (s.sp() != s.fpstack.top())
        s.stack.pop();
    for (; !stack.empty(); stack.pop())
        s.stack.push(std::move(stack.top()));
}

template <Type type>
static void peek(State& s) {
    s.stack.at(s.fpstack.top() + s.stack.top(1) + 1)
      = {type, s.stack.at(s.fpstack.top() + s.stack.top() + 1)};
    s.stack.pop(2);
}

template <Type type>
static void poke(State& s) {
    s.stack.at(s.fpstack.top() + s.stack.top() + 1)
      = {type, s.stack.at(s.fpstack.top() + s.stack.top(1) + 1)};
    s.stack.pop(2);
}

static void swp(State& s) {
    Data first = s.stack.pop();
    Data second = s.stack.pop();
    s.stack.push(std::move(first));
    s.stack.push(std::move(second));
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
    [Pushvf] = pushvf,

    [Popm] = popm,
    [Popv] = popv,
    [Popa] = popa,

    [Peekc] = peek<Type::Char>,
    [Peeks] = peek<Type::Short>,
    [Peeki] = peek<Type::Int>,
    [Peekf] = peek<Type::Float>,

    [Pokec] = poke<Type::Char>,
    [Pokes] = poke<Type::Short>,
    [Pokei] = poke<Type::Int>,
    [Pokef] = poke<Type::Float>,

    [Swp] = swp,

    [Add] = arithmetic<std::plus<float>, std::plus<uint32_t>>,
    [Sub] = arithmetic<std::minus<float>, std::minus<uint32_t>>,
    [Mul] = arithmetic<std::multiplies<float>, std::multiplies<uint32_t>>,
    [Div] = arithmetic<std::divides<float>, std::divides<uint32_t>>,

    [Printc] = print<char>,
    [Prints] = print<short>,
    [Printi] = print<int>,
    [Printf] = printf
};
