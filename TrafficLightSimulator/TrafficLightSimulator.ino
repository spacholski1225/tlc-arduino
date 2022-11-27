#include <Wire.h>
#include "TrafficLight.h"
#include "PCF8574.h"

#define TOGGLE_MODE 9

const int TIME_TO_SETUP = 1000;

PCF8574 CE1;
PCF8574 LE2;
PCF8574 CE2;
PCF8574 LE1;

TrafficLight _traffic;

bool _shouldBeStartUpParams;
bool _isDayMode = false;
bool _lastState;

void setup()
{
  CE1.begin(0x20);
  LE2.begin(0x24);
  CE2.begin(0x22);
  LE1.begin(0x21);

  pinMode(TOGGLE_MODE, INPUT_PULLUP);

  _isDayMode = digitalRead(TOGGLE_MODE) == LOW;

  if(_isDayMode)
  {
    _traffic.setUpTrafficLight(CE1, LE2, CE2, LE1);
    _traffic.setDayModeStartParameters();
  }

  else
  {
   _traffic.setUpTrafficLight(CE1, LE2, CE2, LE1);
   _traffic.setNightModeStartParameters();
  }
}

void loop()
{
  _lastState = _isDayMode;
  _isDayMode = digitalRead(TOGGLE_MODE);

  if(_isDayMode)
  {
    if(!_lastState && _isDayMode)
    {
      _traffic.setUpTrafficLight(CE1, LE2, CE2, LE1);
      _traffic.setDayModeStartParameters(); 
      _shouldBeStartUpParams = false;
      delay(TIME_TO_SETUP);
    }
    _traffic.dayMode();
  }
  else
  {
    if(_lastState && !_isDayMode)
    {
      _traffic.setUpTrafficLight(CE1, LE2, CE2, LE1);
      _traffic.setNightModeStartParameters();

      _shouldBeStartUpParams = false;
      delay(TIME_TO_SETUP);
    }
    _traffic.nightMode();
  }
}
