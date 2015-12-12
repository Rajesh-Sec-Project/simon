#include "lcomm/lcomm.h"
#include <memory>
#include <atomic>
#include <mutex>

namespace lcontrol {
    class Control {
    public:
        static void init();

        static void watchdog();
        static void enableStabilization();
        static void takeoff();
        static void land();
        static void config(std::string const& key, std::string const& value);
        static void ackControl();
        static void getCfgControl();
        static void movement(int flag, float frontBackTilt, float leftRightTilt, float verticalSpeed, float angularSpeed);
        static void strangeInit();
        static void clearSessionId();
        static void setSessionId();
        static void setAppId();
        static void setAppDesc();
        static void setSessionDesc();

    private:
        static std::string float_to_string(float i);

        static void sendNavData(std::uint32_t seqNum);
        static void watchdog(std::uint32_t seqNum);
        static void emergencyMode(std::uint32_t seqNum);
        static void horizontalPlan(std::uint32_t seqNum);
        static void magnetometer(std::uint32_t seqNum);
        static void takeoff(std::uint32_t seqNum);
        static void land(std::uint32_t seqNum);
        static void movement(std::uint32_t seqNum,
                             int flag,
                             float frontBackTilt,
                             float leftRightTilt,
                             float verticalSpeed,
                             float angularSpeed);
        static void ackControl(std::uint32_t seqNum);
        static void getCfgControl(std::uint32_t seqNum);

        static void pmode(std::uint32_t seqNum);
        static void misc(std::uint32_t seqNum);

        static void sessionid(std::uint32_t seqNum, std::string const& id);
        static void appid(std::uint32_t seqNum, std::string const& id);
        static void appdesc(std::uint32_t seqNum, std::string const& desc);
        static void sessiondesc(std::uint32_t seqNum, std::string const& desc);

        static void configids(std::uint32_t seqNums);
        static void config(std::uint32_t seqNum, std::string const& key, std::string const& value);

        static void M_trace(std::string const& msg);
        static void M_traceFrame(std::string const& msg);

        static void write(std::string const &data);

    private:
        static std::atomic<std::uint32_t> m_seqNum;
        static std::unique_ptr<lcomm::ClientSocket> m_sock;

        static std::string m_session_id, m_app_id;
        static std::mutex m_mutex;
    };
}
