/// Yunjin Cho

#include "interpreter/Instructions.h"
#include "interpreter/State.h"
#include <cstdlib>

void halt(State& s) {
    exit(0);
}

InstructionHandler instructions[Last] = {
    [Halt] = halt,
};
