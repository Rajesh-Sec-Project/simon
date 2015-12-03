#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <iomanip>
#include "lcontrol/control.h"

using namespace std::literals;
using namespace lcomm;

#define NUM_ITER 20
#define SLEEP_DELAY 20ms

// Define to 1 if you want the frames sent to the drone to be printed on stdout.
#define PRINT_FRAMES 0

namespace lcontrol {
    std::atomic<std::uint32_t> Control::m_seqNum;
    std::unique_ptr<lcomm::ClientSocket> Control::m_sock;

    std::string Control::float_to_string(float i) {
        return std::to_string(*reinterpret_cast<std::uint32_t const*>(&i));
    }

    // Unique application ID
    std::string Control::m_app_id = "cafecafe";
    std::string Control::m_session_id;

    void Control::init() {
        // Initialize random session id
        srand(time(0));
        std::uint32_t x;
        x = rand() & 0xff;
        x |= (rand() & 0xff) << 8;
        x |= (rand() & 0xff) << 16;
        x |= (rand() & 0xff) << 24;
        std::ostringstream ss;
        ss << std::hex << std::setw(8) << std::setfill('0') << x;
        m_session_id = ss.str();
        M_trace("initialized with session id '" + m_session_id + "'");

        m_seqNum = 1;
        m_sock = std::make_unique<ClientSocket>("127.0.0.1", 5556, false);
        m_sock->connect();
        int numAttempt = 0;
        while(!m_sock->opened()) {
            M_trace("waiting for connection on :5556 " + std::to_string(numAttempt++) + "...");
            std::this_thread::sleep_for(100ms);
        }
        M_trace("connected !");
    }

    void Control::watchdog() {
        watchdog(m_seqNum.fetch_add(1), *m_sock);
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
        Control::configids(m_seqNum.fetch_add(1), *m_sock);
        Control::config(m_seqNum.fetch_add(1), key, value, *m_sock);
    }

    void Control::ackControl() {
        Control::ackControl(m_seqNum.fetch_add(1), *m_sock);
    }

    void Control::getCfgControl() {
        Control::getCfgControl(m_seqNum.fetch_add(1), *m_sock);
    }

    void Control::clearSessionId() {
        Control::configids(m_seqNum.fetch_add(1), *m_sock);
        Control::sessionid(m_seqNum.fetch_add(1), "-all", *m_sock);
    }

    void Control::setSessionId() {
        Control::configids(m_seqNum.fetch_add(1), *m_sock);
        Control::sessionid(m_seqNum.fetch_add(1), m_session_id, *m_sock);
    }

    void Control::setAppId() {
        Control::configids(m_seqNum.fetch_add(1), *m_sock);
        Control::appid(m_seqNum.fetch_add(1), m_app_id, *m_sock);
    }

    void Control::setAppDesc() {
        Control::configids(m_seqNum.fetch_add(1), *m_sock);
        Control::appdesc(m_seqNum.fetch_add(1), "Simon application", *m_sock);
    }

    void Control::setSessionDesc() {
        Control::configids(m_seqNum.fetch_add(1), *m_sock);
        Control::sessiondesc(m_seqNum.fetch_add(1), "Simon session", *m_sock);
    }

    void Control::strangeInit() {
        Control::pmode(m_seqNum.fetch_add(1), *m_sock);
        Control::misc(m_seqNum.fetch_add(1), *m_sock);
    }
    void Control::movement(int flag, float frontBackTilt, float leftRightTilt, float verticalSpeed, float angularSpeed) {
        movement(m_seqNum.fetch_add(1), flag, frontBackTilt, leftRightTilt, verticalSpeed, angularSpeed, *m_sock);
    }
    // Ask the drone to send the navdata :
    // AT*CONFIG="seqNum",\"general:navdata_demo\",\"TRUE\"\r
    void Control::sendNavData(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*CONFIG=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",\"general:navdata_demo\",\"TRUE\"\r";
        M_traceFrame(data);
        s.write(data);
    }

