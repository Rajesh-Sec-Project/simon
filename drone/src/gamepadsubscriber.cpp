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
