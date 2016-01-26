/*
 * Copyright (c) 2015 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
        static void movement(std::uint32_t seqNum, int flag, float frontBackTilt, float leftRightTilt, float verticalSpeed, float angularSpeed);
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

        static void write(std::string const& data);

    private:
        static std::atomic<std::uint32_t> m_seqNum;
        static std::unique_ptr<lcomm::ClientSocket> m_sock;

        static std::string m_session_id, m_app_id;
    };
}
