//
// Created by remi on 26/11/15.
//

#ifndef SIMON_POSITIONCONTROL_H
#define SIMON_POSITIONCONTROL_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

namespace lcontrol {

    //! A distance/length value in metres.
    using Distance = float;

    class PositionControl {
        enum Motion { Front, Back, Left, Right };

    public:
        static void init();
        static void stop();

        static void frontMove() {
            PositionControl::move(Motion::Front);
        }
        static void backMove() {
            PositionControl::move(Motion::Back);
        }
        static void leftMove() {
            PositionControl::move(Motion::Left);
        }
        static void rightMove() {
            PositionControl::move(Motion::Right);
        }

        //! Distance between the drone and the origin along the x axis (i.e. from the left of the player to his right).
        static Distance xPos();
        //! Distance between the drone and the origin along the z axis (i.e. towards the player).
        static Distance yPos();
        //! Distance between the drone and the origin along the y axis (i.e. from hell to heaven).
        static Distance zPos();

    private:
        static void move(Motion m);

        //! Moves the drone in the specified direction.
        static void left(Distance d);
        static void right(Distance d);
        static void up(Distance d);
        static void down(Distance d);
        static void front(Distance d);
        static void back(Distance d);

        static void moveFront();
        static void moveBack();
        static void moveLeft();
        static void moveRight();

        static Distance m_x;
        static Distance m_y;
        static Distance m_z;
        static float m_alpha;
        static int m_move_duration;
        static int m_counter_move_duration;

        static void M_motionThread();

        static std::thread m_motionThread;
        static std::mutex m_motionMutex;
        static std::condition_variable m_motionVariable;
        static std::queue<Motion> m_motionQueue;
        static std::atomic_bool m_alive;
    };
}

#endif // SIMON_POSITIONCONTROL_H
