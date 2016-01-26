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
#include <chrono>

#include "lchrono/chrono.h"

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
        static void leftRight(Distance d);
        static void upDown(Distance d);
        static void frontBack(Distance d);

        static void moveFront();
        static void moveBack();
        static void moveLeft();
        static void moveRight();

        static Distance m_x;
        static Distance m_y;
        static Distance m_z;
        static float m_alpha;
        static lchrono::duration m_move_duration_lr;
        static lchrono::duration m_counter_move_duration_lr;
        static lchrono::duration m_move_duration_front;
        static lchrono::duration m_counter_move_duration_front;
        static lchrono::duration m_move_duration_back;
        static lchrono::duration m_counter_move_duration_back;

        static void M_motionThread();

        static std::thread m_motionThread;
        static std::mutex m_motionMutex;
        static std::condition_variable m_motionVariable;
        static std::queue<Motion> m_motionQueue;
        static std::atomic_bool m_alive;
    };
}

#endif // SIMON_POSITIONCONTROL_H
