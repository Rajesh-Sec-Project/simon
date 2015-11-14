#include "lcomm/lcomm.h"
#include "lcontrol/control.h"
#include <thread>

using namespace lcontrol;
using namespace lcomm;

using namespace std::literals;

int main() {
    ClientSocket s("127.0.0.1", 5556, false);

    Control::enableStabilization();
    std::cout << "Stabilization OK!" << std::endl;

    std::this_thread::sleep_for(1s);

    std::cout << "Taking off..." << std::endl;
    Control::takeoff();
    std::cout << "Took Off!" << std::endl;

    std::this_thread::sleep_for(10s);

    std::cout << "Landing..." << std::endl;
    Control::land();
    std::cout << "Landed!" << std::endl;
    /*c.sendNavData(3);
    c.magnetometer(4);
    c.movement(5, 0, 6.1,5.0,4.8,3.4);*/
}
