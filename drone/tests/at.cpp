#include <iostream>
#include "at.h"

//! This is a very simple test to check out the output of the at commands
//!   string generation.

static int seq = 1;

template <typename Command>
void command(Command const& cmd) {
    Command c = cmd;
    c.seq = seq++;
    std::cout << c.toString() << std::endl;
}

int main() {
    command(at::ref{true, false});
    command(at::ref{false, false});

    command(at::commwdg{});
    command(at::config{"key", "value"});

    for(int i = 0; i < 10; ++i) {
        float roll = (i - 5.0) / 5.0;
        command(at::pcmd{false, false, true, roll, 0, 0, 0});
    }

    return 0;
}
