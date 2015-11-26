//
// Created by remi on 26/11/15.
//

#ifndef SIMON_POSITIONCONTROL_H
#define SIMON_POSITIONCONTROL_H

namespace lcontrol {

    //! A distance/length value in metres.
    using Distance = float;

    class PositionControl {
    public:
        //! Moves the drone in the specified direction.
        static void left(Distance d);
        static void right(Distance d);
        static void up(Distance d);
        static void down(Distance d);
        static void front(Distance d);
        static void back(Distance d);

        //! Changes the origin of the reference axes.
        static void setOrigin(Distance x, Distance y, Distance z);

        //! Distance between the drone and the origin along the x axis (i.e. from the left of the player to his right).
        static Distance xPos();
        //! Distance between the drone and the origin along the z axis (i.e. towards the player).
        static Distance yPos();
        //! Distance between the drone and the origin along the y axis (i.e. from hell to heaven).
        static Distance zPos();
    };
}

#endif //SIMON_POSITIONCONTROL_H
