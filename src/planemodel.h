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
    float dragCoeff;
};

#endif // PLANEMODEL_H
