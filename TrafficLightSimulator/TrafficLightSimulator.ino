#include <Wire.h>
#include "TrafficLight.h"
#include "PCF8574.h"


PCF8574 crossExpander;
PCF8574 lightExpander;
PCF8574 crossExpander2;
PCF8574 lightExpander2;

TrafficLight traffic;

bool isDayMode = true;

void setup()
{
  crossExpander.begin(0x20);
  lightExpander.begin(0x21);
  crossExpander2.begin(0x22);
  lightExpander2.begin(0x24);

  Serial.begin(9600);

  if (isDayMode) // to change cuz it run only once time, so always will execute setdDayModeStartParameters()
  {
    traffic.setUpTrafficLight(crossExpander, lightExpander, crossExpander2, lightExpander2);
    traffic.setDayModeStartParameters();
  }
  else
  {
    traffic.setUpTrafficLight(crossExpander, lightExpander, crossExpander2, lightExpander2);
    //traffic.setUpTrafficLight(crossExpander2, lightExpander2);
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