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
#include <Windows.h>
#include "planemodel.h"
#include "plane.h"

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

void process_joystick_input(PlaneModel* model, joystick_event* event, DebugValues** debug, Plane** planeState);
void saveWin(joystick_event* saved);

#endif // JOYSTICK_INTERFACE_H
