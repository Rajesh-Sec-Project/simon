#include "lcomm/lcomm.h"
#include "lcontrol/control.h"
using namespace lcontrol;
using namespace lcomm;

int main() {
	
	int i = 1;
	ClientSocket s( "127.0.0.1", 50001, 5);
	control c;
	c.takeoff(i, s);
	c.land(2, s) ;
	/*c.sendNavData(3);
	c.magnetometer(4);
	c.movement(5, 0, 6.1,5.0,4.8,3.4);*/
}
