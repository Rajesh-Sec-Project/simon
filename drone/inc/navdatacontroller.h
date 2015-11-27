#ifndef DRONE_NAVDATACONTROLLER_H
#define DRONE_NAVDATACONTROLLER_H

#include <cstdint>
#include "pcap.h"
#include <string>
#include <thread>
#include <atomic>
#include "gameelement.h"

namespace navdata {
    //! This enumeration map bitfields to the drone state word in
    //!   the navdata header.
    enum drone_state {
        //!< (0) ardrone is landed, (1) ardrone is flying
        fly = 1 << 0,
        //!< (0) video disable, (1) video enable
        video = 1 << 1,
        //!< (0) vision disable, (1) vision enable
        vision = 1 << 2,
        //!< (0) euler angles control, (1) angular speed control
        control = 1 << 3,
        //!< (0) altitude control inactive (1) altitude control active
        altitude = 1 << 4,
        //!< Start button stat
        user_feedback = 1 << 5,
        //!< Control command ACK : (0) None, (1) one received
        command_ack = 1 << 6,
        //!< (0) camera not ready, (1) Camera ready
        camera = 1 << 7,
        //!< Travelling mask : (0) disable, (1) enable
        travelling = 1 << 8,
        //!< USB key : (0) usb key not ready, (1) usb key ready
        usb = 1 << 9,
        //!< Navdata demo : (0) All navdata, (1) only navdata demo
        navdata_demo = 1 << 10,
        //!< (0) options sent in all or demo mode, (1) no navdata options sent
        navdata_bootstrap = 1 << 11,
        //!< (0) Ok, (1) Motors proble
        motors = 1 << 12,
        //!< Communication Lost : (1) com problem, (0) Com is ok
        com_lost = 1 << 13,
        //!< Software fault detected - user should land as quick as possible (1)
        software_fault = 1 << 14,
        //!< VBat low : (1) too low, (0) Ok
        vbat_low = 1 << 15,
        //!< User Emergency Landing : (1) User EL is ON, (0) User EL is OFF
        user_emergency = 1 << 16,
        //!< Timer elapsed : (1) elapsed, (0) not elapsed
        timer_elapsed = 1 << 17,
        //!< Magnetometer calibration state : (0) Ok, no calibration needed, (1) not ok, calibration needed
        magneto_needs_calib = 1 << 18,
        //!< Angles : (0) Ok, (1) out of range
        angles_out_of_range = 1 << 19,
        //!< (0) ok, (1) Too much wind
        wind = 1 << 20,
        //!< Ultrasonic sensor : (0) Ok, (1) deaf
        ultrasonic = 1 << 21,
        //!< Cutout system detection : (0) Not detected, (1) detected
        cutout = 1 << 22,
        //!< Version number OK : (0) a bad version number, (1) version number is OK
        navboard_version = 1 << 23,
        //!< ATCodec thread ON : (0) thread OFF (1) thread ON
        atcodec_thread = 1 << 24,
        //!< Navdata thread ON : (0) thread OFF (1) thread ON
        navdata_thread = 1 << 25,
        //!< Video thread ON : (0) thread OFF (1) thread ON
        video_thread = 1 << 26,
        //!< Acquisition thread ON : (0) thread OFF (1) thread ON
        acq_thread = 1 << 27,
        //!< CTRL watchdog : (1) delay in control execution (> 5ms), (0) control is well scheduled
        ctrl_watchdog = 1 << 28,
        //!< ADC Watchdog : (1) delay in uart2 dsr (> 5ms), (0) uart2 is good
        adc_watchdog = 1 << 29,
        //!< Communication Watchdog : (1) com problem, (0) Com is ok
        com_watchdog = 1 << 30,
        //!< Emergency landing : (0) no emergency, (1) emergency
        emergency = 1 << 31
    };

    //! This enumeration defines the navdata options' tags.
    //! To enable a specific navdata option, write (1 << tag) in
    //!   the general:navdata_options configuration parameter (while
    //!   setting general:navdata_demo to FALSE).
    enum option_tag {
        option_demo = 0,
        option_time,
        option_raw_measures,
        option_phys_measures,
        option_gyros_offsets,
        option_euler_angles,
        option_references,
        option_trims,
        option_rc_references,
        option_pwm,
        option_altitude,
        option_vision_raw,
        option_vision_of,
        option_vision,
        option_vision_perf,
        option_trackers_send,
        option_vision_detect,
        option_watchdog,
        option_adc_data_frame,
        option_video_stream,
        option_games,
        option_pressure_raw,
        option_magneto,
        option_wind,
        option_kalman_pressure,
        option_hdvideo_stream,
        option_wifi,
        option_cks = 0xFFFF
    };

