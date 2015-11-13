#ifndef DRONE_NAVDATA_H
#define DRONE_NAVDATA_H

#include <cstdint>

namespace navdata {
    //! This enumeration map bitfields to the drone state word in
    //!   the navdata header.
    enum class drone_state {
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
        command = 1 << 6,
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
        option_wifi
    };

    // See ARDroneLib/Soft/Common/navdata_common.h for
    //   options structures.

    //! The global header for all Navdata UDP packets
    struct header {
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
    struct option_header {
        //!< Option tag, \see option_tag enumeration
        uint16_t tag;
        //!< Option data size
        uint16_t size;
    };
}

#endif // DRONE_NAVDATA_H
