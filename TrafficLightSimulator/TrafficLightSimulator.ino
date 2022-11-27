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
bool _isDayMode = false;
bool _lastState;

//int switchHelper[500];

void setup()
{
  CE1.begin(0x20); //CE1
  LE2.begin(0x24); // LE2
  CE2.begin(0x22); // CE2
  LE1.begin(0x21); //LE1

  Serial.begin(9600);

  pinMode(TOGGLE_MODE, INPUT_PULLUP);

  //_isDayMode = digitalRead(TOGGLE_MODE) == HIGH;

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
  Serial.println(digitalRead(TOGGLE_MODE));
//  int sum = 0;

//  for(int i = 0; i < 500; i++)
//  {
//    //Serial.println(digitalRead(TOGGLE_MODE));
//    switchHelper[i] = digitalRead(TOGGLE_MODE);
//  }

//  for(int i = 0; i < 500; i++)
//  {  
//    sum += switchHelper[i];

//    if(sum > 0)
//    {
//      _isDayMode = true;
//    }
//    else
//    {
//      _isDayMode = false;
//    }
//  }

//  for(int i = 0; i < 500; i++)
//  {
//    switchHelper[i] = 0;
//  }
 

//  if(_isDayMode)
//  {
//    Serial.println("tryb dzien");
//  }
//  else
//  {
//    Serial.println("tryb noc");
//  }
  
//   if (_isDayMode)
//   {
//     _traffic.dayMode();
//   }
//   else
//   {
//     _traffic.nightMode();
//   }
}