    //! The global header for all Navdata UDP packets
    struct __attribute__((packed)) header {
        //!< Navdata magic number, always 0x55667788
        uint32_t magic;
        //!< Drone's state mask, \see drone_state enumeration
        uint32_t state;
        //!< Current sequence number
        uint32_t seq;
        //!< Is vision enabled ?
        uint32_t vision;
    };

    //! The Navdata option header.
    struct __attribute__((packed)) option_header {
        //!< Option tag, \see option_tag enumeration
        uint16_t tag;
        //!< Option data size
        uint16_t size;
    };

    // See ARDroneLib/Soft/Common/navdata_common.h for
    //   options structures.

    //! Enumeration values for demo.ctrl_state
    enum option_demo_state {
        DEMO_STATE_DEFAULT = 0,
        DEMO_STATE_INIT,
        DEMO_STATE_LANDED,
        DEMO_STATE_FLYING,
        DEMO_STATE_HOVERING
    };

    //! Demo navdata option
    struct __attribute__((packed)) demo {
        //!< Normally this should be 'option_demo'
        uint16_t tag;
        //!< Size of this option
        uint16_t size;

        //!< Drone's state, see option_demo_state enumeration
        uint32_t ctrl_state;
        //< Battery voltage in mV
        uint32_t vbat_flying_percentage;

        //!< Pitch in mdeg
        float theta;
        //!< Roll in mdeg
        float phi;
        //!< Yaw in mdeg
        float psi;

        //!< Altitude in cm
        int32_t altitude;

        //!< Estimated linear speeds
        float vx;
        float vy;
        float vz;

        //!< Not used
        uint32_t num_frames;

        //!< Don't use, deprecated
        uint32_t detection_camera_rot[3][3];
        uint32_t detection_camera_trans[3];
        uint32_t detection_tag_index;

        //!< Type of tag searched in detection
        uint32_t detection_camera_type;

        //!< Don't use, deprecated
        uint32_t drone_camera_rot[3][3];
        uint32_t drone_camera_trans[3];
    };

    struct matrix33_t
    {
      float m11;
      float m12;
      float m13;
      float m21;
      float m22;
      float m23;
      float m31;
      float m32;
      float m33;
    };

    struct vector31_t {
      union {
        float v[3];
        struct
        {
          float x;
          float y;
          float z;
        };
      };
    };

    //! Vision detection navdata option
    struct __attribute__((packed)) vision_detect {
        //!< Normally this should be 'option_vision_detect'
        uint16_t tag;
        //!< Size of this struct
        uint16_t size;

        uint32_t nb_detected;
        uint32_t type[4];
        uint32_t xc[4];
        uint32_t yc[4];
        uint32_t width[4];
        uint32_t height[4];
        uint32_t dist[4];
        float orientation_angle[4];
        matrix33_t rotation[4];
        vector31_t translation[4];
        uint32_t camera_source[4];
    };

    struct __attribute__((packed)) trackers_send {
        uint16_t tag;
        uint16_t size;

        int32_t locked[6*5];
        int32_t point[6*5][2];
    };
}

struct Navdata {
    navdata::header header;
    navdata::demo demo;
    navdata::vision_detect vision_detect;
    navdata::trackers_send trackers_send;
};

class GameSystem;

class NavdataController : public GameElement {
public:
    NavdataController(GameSystem& system);
    ~NavdataController();

    //! Init the navdata controller, but don't setup
    //!   navdata options, leave the drone in bootstrap mode
    void init();

    //! Configure the navdata (i.e. leave bootstrap mode)
    void configure();

    //! Check if the navdata controller is initialized
    bool inited() const;

    //! Check if the navdata controller is configures
    bool configured() const;

    //! Check if some new navdata is available
    bool available() const;

    //! Grab some navdata (resets avail flag)
    Navdata grab();

private:
    void M_setupPcap();
    void M_sniffed(const pcap_pkthdr* header, const unsigned char* buffer);
    static void M_proxy(unsigned char* self, const pcap_pkthdr* header, const unsigned char* buffer);

    void M_initNavdata();
    void M_configure();
    void M_decode(const unsigned char* data, int size);

private:
    std::thread m_pcap;
    pcap_t* m_pcap_handle;

    std::atomic<bool> m_available, m_inited, m_startPcap, m_configured;
    Navdata m_navdata;

    static std::string m_fool_ip;
    static std::string m_sniff_ip;
    static int m_sniff_port;
    static int m_timeout;
};

#endif // DRONE_NAVDATACONTROLLER_H
