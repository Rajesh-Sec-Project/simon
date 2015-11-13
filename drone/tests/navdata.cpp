#include <iostream>
#include "navdata.h"
#include "at.h"

int main() {
    int mask = (1 << navdata::option_altitude) | (1 << navdata::option_wifi);

    at::config cmd;
    cmd.seq = 1;
    cmd.key = "general:navdata_options";
    cmd.value = at::argument(mask);
    std::cout << cmd.toString() << std::endl;
}
