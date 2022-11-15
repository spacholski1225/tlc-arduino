#ifndef TrafficLight_h
#define TrafficLight_h

#include <Wire.h>
#include "PCF8574.h"
#include <Arduino.h>

class TrafficLight 
{
    public:
        void setUpTrafficLight(PCF8574 CE1, PCF8574 LE2, PCF8574 CE2, PCF8574 LE1);
        void dayMode();
        void setDayModeStartParameters();
        void nightMode();
        void setNightModeStartParameters();
};

#endif