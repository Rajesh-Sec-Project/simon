#include "roundelcontroller.h"
#include "lcontrol/control.h"
#include "navdatacontroller.h"

#include <thread>
#include <chrono>

enum DetectionMode
{
  Mode_Deprecated1 = 0,
  Mode_Deprecated2,
  //!< 2D horizontal tags on drone shells
  Mode_F_HShells,
  Mode_None,
  //!< Roundel under the drone
  Mode_U_Cocarde,
  //!< Oriented roundel under the drone
  Mode_U_OrientedCocarde,
  //!< Uniform stripe on the ground
  Mode_U_Stripe,
  //!< Roundel in front of the drone
  Mode_F_Cocarde,
  //!< Oriented roundel in front of the drone
  Mode_F_OrientedCocarde,
  Mode_F_Stripe,
  Mode_Multiple,
  Mode_F_Cap,
  Mode_OrientedCocardeBW,
  Mode_F_HShellsV2,
  Mode_F_TowerSide
};

enum Tag
{
	  Tag_None             = 0,
  Tag_ShellTag,
  Tag_Roundel,
  Tag_OrientedRoundel,
  Tag_Stripe,
  Tag_Cap,
  Tag_ShellsV2,
  Tag_TowerSize,
  Tag_RoundelBW
};

using namespace lcontrol;

RoundelController::RoundelController(NavdataController& navctrl) :
	m_navctrl(navctrl)
{
	
}

RoundelController::~RoundelController()
{
	
}

void RoundelController::init()
{
	while (!m_navctrl.inited())
		;

	Control::config("video:video_channel", "1");
	std::this_thread::sleep_for(std::chrono::milliseconds(250));

	Control::config("detect:detect_type", std::to_string(Mode_Multiple));
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
	M_trace("setting detect type");

	Control::config("detect:detections_select_v", std::to_string((0x01 << Tag_OrientedRoundel) | (0x01 << Tag_RoundelBW)));
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
	M_trace("setting detected tags");
}

void RoundelController::M_trace(std::string const& msg) const
{
	std::cout << "[RoundelController] " << msg << std::endl;
}
