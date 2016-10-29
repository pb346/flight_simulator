#ifndef PLANEMODEL_H
#define PLANEMODEL_H
#include <QString>

class PlaneModel
{
public:
    PlaneModel();
    int ID;             //program defined
    QString Name;
    QString Nickname;
    int numEngines;
    float maxThrust;
    float surfaceArea;
    float minWeight;    //no fuel, no AUX
    float maxWeight;    //max fuel, plus AUX

/**Lift Surfaces **/
    float maxSlatPOS;     //leading edge flap angle positive
    float maxSlatNEG;     //leading edge flap angle negative
    float maxFlapAngleNEG; //never positive
/**Control Surfaces **/
    float maxElevPOS;
    float maxElevNEG;      //less than pos
    float maxRudderAngle;  //both pos and neg;
    float maxAileronAngle; //both pos and neg

    float maxActuatorSpeed; //degrees/10ms
    float dragCoeff;
};

struct DebugValues
{
    double aileronLeft;
    double aileronRight;
    double elevator;
    double rudder;
    double thrust;
    double slat;
    double flap;
};
#endif // PLANEMODEL_H
