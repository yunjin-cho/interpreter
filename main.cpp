// Yunjin Cho

#include "interpreter/State.h"
#include "interpreter/Instructions.h"

int main(int argc, char **argv) {
    State s(argv[1]);
    for (uint8_t ins; (ins = s.nextInstruction()) != Halt; )
        instructions[ins](s);
}
