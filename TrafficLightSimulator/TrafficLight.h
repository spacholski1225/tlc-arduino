#ifndef TrafficLight_h
#define TrafficLight_h

#include <Wire.h>
#include "PCF8574.h"
#include <Arduino.h>

class TrafficLight 
{
    private:
        PCF8574 _crossExpander;
        PCF8574 _lightExpander;
    public:
        TrafficLight(PCF8574 crossExpander, PCF8574 lightExpander);
        void turnOnGreenDiodeAndOffRed(int greenDiode, int yellowDiode, int redDiode);
        void turnOnRedDiodeAndOffGreen(int greenDiode, int yellowDiode, int redDiode);
        void turnOnRedZebraCrossControlledByTime(int greenDiode, int redDiode);
        void turnOnGreenZebraCrossControlledByTime(int greenDiode, int redDiode);
};

#endif