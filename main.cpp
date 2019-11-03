// Yunjin Cho

#include "interpreter/State.h"
#include "interpreter/Instructions.h"

int main(int argc, char **argv) {
    State state(argv[1]);
    for (uint8_t ins; (ins = state.nextInstruction()) != Halt; ) {
        instructions[ins](state);

    }
    state.pc--;
    state.os() << '\n';
    halt(state);
}
