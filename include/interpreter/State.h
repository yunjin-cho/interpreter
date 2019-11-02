/// Yunjin Cho
/// This file defines the State class which carries state information of
/// the interpreter.

#ifndef INT_STATE_H
#define INT_STATE_H

#include "MappedFile.h"
#include "Stack.h"
#include <ostream>

class State {
    Stack<Data> stack;
    int64_t sp = -1;

    MappedFile programText;
    uint64_t pc = 0;

public:
    State() : programText(nullptr) {}
    State(const char *name) : programText(name) {}

    Stack<Data>& getStack() {
        return stack;
    }

    int64_t getSP() const {
        return sp;
    }

    uint64_t getPC() const {
        return pc;
    }

    uint8_t nextInstruction() {
        return programText[pc++];
    }
};

#endif // INT_STATE_H
