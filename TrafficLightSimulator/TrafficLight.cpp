#include <Wire.h>
#include "PCF8574.h"
#include "TrafficLight.h"

TrafficLight::TrafficLight(PCF8574 crossExpander, PCF8574 lightExpander)
{
  _crossExpander = crossExpander;
  _lightExpander = lightExpander;
}

bool isHighDiodeState(int diode)
{
  if (digitalRead(diode) == HIGH)
  {
    return true;
  }
  return false;
}

void TrafficLight::turnOnGreenDiodeAndOffRed(int greenDiode, int yellowDiode, int redDiode)
{
  if (isHighDiodeState(redDiode) == true)
  {
    _lightExpander.digitalWrite(redDiode, LOW);
    _lightExpander.digitalWrite(yellowDiode, HIGH);
    delay(500);
  }

  if (!isHighDiodeState(redDiode) && isHighDiodeState(yellowDiode))
  {
    _lightExpander.digitalWrite(yellowDiode, LOW);
    _lightExpander.digitalWrite(greenDiode, HIGH);
  }
}

void TrafficLight::turnOnRedDiodeAndOffGreen(int greenDiode, int yellowDiode, int redDiode)
{
  if (isHighDiodeState(greenDiode))
  {
    _lightExpander.digitalWrite(greenDiode, LOW);
    delay(200);
  }

  if (!isHighDiodeState(yellowDiode) && !isHighDiodeState(redDiode))
  {
    _lightExpander.digitalWrite(yellowDiode, HIGH);
    delay(2000);
    _lightExpander.digitalWrite(yellowDiode, LOW);
    _lightExpander.digitalWrite(redDiode, HIGH);
  }
}

void TrafficLight::turnOnRedZebraCrossControlledByTime(int greenDiode, int redDiode)
{
  if (isHighDiodeState(greenDiode))
  {
    _crossExpander.digitalWrite(greenDiode, LOW);
    _crossExpander.digitalWrite(redDiode, HIGH);

    return;
  }
}

void TrafficLight::turnOnGreenZebraCrossControlledByTime(int greenDiode, int redDiode)
{
  if (isHighDiodeState(redDiode))
  {
    _crossExpander.digitalWrite(redDiode, LOW);
    _crossExpander.digitalWrite(greenDiode, HIGH);

    return;
  }
}