#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include "lcontrol/control.h"

using namespace lcomm;

namespace lcontrol {

    std::string control::int_to_string(int i) {
        std::stringstream out;
        out << i;
        return out.str();
    }
    std::string control::float_to_string(float i) {
        union {
            int as_int;
            float as_float;
        };
        as_float = i;
        std::stringstream out;
        out << as_int;
        return out.str();
    }


    // Ask the drone to send the navdata :
    // AT*CONFIG="seqNum",\"general:navdata_demo\",\"TRUE\"\r
    void control::sendNavData(int seqNum, ClientSocket& s) {
        std::string data("AT*CONFIG=");
        std::string seqStr = control::int_to_string(seqNum);
        data += seqStr + ",\"general:navdata_demo\",\"TRUE\"\r";
        std::cout << data << '\n';
        s.write(data);
    }

    // Reset the communication watchdog :
    // AT*COMWDG="seqNum"\r
    void control::watchdog(int seqNum, ClientSocket& s) {
        std::string data("AT*COMWD=");
        std::string seqStr = control::int_to_string(seqNum);
        data += seqStr + "\r";
        std::cout << data << '\n';
        s.write(data);
    }

    // Emergency mode on the drone :
    // AT*REF="seqNum",290717952\r
    void control::emergencyMode(int seqNum, ClientSocket& s) {
        std::string data("AT*REF=");
        std::string seqStr = control::int_to_string(seqNum);
        data += seqStr + ",290717952\r";
        std::cout << data << '\n';
        s.write(data);
    }

    // Calibrate the horizontal plan :
    // AT*FTRIM="seqNum"\r
    void control::horizontalPlan(int seqNum, ClientSocket& s) {
        std::string data("AT*FTRIM=");
        std::string seqStr = control::int_to_string(seqNum);
        data += seqStr + "\r";
        std::cout << data << '\n';
        s.write(data);
    }

    // Calibrate the magnetometer :
    // AT*CALIB="seqNum",0\r
    void control::magnetometer(int seqNum, ClientSocket& s) {
        std::string data("AT*CALIB=");
        std::string seqStr = control::int_to_string(seqNum);
        data += seqStr + ",0\r";
        std::cout << data << '\n';
        s.write(data);
    }

    // Take off :
    // AT*REF="seqNum",290718208\r
    void control::takeoff(int seqNum, ClientSocket& s) {
        std::string data("AT*REF=");
        std::string seqStr = control::int_to_string(seqNum);
        data += seqStr + ",290718208\r";
        std::cout << data << '\n';
        s.write(data);
    }

    // Land :
    // AT*REF="seqNum",290717696\r
    void control::land(int seqNum, ClientSocket& s) {
        std::string data("AT*REF=");
        std::string seqStr = control::int_to_string(seqNum);
        data += seqStr + ",290717696\r";
        std::cout << data << '\n';
        s.write(data);
    }

    // Send a movement to the drone :
    // AT*PCMD="seqNum","flag","leftRightTilt","frontBackTilt","verticalSpeed","angularSpeed",\r
    void control::movement(int seqNum, int flag, float frontBackTilt, float leftRightTilt, float verticalSpeed, float angularSpeed, ClientSocket& s) {
        std::string data("AT*PCMD=");
        std::string seqStr = control::int_to_string(seqNum);
        data += seqStr + "," + control::int_to_string(flag) + "," + control::float_to_string(leftRightTilt) + "," +
                control::float_to_string(frontBackTilt) + "," + control::float_to_string(verticalSpeed) + "," +
                control::float_to_string(angularSpeed) + ",\r";
        std::cout << data << '\n';
        s.write(data);
    }
}
