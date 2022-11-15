#include <Wire.h>
#include "TrafficLight.h"
#include "PCF8574.h"

PCF8574 CE1; //CE1
PCF8574 LE2;
PCF8574 CE2;
PCF8574 LE1;

TrafficLight traffic;

bool isDayMode = true;

void setup()
{
  CE1.begin(0x20); //CE1
  LE2.begin(0x24); // LE2
  CE2.begin(0x22); // CE2
  LE1.begin(0x21); //LE1

  Serial.begin(9600);

  if(isDayMode)
  {
    traffic.setUpTrafficLight(CE1, LE2, CE2, LE1);
    traffic.setDayModeStartParameters();
  }

  else
  {
    traffic.setUpTrafficLight(CE1, LE2, CE2, LE1);
    //traffic.setUpTrafficLight(CE2, LE1);
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