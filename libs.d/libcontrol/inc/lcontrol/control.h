#include "lcomm/lcomm.h"

namespace lcontrol {
	class control {
		public :
		static std::string int_to_string (int i) ;
		static std::string float_to_string(float i);

		static void sendNavData(int seqNum, lcomm::ClientSocket &s);
		static void watchdog(int seqNum, lcomm::ClientSocket &s);
		static void emergencyMode(int seqNum, lcomm::ClientSocket &s);
		static void horizontalPlan(int seqNum, lcomm::ClientSocket &s);
		static void magnetometer( int seqNum, lcomm::ClientSocket &s);
		static void takeoff(int seqNum, lcomm::ClientSocket &s);
		static void land(int seqNum, lcomm::ClientSocket &s);
		static void movement(int seqNum, int flag, float frontBackTilt, float leftRightTilt, float verticalSpeed, float angularSpeed, lcomm::ClientSocket &s);
	};

}
