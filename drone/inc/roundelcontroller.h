#ifndef SIMON_ROUNDELCONTROLLER_H
#define SIMON_ROUNDELCONTROLLER_H

#include <string>

class NavdataController;

class RoundelController {
public:
    RoundelController(NavdataController& ctrl);
    ~RoundelController();

    void init();

private:
    void M_trace(std::string const& msg) const;

private:
    NavdataController& m_navctrl;
};

#endif // SIMON_ROUNDEL_H
