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

#include "gamepadsubscriber.h"
#include "lcontrol/control.h"
#include "lcontrol/positioncontrol.h"
#include "lcomm/lcomm.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/gamepad_position_packet.h"
#include "gamesystem.h"
#include <thread>
#include <chrono>

using namespace lcontrol;
using namespace lcomm;
using namespace lmoves;

GamePadSubscriber::GamePadSubscriber(GameSystem& system)
        : GameElement(system) {
}

void GamePadSubscriber::notify(Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) {

    if(GamepadPositionPacket* ctrl = packet->downcast<GamepadPositionPacket>()) {
        if(ctrl->keys() & GamepadPositionPacket::Land) {
            Control::land();
        } else if(ctrl->keys() & GamepadPositionPacket::TakeOff) {
            Control::takeoff();
        } else if(ctrl->keys() & GamepadPositionPacket::Up) {
            PositionControl::frontMove();
        } else if(ctrl->keys() & GamepadPositionPacket::Down) {
            PositionControl::backMove();
        } else if(ctrl->keys() & GamepadPositionPacket::Left) {
            PositionControl::leftMove();
        } else if(ctrl->keys() & GamepadPositionPacket::Right) {
            PositionControl::rightMove();
        }
    }
}
