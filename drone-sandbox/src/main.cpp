#include "lcomm/lcomm.h"
#include "lcontrol/control.h"
#include <thread>

using namespace lcontrol;
using namespace lcomm;

using namespace std::literals;

int main() {

    int i = 0;
    ClientSocket s("127.0.0.1", 5556, false);

    for(int j = 0; j < 20; ++j) {
        control::horizontalPlan(i++, s);
        std::this_thread::sleep_for(50ms);
    }
    std::cout << "Stabilization OK!" << std::endl;

    std::this_thread::sleep_for(1s);

    std::cout << "Taking off..." << std::endl;
    for(int j = 0; j < 20; ++j) {
        control::takeoff(i++, s);
        std::this_thread::sleep_for(50ms);
    }
    std::cout << "Took Off!" << std::endl;

    std::this_thread::sleep_for(10s);

    std::cout << "Landing..." << std::endl;
    for(int j = 0; j < 20; ++j) {
        control::land(i++, s);
        std::this_thread::sleep_for(50ms);
    }
    std::cout << "Landed!" << std::endl;
    /*c.sendNavData(3);
    c.magnetometer(4);
    c.movement(5, 0, 6.1,5.0,4.8,3.4);*/
}
