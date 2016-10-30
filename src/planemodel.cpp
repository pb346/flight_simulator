#include "planemodel.h"

PlaneModel::PlaneModel()
{

}

void DebugValues::copyDebug(DebugValues* previousDebug)
{
    previousDebug->aileronLeft = this->aileronLeft;
    previousDebug->aileronRight = this->aileronRight;
    previousDebug->elevator = this->elevator;
   // previousDebug->flap = this->flap;
    previousDebug->rudder = this->rudder;
  //  previousDebug->slat = this->slat;
    previousDebug->thrust = this->thrust;
    previousDebug->gears = this->gears;
}
