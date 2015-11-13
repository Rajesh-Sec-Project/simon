#include "lcomm/lcomm.h"

using namespace lcomm;
namespace lcontrol {
	class control {
		public : 
		inline std::string int_to_string (int i) ;	
		inline std::string float_to_string(float i);
		void sendNavData(int seqNum, ClientSocket &s);
		void watchdog(int seqNum, ClientSocket &s);
		void emergencyMode(int seqNum, ClientSocket &s);
		void horizontalPlan(int seqNum, ClientSocket &s);	
		void magnetometer( int seqNum, ClientSocket &s);
		void takeoff(int seqNum, ClientSocket &s);
		void land(int seqNum, ClientSocket &s);	
		void movement(int seqNum, int flag, float frontBackTilt, float leftRightTilt, float verticalSpeed, float angularSpeed, ClientSocket &s);
	};

}
