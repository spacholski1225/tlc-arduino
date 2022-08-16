#include <Wire.h>
#include "TrafficLight.h"
#include "PCF8574.h"


PCF8574 crossExpander;
PCF8574 lightExpander;

TrafficLight traffic;

bool isDayMode = true;

void setup()
{
  crossExpander.begin(0x20);
  lightExpander.begin(0x21);

  Serial.begin(9600);

  if (isDayMode) // to change cuz it run only once time, so always will execute setdDayModeStartParameters()
  {
    traffic.setUpTrafficLight(crossExpander, lightExpander);
    traffic.setDayModeStartParameters();
  }
  else
  {
    traffic.setUpTrafficLight(crossExpander, lightExpander);
    traffic.setNightModeStartParameters();
  }
}

void loop()
{
  if (isDayMode)
  {
    traffic.dayMode();
  }
  else
  {
    traffic.nightMode();
  }
}