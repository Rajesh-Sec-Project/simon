#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include "lcontrol/control.h"
#include <thread>

using namespace std::literals;
using namespace lcomm;

#define NUM_ITER 20
#define SLEEP_DELAY 20ms

// Define to 1 if you want the frames sent to the drone to be printed on stdout.
#define PRINT_FRAMES 0

namespace lcontrol {

    namespace {
        void printFrame(std::string const& f) {
#if PRINT_FRAMES
            std::cout << f << '\n';
#endif
        }
    }

    std::atomic<std::uint32_t> Control::m_seqNum;
    std::unique_ptr<lcomm::ClientSocket> Control::m_sock;

    std::string Control::float_to_string(float i) {
        return std::to_string(*reinterpret_cast<std::uint32_t const*>(&i));
    }

    void Control::init() {
        m_seqNum = 0;
        m_sock = std::make_unique<ClientSocket>("127.0.0.1", 5556, false);
        int numAttempt = 0;
        while(!m_sock->opened()) {
            std::cout << "Waiting for lcontrol's connection " << numAttempt++ << "..." << std::endl;
            std::this_thread::sleep_for(100ms);
        }
        std::cout << "lcontrol connected!" << std::endl;
    }

    void Control::enableStabilization() {
        for(int i = 0; i < NUM_ITER; ++i) {
            Control::horizontalPlan(m_seqNum.fetch_add(1), *m_sock);
            std::this_thread::sleep_for(SLEEP_DELAY);
        }
    }

    void Control::takeoff() {
        for(int i = 0; i < NUM_ITER; ++i) {
            Control::takeoff(m_seqNum.fetch_add(1), *m_sock);
            std::this_thread::sleep_for(SLEEP_DELAY);
        }
    }

    void Control::land() {
        for(int i = 0; i < NUM_ITER; ++i) {
            Control::land(m_seqNum.fetch_add(1), *m_sock);
            std::this_thread::sleep_for(SLEEP_DELAY);
        }
    }

    void Control::config(std::string const& key, std::string const& value) {
        Control::config(m_seqNum.fetch_add(1), key, value, *m_sock);
    }

    void Control::ackControl() {
        Control::ackControl(m_seqNum.fetch_add(1), *m_sock);
    }

    void Control::getCfgControl() {
        Control::getCfgControl(m_seqNum.fetch_add(1), *m_sock);
    }

    // Ask the drone to send the navdata :
    // AT*CONFIG="seqNum",\"general:navdata_demo\",\"TRUE\"\r
    void Control::sendNavData(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*CONFIG=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",\"general:navdata_demo\",\"TRUE\"\r";
        printFrame(data);
        s.write(data);
    }

    // Reset the communication watchdog :
    // AT*COMWDG="seqNum"\r
    void Control::watchdog(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*COMWD=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + "\r";
        printFrame(data);
        s.write(data);
    }

    // Emergency mode on the drone :
    // AT*REF="seqNum",290717952\r
    void Control::emergencyMode(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*REF=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",290717952\r";
        printFrame(data);
        s.write(data);
    }

    // Calibrate the horizontal plan :
    // AT*FTRIM="seqNum"\r
    void Control::horizontalPlan(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*FTRIM=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + "\r";
        printFrame(data);
        s.write(data);
    }

    // Calibrate the magnetometer :
    // AT*CALIB="seqNum",0\r
    void Control::magnetometer(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*CALIB=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",0\r";
        printFrame(data);
        s.write(data);
    }

    // Take off :
    // AT*REF="seqNum",290718208\r
    void Control::takeoff(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*REF=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",290718208\r";
        printFrame(data);
        s.write(data);
    }

    // Land :
    // AT*REF="seqNum",290717696\r
    void Control::land(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*REF=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",290717696\r";
        printFrame(data);
        s.write(data);
    }

    // Send a movement to the drone :
    // AT*PCMD="seqNum","flag","leftRightTilt","frontBackTilt","verticalSpeed","angularSpeed",\r
    void Control::movement(std::uint32_t seqNum, int flag, float frontBackTilt, float leftRightTilt, float verticalSpeed, float angularSpeed, ClientSocket& s) {
        std::string data("AT*PCMD=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + "," + std::to_string(flag) + "," + Control::float_to_string(leftRightTilt) + "," +
                Control::float_to_string(frontBackTilt) + "," + Control::float_to_string(verticalSpeed) + "," +
                Control::float_to_string(angularSpeed) + ",\r";
        printFrame(data);
        s.write(data);
    }

    void Control::config(std::uint32_t seqNum, std::string const& key, std::string const& value, ClientSocket& s) {
        std::string data("AT*CONFIG=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",\"" + key + "\",\"" + value + "\"\r";
        printFrame(data);
        s.write(data);
    }

    // Send a control command ack, this resets the command_ack bit
    //   in the drone's state bitfield
    void Control::ackControl(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*CTRL=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",5\r";
        printFrame(data);
        s.write(data);
    }

    // Get the drone's configuration dumped to the control port
    //   (TCP 5559)
    void Control::getCfgControl(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*CTRL=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",4\r";
        printFrame(data);
        s.write(data);
    }
}
