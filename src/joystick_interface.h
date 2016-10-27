/*
 *  \file joystick_interface.h
 *
 *  \brief  functions used in data processing
 *
 *  \author Paul Babala
 *
 *
 */

#ifndef JOYSTICK_INTERFACE_H
#define JOYSTICK_INTERFACE_H
/*-- INCLUDES: ------------------------------------------------------------*/

#include <vector>
#if __WIN32__
#include <Windows.h>
#endif

/*-- DEFINES: -------------------------------------------------------------*/

/*-- TYPEDEFS: ------------------------------------------------------------*/

/*-- CONSTANT DEFINITIONS: ------------------------------------------------*/

/*-- GLOBAL DECLARATIONS: -------------------------------------------------*/

/*-- FUNCTION DECLARATIONS: -----------------------------------------------*/
struct joystick_event {
    int button[12];
    float stick_x; //Up/Down Aileron
    float stick_y; //up/down Elevator
    float stick_z; //left/right rudder
    float throttle; //thrust
    float direct_x;
    float direct_y;
};

#if __WIN32__
void saveWin(joystick_event* saved);
#endif

#endif // JOYSTICK_INTERFACE_H
