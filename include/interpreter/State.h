/// Yunjin Cho
/// This file defines the State class which carries state information of
/// the interpreter.

#include "MappedFile.h"
#include "Stack.h"
#include <ostream>

class State {
    Stack stack;

    MappedFile programText;
    uint64_t pc = 0;

public:

    int64_t getSP() const {
        return stack.size();
    }

    uint64_t getPC() const {
        return pc;
    }
};
