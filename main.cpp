// Yunjin Cho

#include "interpreter/State.h"
#include "interpreter/Instructions.h"

int main(int argc, char **argv) {
    State state(argv[1]);
    for (uint8_t ins; (ins = state.nextInstruction()) != Halt; ) {
        instructions[ins](state);
        std::cout << "=========\n";
        printState(state);
    }
    // pc gets increased from nextInstruction so it is one past after halt
    // so put it down one to output correctly.
    state.pc--;
    std::cout << '\n';
    halt(state);
}
