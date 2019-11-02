/// Yunjin Cho

#include "interpreter/Instructions.h"
#include "interpreter/State.h"
#include <cstdlib>

void halt(State& s) {
    exit(0);
}

void cmpe(State& s) {
    Stack<Data>& stack = s.getStack();
    stack.push({Type::Int, stack.top(1) == stack.top()});
}

void cmplt(State& s) {
    Stack<Data>& stack = s.getStack();
    stack.push({Type::Int, stack.top(1) < stack.top()});
}

void cmpgt(State& s) {
    Stack<Data>& stack = s.getStack();
    stack.push({Type::Int, stack.top(1) > stack.top()});
}

InstructionHandler instructions[Last] = {
    [Halt] = halt,

    [Cmpe] = cmpe,
    [Cmplt] = cmplt,
    [Cmpgt] = cmpgt
};
