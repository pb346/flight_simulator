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

#if __WIN32__
void saveWin(save_event* saved);
#endif

#endif // JOYSTICK_INTERFACE_H
