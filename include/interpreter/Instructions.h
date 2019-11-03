/// Yunjin Cho

#ifndef INT_INSTRUCTIONS_H
#define INT_INSTRUCTIONS_H

#include "State.h"

void halt(State&);

enum Instructions {
    Halt = 0,

    Cmpe = 132,
    Cmplt = 136,
    Cmpgt = 140,

    Jmp = 36,
    Jmpc = 40,
    Call = 44,
    Ret = 48,

    Pushc = 68,
    Pushs = 69,
    Pushi = 70,
    Pushf = 71,
    Pushvc = 72,
    Pushvs = 73,
    Pushvi = 74,
    Pushvf = 75,
    Popm = 76,
    Popa = 77,
    Popv = 80,

    Peekc = 84,
    Peeks = 85,
    Peeki = 86,
    Peekf = 87,
    
    Pokec = 88,
    Pokes = 89,
    Pokei = 90,
    Pokef = 91,

    Swp = 94,

    Add = 100,
    Sub = 104,
    Mul = 108,
    Div = 112,

    Printc = 144,
    Prints = 145,
    Printi = 146,
    Printf = 147,

    Last
};

using InstructionHandler = void(*)(State&);

extern InstructionHandler instructions[Instructions::Last];

#endif // INT_INSTRUCTIONS_H
