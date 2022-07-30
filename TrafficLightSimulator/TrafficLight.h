#ifndef TrafficLight_h
#define TrafficLight_h

#include <Arduino.h>

class TrafficLight 
{
    private:
        int _diodes[18]; //cuz 12 for traffic light and 6 for zebra cross

    public:
        void setPinModes();
        void setDiodeToIndex(int diode, int index);
        void turnOnDiodes();//only for tests purpose
};

#endif