#ifndef DRONE_AT_H
#define DRONE_AT_H

#include <string>
#include <sstream>
#include <cstdint>

namespace at {
    //! Encode an integer into the AT codec.
    //! \param value The integer to encode
    //! \return The encoded value
    static std::string argument(int value) {
        std::ostringstream ss;
        ss << value;
        return ss.str();
    }

    //! Encode a boolean into the AT codec (as an integer).
    //! \param value The boolean to encode
    //! \return The encoded value
    static std::string argument(bool value) {
        return argument((int)value);
    }

    //! Encode an string into the AT codec.
    //! \param value The string to encode
    //! \return The encoded value
    static std::string argument(std::string const& value) {
        return "\"" + value + "\"";
    }

    //! Encode an floating-point value into the AT codec.
    //! \param value The float to encode
    //! \return The encoded value
    static std::string argument(float value) {
        union {
            float as_float;
            int32_t as_int;
        };

        as_float = value;
        return argument(as_int);
    }

    //! Encode a sequence of values into the AT format
    //! (internal implementation function - termination)
    template <typename T>
    static std::string formatted_impl(T t) {
        return argument(t);
    }

    //! Encode a sequence of values into the AT format
    //! (internal implementation function - recursion)
    template <typename T, typename... Args>
    static std::string formatted_impl(T t, Args... args) {
        std::string fmted = argument(t);
        return fmted + "," + formatted_impl(args...);
    }

    //! Encode a sequence of values into the AT format.
    //! \param args The arguments to encode
    //! \return The encoded string
    template <typename... Args>
    static std::string formatted(Args... args) {
        return formatted_impl(args...);
    }

    //! Product an AT command string.
    //! \param cmd The command name
    //! \param seq The sequence number
    //! \param args The additional command's arguments
    //! \return The produced command
    template <typename... Args>
    static std::string command(std::string const& cmd, int seq, Args... args) {
        return "AT*" + cmd + "=" + formatted(seq, args...) + "\r";
    }
}

namespace at {
    //! Tell the drone to take off, land or set it
    //!   to emergency mode.
    struct ref {
        //!< Start bit, set to 'true' to take off, 'false' to land
        bool start;
        //!< Select bit, set to 'true' to enter or exit emergency mode
        bool select;
        //!< The sequence number
        int seq;

        //! Serialize this command to a sendable string
        std::string toString() const {
            int base = (1 << 18) | (1 << 20) | (1 << 22) | (1 << 24) | (1 << 28);
            return command("REF", seq, base | (start << 9) | (select << 8));
        }
    };

    //! Tell the drone to change its orientation or speed, or tell
    //!   it to enter hovering mode.
    //! Use when taking off or when taked off.
    struct pcmd {
        //!< Enable absolute control mode
        bool absolute_control;
        //!< Enable combined yaw mode
        bool combined_yaw;
        //!< Progressive command enable (set to false for hovering mode)
        bool progressive_enable;
        //!< Positive to go right, negative to go left, [-1.0, 1.0]
        float roll;
        //!< Negative to go front, negative to go back, [-1.0, 1.0]
        float pitch;
        //!< Positive to go up, negative to go down, [-1.0, 1.0]
        float gaz;
        //!< Positive to spin right, negative to spin left, [-1.0, 1.0]
        float yaw;
        //!< The sequence number
        int seq;

        //! Serialize this command to a sendable string
        std::string toString() const {
            int flag = (absolute_control << 2) | (combined_yaw << 1) | (progressive_enable << 0);
            return command("PCMD", seq, flag, roll, pitch, gaz, yaw);
        }
    };

    //! Tell the drone that it is lying horizontally on the floor.
    //! Use only when landed !
    struct ftrim {
        //!< The sequence number
        int seq;

        //! Serialize this command to a sendable string
        std::string toString() const {
            return command("FTRIM", seq);
        }
    };

    //! Set a configuration key-value pair.
    //! You shall probably look on the control port if everything went well.
    struct config {
        //!< The configuration key
        std::string key;
        //!< The configuration value
        std::string value;
        //!< The sequence number
        int seq;

        //! Serialize this command to a sendable string
        std::string toString() const {
            return command("CONFIG", seq, key, value);
        }
    };

    //! Send a communication watchdog reset command.
    //! You should send this command on a regular basis (less than 50 ms).
    struct commwdg {
        //!< The sequence number
        int seq;

        //! Serialize this command to a sendable string
        std::string toString() const {
            return command("COMMWDG", seq);
        }
    };
}

#endif // DRONE_AT_H