    // Reset the communication watchdog :
    // AT*COMWDG="seqNum"\r
    void Control::watchdog(std::uint32_t, ClientSocket& s) {
        std::string data("AT*COMWDG=1\r");
        // M_traceFrame(data);
        s.write(data);
    }

    // Emergency mode on the drone :
    // AT*REF="seqNum",290717952\r
    void Control::emergencyMode(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*REF=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",290717952\r";
        M_traceFrame(data);
        s.write(data);
    }

    // Calibrate the horizontal plan :
    // AT*FTRIM="seqNum"\r
    void Control::horizontalPlan(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*FTRIM=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + "\r";
        M_traceFrame(data);
        s.write(data);
    }

    // Calibrate the magnetometer :
    // AT*CALIB="seqNum",0\r
    void Control::magnetometer(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*CALIB=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",0\r";
        M_traceFrame(data);
        s.write(data);
    }

    // Take off :
    // AT*REF="seqNum",290718208\r
    void Control::takeoff(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*REF=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",290718208\r";
        M_traceFrame(data);
        s.write(data);
    }

    // Land :
    // AT*REF="seqNum",290717696\r
    void Control::land(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*REF=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",290717696\r";
        M_traceFrame(data);
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
        M_traceFrame(data);
        s.write(data);
    }

    // Send a control command ack, this resets the command_ack bit
    //   in the drone's state bitfield
    void Control::ackControl(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*CTRL=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",5,0\r";
        M_traceFrame(data);
        s.write(data);
    }

    // Get the drone's configuration dumped to the control port
    //   (TCP 5559)
    void Control::getCfgControl(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*CTRL=");
        std::string seqStr = std::to_string(seqNum);
        data += seqStr + ",4,0\r";
        M_traceFrame(data);
        s.write(data);
    }

    void Control::pmode(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*PMODE=");
        data += std::to_string(seqNum) + ",2\r";
        M_traceFrame(data);
        s.write(data);
    }

    void Control::misc(std::uint32_t seqNum, ClientSocket& s) {
        std::string data("AT*MISC=");
        data += std::to_string(seqNum) + ",2,20,2000,3000\r";
        M_traceFrame(data);
        s.write(data);
    }

    void Control::sessionid(std::uint32_t seqNum, std::string const& id, lcomm::ClientSocket& s) {
        config(seqNum, "custom:session_id", id, s);
    }

    void Control::appid(std::uint32_t seqNum, std::string const& id, lcomm::ClientSocket& s) {
        config(seqNum, "custom:application_id", id, s);
    }

    void Control::appdesc(std::uint32_t seqNum, std::string const& desc, lcomm::ClientSocket& s) {
        config(seqNum, "custom:application_desc", desc, s);
    }

    void Control::sessiondesc(std::uint32_t seqNum, std::string const& desc, lcomm::ClientSocket& s) {
        config(seqNum, "custom:session_desc", desc, s);
    }

    void Control::configids(std::uint32_t seqNum, ClientSocket& s) {
        std::string data = "AT*CONFIG_IDS=" + std::to_string(seqNum) + ",\"" + m_session_id + "\",\"" + "00000000" +
                           "\",\"" + m_app_id + "\"\r";
        M_traceFrame(data);
        s.write(data);
    }

    void Control::config(std::uint32_t seqNum, std::string const& key, std::string const& value, ClientSocket& s) {
        std::string data = "AT*CONFIG=" + std::to_string(seqNum) + ",\"" + key + "\",\"" + value + "\"\r";
        M_traceFrame(data);
        s.write(data);
    }

    void Control::M_trace(std::string const& msg) {
        std::cout << "[lcontrol::Control] " << msg << std::endl;
    }

    void Control::M_traceFrame(std::string const& msg) {
#if PRINT_FRAMES
        std::cout << "[lcontrol::Control] " << msg << std::endl;
#endif
    }
}
