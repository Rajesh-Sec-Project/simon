#include "lcomm/lcomm.h"
#include <memory>
#include <atomic>

namespace lcontrol {
    class Control {
    public:
        static void init();

        static void enableStabilization();
        static void takeoff();
        static void land();

    private:
        static std::string float_to_string(float i);

        static void sendNavData(std::uint32_t seqNum, lcomm::ClientSocket& s);
        static void watchdog(std::uint32_t seqNum, lcomm::ClientSocket& s);
        static void emergencyMode(std::uint32_t seqNum, lcomm::ClientSocket& s);
        static void horizontalPlan(std::uint32_t seqNum, lcomm::ClientSocket& s);
        static void magnetometer(std::uint32_t seqNum, lcomm::ClientSocket& s);
        static void takeoff(std::uint32_t seqNum, lcomm::ClientSocket& s);
        static void land(std::uint32_t seqNum, lcomm::ClientSocket& s);
        static void movement(std::uint32_t seqNum,
                             int flag,
                             float frontBackTilt,
                             float leftRightTilt,
                             float verticalSpeed,
                             float angularSpeed,
                             lcomm::ClientSocket& s);

        static std::atomic<std::uint32_t> m_seqNum;
        static std::unique_ptr<lcomm::ClientSocket> m_sock;
    };
}
