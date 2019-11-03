/// Yunjin Cho
/// This file defines the State class which carries state information of
/// the interpreter.

#ifndef INT_STATE_H
#define INT_STATE_H

#include "MappedFile.h"
#include "Stack.h"
#include <stack>
#include <iostream>

struct State {
    Stack<Data> stack;

    Stack<uint64_t> fpstack;

    MappedFile programText;
    int64_t pc = 0;

    std::ostream* stream = &std::cout;

    State() : programText(nullptr) {}
    State(const char *name) : programText(name) {}

    std::ostream& os() {
        return *stream;
    }

    int64_t sp() {
        return stack.size() - 1;
    }

    int64_t fpsp() {
        return fpstack.size() - 1;
    }

    Stack<Data>& getStack() {
        return stack;
    }

    const char *text() {
        return programText;
    }

    uint8_t nextInstruction() {
        return programText[pc++];
    }
};

#endif // INT_STATE_H
