/*
 *  \file joystick_interface.h
 *
 *  \brief  functions used in data processing
 *
 *  \author Paul Babala
 *
 *
 */
/*-- INCLUDES: ------------------------------------------------------------*/
#include "joystick_interface.h"
#include "plane.h"
#include "planemodel.h"
#include <vector>
#include "Windows.h"


/*-- DEFINES: -------------------------------------------------------------*/
#define WIN_MAX_POS (65535.0)
void saveWin(joystick_event* saved)
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

void process_joystick_input(PlaneModel* model, joystick_event* event, DebugValues** debug, Plane** planeState)
{
    DebugValues* localDebug = *debug;
    Plane* planeReference = *planeState;

/***************************AILERON*************************************/
    double local_right_aileron_angle = event->stick_x * model->maxAileronAngle;
    double local_left_aileron_angle = (-1.0)* event->stick_x * model->maxAileronAngle;
    if(( local_right_aileron_angle < 0 && (local_right_aileron_angle + .2) > 0 ) || (local_right_aileron_angle > 0 && (local_right_aileron_angle-.2 < 0)))
    {
        planeReference->right_aileron_angle = 0;
        planeReference->left_aileron_angle =  0;
        localDebug->aileronLeft = 0;
        localDebug->aileronRight = 0;
    }
    else
    {
        planeReference->right_aileron_angle = local_right_aileron_angle;
        planeReference->left_aileron_angle = local_left_aileron_angle;
        localDebug->aileronLeft = planeReference->left_aileron_angle;
        localDebug->aileronRight = planeReference->right_aileron_angle;
    }
    double tempThrottle;
    if(event->throttle > 0.0)
    {
        tempThrottle = 1.0 + event->throttle;
    }
    else
    {
        tempThrottle = 1.0 - (-1.0)*event->throttle;
    }
    if(tempThrottle > 1.7)
    {
        double afterburnerTemp = 2.0 - tempThrottle;
        double afterburnerVal = ((.3-afterburnerTemp) / .30 *100);
        localDebug->thrust = 100;
        afterburnerTemp = 2.0 - tempThrottle;
        if(planeReference->afterburnerActive == 1)
        {
            afterburnerVal = ((.30-afterburnerTemp) / .30 *100);
            localDebug->afterburner = afterburnerVal;
        }
        else
        {
            localDebug->afterburner = 0;
        }
    }
    else
    {
        localDebug->thrust = tempThrottle /1.7 * 100;
        localDebug->afterburner = 0;
    }

    localDebug->rudder = event->stick_z *model->maxRudderAngle;
    planeReference->rudder_angle = localDebug->rudder;
    planeReference->m_thrust = localDebug->thrust;

     if(( event->stick_y < 0 && (event->stick_y + .03) > 0 ) || (event->stick_y > 0 && (event->stick_y -.03 < 0)))
     {
         localDebug->elevator = 0;
         planeReference->elevator_angle = 0;
     }
     else
     {
         localDebug->elevator = event->stick_y * model->maxElevPOS;
         planeReference->elevator_angle = localDebug->elevator;
     }

    if(event->button[7] != 0)
    {
        localDebug->gears = 1;
    }
    else
    {
        localDebug->gears = 0;
    }
    if(event->button[1] != 0)
    {
        localDebug->afterburnerActive = 1;
        planeReference->afterburnerActive = 1;
    }
    else
    {
        localDebug->afterburnerActive = 0;
        planeReference->afterburnerActive = 0;
    }
    /*
    if(event->button[10] != 0)
    {
        localDebug->flapDown = 1;
    }
    else
    {
        localDebug->flapDown = 0;
    }
    if(event->button[11] != 0)
    {
        localDebug->flapUp = 1;
    }
    else
    {
        localDebug->flapUp = 0;
    }*/

    if(event->button[8] != 0)
    {
        localDebug->brakes = 1;
        planeReference->brakes = 1;
    }
    else
    {
        localDebug->brakes = 0;
        planeReference->brakes = 0;
    }
    if(event->button[9] != 0)
    {
        localDebug->auxActive = 1;
        planeReference->auxActive = 1;
    }
    else
    {
        localDebug->auxActive = 0;
        planeReference->auxActive = 0;
    }
    //slat
    //flap
    //thrust, lElev, rElev, lAil, rAil, slatl, slatr, rudder

}
