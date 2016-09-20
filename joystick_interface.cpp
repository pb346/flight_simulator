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
#define WIN_MAX_POS (65535.0)

#if __WIN32__
void saveWin(save_event* saved)
{
    JOYINFOEX joyinfo;
    joyinfo.dwFlags= JOY_RETURNBUTTONS;
    joyGetPosEx(JOYSTICKID1, &joyinfo);
    float calculate;
    if(joyinfo.dwXpos >= 32767)
    {
        calculate = (((joyinfo.dwXpos - (WIN_MAX_POS/2.0))*2)/WIN_MAX_POS);
        saved->stick_x = calculate;
    }
    else
    {
        calculate = ((joyinfo.dwXpos - WIN_MAX_POS/2)/(WIN_MAX_POS/2));
        saved->stick_x = calculate;
    }
    if(joyinfo.dwYpos >= 32767)
    {
        calculate = (((joyinfo.dwYpos - (WIN_MAX_POS/2.0))*2)/WIN_MAX_POS);
        saved->stick_y = calculate;
    }
    else
    {
        calculate = ((joyinfo.dwYpos - WIN_MAX_POS/2)/(WIN_MAX_POS/2));
        saved->stick_y = calculate;
    }
    if(joyinfo.dwRpos >= 32767)
    {
        calculate = (((joyinfo.dwRpos - (WIN_MAX_POS/2.0))*2)/WIN_MAX_POS);
        saved->stick_z = calculate;
    }
    else
    {
        calculate = ((joyinfo.dwRpos - WIN_MAX_POS/2)/(WIN_MAX_POS/2));
        saved->stick_z = calculate;
    }
    if(joyinfo.dwZpos >= 32767)
    {
        calculate = (((joyinfo.dwZpos - (WIN_MAX_POS/2.0))*2)/WIN_MAX_POS);
        saved->throttle = (-1)*calculate;
    }
    else
    {
        calculate = ((joyinfo.dwZpos - WIN_MAX_POS/2)/(WIN_MAX_POS/2));
        saved->throttle = (-1)*calculate;
    }
    saved->button[0] = joyinfo.dwButtons & 1;
    saved->button[1] = joyinfo.dwButtons & 2;
    saved->button[2] = joyinfo.dwButtons & 4;
    saved->button[3] = joyinfo.dwButtons & 8;
    saved->button[4] = joyinfo.dwButtons & 16;
    saved->button[5] = joyinfo.dwButtons & 32;
    saved->button[6] = joyinfo.dwButtons & 64;
    saved->button[7] = joyinfo.dwButtons & 128;
    saved->button[8] = joyinfo.dwButtons & 256;
    saved->button[9] = joyinfo.dwButtons & 512;
    saved->button[10] = joyinfo.dwButtons & 1024;
    saved->button[11] = joyinfo.dwButtons & 2048;

    int angle = joyinfo.dwPOV /100;
    if( 90 == angle )
    {
        saved->direct_x = 1;
        saved->direct_y = 0;
    }
    else if( 45 == angle)
    {
        saved->direct_x = 1;
        saved->direct_y = 1;
    }
    else if( 0 == angle )
    {
        saved->direct_x = 0;
        saved->direct_y = 1;
    }
    else if( 315 == angle)
    {
        saved->direct_x = -1;
        saved->direct_y = 1;
    }
    else if( 270 == angle )
    {
        saved->direct_x = -1;
        saved->direct_y = 0;
    }
    else if( 225 == angle)
    {
        saved->direct_x = -1;
        saved->direct_y = -1;
    }
    else if( 180 == angle )
    {
        saved->direct_x = 0;
        saved->direct_y = -1;
    }
    else if( 135 == angle)
    {
        saved->direct_x = 1;
        saved->direct_y = -1;
    }
    else
    {
        saved->direct_x = 0;
        saved->direct_y = 0;
    }
}
#endif

#endif // JOYSTICK_INTERFACE_H
