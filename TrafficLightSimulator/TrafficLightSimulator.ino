#include <Wire.h>
#include "TrafficLight.h"
#include "PCF8574.h"

#define TOGGLE_MODE 9

PCF8574 CE1;
PCF8574 LE2;
PCF8574 CE2;
PCF8574 LE1;

TrafficLight _traffic;

bool _shouldBeStartUpParams;
bool _isDayMode;
bool _lastState;

void setup()
{
  CE1.begin(0x20); //CE1
  LE2.begin(0x24); // LE2
  CE2.begin(0x22); // CE2
  LE1.begin(0x21); //LE1

  pinMode(TOGGLE_MODE, INPUT);

  _isDayMode = digitalRead(TOGGLE_MODE) == HIGH;

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

  if (_isDayMode)
  {
    if(!_lastState && _isDayMode)
    {
      _traffic.setUpTrafficLight(CE1, LE2, CE2, LE1);
      _traffic.setDayModeStartParameters(); 

      _shouldBeStartUpParams = false;
      delay(1000);
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
      delay(1000);
    }

    _traffic.nightMode();
  }
}