#ifndef SIMON_TAGCONTROLLER_H
#define SIMON_TAGCONTROLLER_H

#include "gameelement.h"
#include "lcomm/clientsocket.h"
#include <string>
#include <array>
#include <fstream>

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

    //! Returns the detected tag's depth
    //!   position (if applicable) in cm
    //! Computed using its size
    float tagPositionZ() const;

    //! Returns the corrected and averaged tag
    //!   horizontal speed in mm/s
    float tagSpeedX() const;

    //! Returns the corrected and averaged tag
    //!   vertical speed in mm/s
    float tagSpeedY() const;

private:
    struct DContext;

    void M_clearAck();
    void M_initMoveDetection();
    int M_moveDetection(DContext& context, float v);
    float M_xcorr(std::vector<float> const& d, std::vector<float> const& p, int i);
    std::vector<float>& M_xcorr(std::vector<float> const& d, std::vector<float> const& p);
    int M_localMax(std::vector<float>& v, float minval);
    int M_localMin(std::vector<float>& v, float maxval);

private:
    NavdataController& m_navctrl;
    bool m_has_detection;
    float m_tag_x;
    float m_tag_y;
    float m_tag_z;

    float m_avg_update, m_avg_corr_update;
    float m_avg_vx, m_avg_cor_vx;
    float m_avg_vy, m_avg_cor_vy;

    struct DContext {
        //! Width of the detection window
        unsigned int window;
        //! Step of the detection
        unsigned int step;
        //! Nominal amplitude of moves
        float amplitude;
        //! Current data window
        std::vector<float> data;
        //! Reference pattern
        std::vector<float> reference_pattern;

        //! Counter for the step
        unsigned int step_counter;
    } m_horiz_detect, m_vert_detect;

    std::ofstream m_log;

    int m_delay;
    int m_delayCounter;
};

#endif // SIMON_TAGCONTROLLER_H
