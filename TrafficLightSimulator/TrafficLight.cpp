#include "TrafficLight.h"

void TrafficLight::setDiodeToIndex(int diode, int index)
{
    _diodes[index] = diode;
}

void TrafficLight::setPinModes()
{
    int diodesLength = sizeof(_diodes) / sizeof(*_diodes);

    for (int i = 0; i < diodesLength; i++)
    {
        pinMode(_diodes[i], OUTPUT);
        delay(1);
    }
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
        digitalWrite(redDiode, LOW);
        digitalWrite(yellowDiode, HIGH);
        delay(500);
    }

    if (!isHighDiodeState(redDiode) && isHighDiodeState(yellowDiode))
    {
        digitalWrite(yellowDiode, LOW);
        digitalWrite(greenDiode, HIGH);
    }
}

void TrafficLight::turnOnRedDiodeAndOffGreen(int greenDiode, int yellowDiode, int redDiode)
{
    
  if (isHighDiodeState(greenDiode))
  {
    digitalWrite(greenDiode, LOW);
    delay(200);
  }

  if (!isHighDiodeState(yellowDiode) && !isHighDiodeState(redDiode))
  {
    digitalWrite(yellowDiode, HIGH);
    delay(2000);
    digitalWrite(yellowDiode, LOW);
    digitalWrite(redDiode, HIGH);
  }
}

void TrafficLight::turnOnZebraCrossControlledByTime(int greenDiode, int redDiode)
{
    
  if (isHighDiodeState(greenDiode))
  {
    digitalWrite(greenDiode, LOW);
    digitalWrite(redDiode, HIGH);

    return;
  }

  if (isHighDiodeState(redDiode))
  {
    digitalWrite(redDiode, LOW);
    digitalWrite(greenDiode, HIGH);

    return;
  }
}

void TrafficLight::turnOnDiodes() // only for tests purpose, to delete
{
    int diodesLength = sizeof(_diodes) / sizeof(*_diodes);

    for (int i = 0; i < diodesLength; i++)
    {
        digitalWrite(_diodes[i], HIGH);
        delay(1);
    }
}

