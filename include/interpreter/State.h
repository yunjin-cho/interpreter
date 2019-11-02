/// Yunjin Cho
/// This file defines the State class which carries state information of
/// the interpreter.

#ifndef INT_STATE_H
#define INT_STATE_H

#include "MappedFile.h"
#include "Stack.h"
#include <ostream>

struct State {
    Stack<Data> stack;

    std::stack<uint64_t> fpstack;

    MappedFile programText;
    uint64_t pc = 0;

    State() : programText(nullptr) {}
    State(const char *name) : programText(name) {}

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
