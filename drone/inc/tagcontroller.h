#ifndef SIMON_TAGCONTROLLER_H
#define SIMON_TAGCONTROLLER_H

#include "gameelement.h"
#include "lcomm/clientsocket.h"
#include <string>
#include <array>

class NavdataController;

class TagController : public GameElement {
public:
    TagController(GameSystem& system);
    ~TagController();

    void gameInit() override;
    void gameLoop() override;

    //! Returns true if there is a tag being detected
    bool hasDetection() const;

    //! Returns the detected tag's horizontal
    //!   position (if applicable) in cm
    float tagPositionX() const;

    //! Returns the detected tag's vertical
    //!   position (if applicable) in cm
    float tagPositionY() const;

    //! Returns the corrected and averaged tag
    //!   horizontal speed in mm/s
    float tagSpeedX() const;

    //! Returns the corrected and averaged tag
    //!   vertical speed in mm/s
    float tagSpeedY() const;

private:
    void M_clearAck();

private:
    NavdataController& m_navctrl;
    bool m_has_detection;
    float m_tag_x;
    float m_tag_y;

    float m_avg_update, m_avg_corr_update;
    float m_avg_vx, m_avg_cor_vx;
    float m_avg_vy, m_avg_cor_vy;
};

#endif // SIMON_TAGCONTROLLER_H
