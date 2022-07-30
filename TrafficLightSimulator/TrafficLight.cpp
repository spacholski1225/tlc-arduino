#include "TrafficLight.h"

void TrafficLight::setDiodeToIndex(int diode, int index)
{
    _diodes[index] = diode;
}

void TrafficLight::setPinModes()
{
    int diodesLength = sizeof(_diodes) / sizeof(*_diodes);

    for (int i = 0; i < diodesLength; i++)
    {
        pinMode(_diodes[i], OUTPUT);
        delay(1);
    }
}

void TrafficLight::turnOnDiodes() //only for tests purpose, to delete
{
    int diodesLength = sizeof(_diodes) / sizeof(*_diodes);

    for (int i = 0; i < diodesLength; i++)
    {
        digitalWrite(_diodes[i], HIGH);
        delay(1);
    }
}