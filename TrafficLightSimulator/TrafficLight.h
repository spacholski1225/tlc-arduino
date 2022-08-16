#ifndef TrafficLight_h
#define TrafficLight_h

#include <Wire.h>
#include "PCF8574.h"
#include <Arduino.h>

class TrafficLight 
{
    public:
        void setUpTrafficLight(PCF8574 crossExpander, PCF8574 lightExpander);
        void dayMode();
        void setDayModeStartParameters();
        void nightMode();
        void setNightModeStartParameters();
};

#endif