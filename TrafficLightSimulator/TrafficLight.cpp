#include <Wire.h>
#include "PCF8574.h"
#include "TrafficLight.h"

#define CE1_P0_R 0
#define CE1_P1_G 1
#define CE1_P2_R 2
#define CE1_P3_G 3

#define LE2_P6_R 6
#define LE2_P5_Y 5
#define LE2_P4_G 4
#define LE2_P2_R 2
#define LE2_P1_Y 1
#define LE2_P0_G 0

#define CE2_P0_R 0
#define CE2_P1_G 1
#define CE2_P2_R 2
#define CE2_P3_G 3
#define CE2_P5_R 5
#define CE2_P6_G 6

#define LE1_P6_R 6
#define LE1_P5_Y 5
#define LE1_P4_G 4
#define LE1_P2_R 2
#define LE1_P1_Y 1
#define LE1_P0_G 0

#define U1_TRIGGER 13
#define U1_ECHO 12

#define U2_TRIGGER 5
#define U2_ECHO 4

#define U3_TRIGGER 11
#define U3_ECHO 10

#define U4_TRIGGER 3
#define U4_ECHO 2

#define B1 8
#define B2 7
#define B3 6

PCF8574 _CE1;
PCF8574 _LE2;
PCF8574 _CE2;
PCF8574 _LE1;

const int CROSS_EXPANDER1_DIODES[] = {CE1_P0_R, CE1_P1_G, CE1_P2_R, CE1_P3_G};
const int LIGHT_EXPANDER2_DIODES[] = {LE2_P6_R, LE2_P5_Y, LE2_P4_G, LE2_P2_R, LE2_P1_Y, LE2_P0_G};
const int CROSS_EXPANDER2_DIODES[] = {CE2_P0_R, CE2_P1_G, CE2_P2_R, CE2_P3_G, CE2_P5_R, CE2_P6_G};
const int LIGHT_EXPANDER1_DIODES[] = {LE1_P6_R, LE1_P5_Y, LE1_P4_G, LE1_P2_R, LE1_P1_Y, LE1_P0_G};

bool _wasUsedSonar1 = false;
bool _wasUsedSonar2 = false;

bool _isOnLeftCross = false;
bool _isOnBottomCross = false;
bool _isOnRightCross = false;

void TrafficLight::setUpTrafficLight(PCF8574 CE1, PCF8574 LE2, PCF8574 CE2, PCF8574 LE1)
{
  _LE2 = LE2;
  _CE1 = CE1;
  _LE1 = LE1;
  _CE2 = CE2;

  pinMode(U1_TRIGGER, OUTPUT);
  pinMode(U1_ECHO, INPUT);
  pinMode(U2_TRIGGER, OUTPUT);
  pinMode(U2_ECHO, INPUT);
  pinMode(U3_TRIGGER, OUTPUT);
  pinMode(U3_ECHO, INPUT);
  pinMode(U4_TRIGGER, OUTPUT);
  pinMode(U4_ECHO, INPUT);

  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);

  int crossExpander1Length = sizeof(CROSS_EXPANDER1_DIODES) / sizeof(*CROSS_EXPANDER1_DIODES);
  for (int index = 0; index < crossExpander1Length; index++)
  {
    _CE1.pinMode(CROSS_EXPANDER1_DIODES[index], OUTPUT);
  }

  int lightExpander2Length = sizeof(LIGHT_EXPANDER2_DIODES) / sizeof(*LIGHT_EXPANDER2_DIODES);
  for (int index = 0; index < lightExpander2Length; index++)
  {
    _LE2.pinMode(LIGHT_EXPANDER2_DIODES[index], OUTPUT);
  }

  int crossExpander2Length = sizeof(CROSS_EXPANDER2_DIODES) / sizeof(*CROSS_EXPANDER2_DIODES);
  for (int index = 0; index < crossExpander2Length; index++)
  {
    _CE2.pinMode(CROSS_EXPANDER2_DIODES[index], OUTPUT);
  }

  int lightExpander1Length = sizeof(LIGHT_EXPANDER1_DIODES) / sizeof(*LIGHT_EXPANDER1_DIODES);
  for (int index = 0; index < lightExpander1Length; index++)
  {
    _LE1.pinMode(LIGHT_EXPANDER1_DIODES[index], OUTPUT);
  }
}

int getDistance(int trigger, int echo)
{
  delay(1000);
  long time, distance;

  digitalWrite(trigger, LOW);
  delayMicroseconds(20);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(100);
  digitalWrite(trigger, LOW);

  time = pulseIn(echo, HIGH);
  distance = time / 58;

  return distance;
}

void nightModeTurnOnRedDiodeAndOffGreen(PCF8574 expander, int greenDiode, int yellowDiode, int redDiode)
{
  expander.digitalWrite(greenDiode, LOW);

  expander.digitalWrite(yellowDiode, HIGH);
  delay(2000);
  expander.digitalWrite(yellowDiode, LOW);
  expander.digitalWrite(redDiode, HIGH);
}

void nightModeTurnOnGreenDiodeAndOffRed(PCF8574 expander, int greenDiode, int yellowDiode, int redDiode)
{

  expander.digitalWrite(redDiode, LOW);
  expander.digitalWrite(yellowDiode, HIGH);
  delay(500);

  expander.digitalWrite(yellowDiode, LOW);
  expander.digitalWrite(greenDiode, HIGH);
}

void nightModeTurnOnCrossLightAfterButtonClick(PCF8574 expander, int crossGreenDiode, int crossRedDiode, int lightGreenDiode, int lightYellowDiode, int lightRedDiode)
{
  nightModeTurnOnRedDiodeAndOffGreen(expander, lightGreenDiode, lightYellowDiode, lightRedDiode);

  expander.digitalWrite(crossRedDiode, LOW);
  expander.digitalWrite(crossGreenDiode, HIGH);

  delay(5000);

  expander.digitalWrite(crossRedDiode, HIGH);
  expander.digitalWrite(crossGreenDiode, LOW);

  nightModeTurnOnGreenDiodeAndOffRed(expander, lightGreenDiode, lightYellowDiode, lightRedDiode);
}

void nightModeTurnOnLightAfterAppropriateDistance(PCF8574 crossExpander, PCF8574 lightExpander, int crossGreenDiode, int crossRedDiode, int lightGreenDiode, int lightYellowDiode, int lightRedDiode, int trigger, int echo, bool wasUsedSonar)
{
  int distance = getDistance(trigger, echo);

  if (distance < 5)
  {
    if (wasUsedSonar)
    {
      return;
    }

    crossExpander.digitalWrite(crossRedDiode, HIGH);
    crossExpander.digitalWrite(crossGreenDiode, LOW);

    nightModeTurnOnGreenDiodeAndOffRed(lightExpander, lightGreenDiode, lightYellowDiode, lightRedDiode);

    wasUsedSonar = true;

    return;
  }

  if (wasUsedSonar && distance > 5)
  {
    wasUsedSonar = false;

    nightModeTurnOnRedDiodeAndOffGreen(lightExpander, lightGreenDiode, lightYellowDiode, lightRedDiode);

    crossExpander.digitalWrite(crossRedDiode, LOW);
    crossExpander.digitalWrite(crossGreenDiode, HIGH);
  }
}

void dayModeTurnOnGreenDiodeAndOffRed(PCF8574 expander, int greenDiode, int yellowDiode, int redDiode)
{
  expander.digitalWrite(redDiode, LOW);
  expander.digitalWrite(yellowDiode, HIGH);
  delay(500);

  expander.digitalWrite(yellowDiode, LOW);
  expander.digitalWrite(greenDiode, HIGH);
}

void dayModeTurnOnRedDiodeAndOffGreen(PCF8574 expander, int greenDiode, int yellowDiode, int redDiode)
{
  expander.digitalWrite(greenDiode, LOW);
  delay(200);

  expander.digitalWrite(yellowDiode, HIGH);
  delay(2000);
  expander.digitalWrite(yellowDiode, LOW);
  expander.digitalWrite(redDiode, HIGH);
}

void dayModeTurnOnRedZebraCrossControlledByTime(PCF8574 expander, int greenDiode, int redDiode)
{
  expander.digitalWrite(greenDiode, LOW);
  expander.digitalWrite(redDiode, HIGH);

  return;
}

void dayModeTurnOnGreenZebraCrossControlledByTime(PCF8574 expander, int greenDiode, int redDiode)
{
  expander.digitalWrite(redDiode, LOW);
  expander.digitalWrite(greenDiode, HIGH);

  return;
}

void TrafficLight::setNightModeStartParameters()
{
  // setup traffic lights
  _LE2.digitalWrite(LE2_P4_G, HIGH);
  _LE1.digitalWrite(LE1_P4_G, HIGH);
  _LE2.digitalWrite(LE2_P2_R, HIGH);
  _LE1.digitalWrite(LE1_P2_R, HIGH);

  // setup zebra cross
  _CE1.digitalWrite(CE1_P0_R, HIGH);
  _CE2.digitalWrite(CE2_P0_R, HIGH);
  _CE1.digitalWrite(CE1_P3_G, HIGH);
  _CE2.digitalWrite(CE2_P3_G, HIGH);
}

void TrafficLight::setDayModeStartParameters()
{

  // setting traffic light
  _LE2.digitalWrite(LE2_P4_G, HIGH);
  _LE1.digitalWrite(LE1_P4_G, HIGH);
  // digitalWrite(GREEN_R, HIGH);
  _LE2.digitalWrite(LE2_P2_R, HIGH);
  _LE1.digitalWrite(LE1_P2_R, HIGH);

  // setting zebra cross for left up
  _CE1.digitalWrite(CE1_P0_R, HIGH);
  _CE2.digitalWrite(CE2_P0_R, HIGH);

  // setting zebra cross for down
  _CE1.digitalWrite(CE1_P3_G, HIGH);
  _CE2.digitalWrite(CE2_P3_G, HIGH);

  // setting center zebra cross
  _CE2.digitalWrite(CE2_P5_R, HIGH);
  _CE2.digitalWrite(CE2_P6_G, HIGH);

}

void TrafficLight::dayMode()
{
  delay(2000); // waiting for cars to go left-right
  dayModeTurnOnRedDiodeAndOffGreen(_LE1, LE1_P4_G, LE1_P5_Y, LE1_P6_R);
  dayModeTurnOnRedDiodeAndOffGreen(_LE2, LE2_P4_G, LE2_P5_Y, LE2_P6_R);

  // turn on zebra lights
  dayModeTurnOnGreenZebraCrossControlledByTime(_CE1, CE1_P1_G, CE1_P0_R);
  dayModeTurnOnGreenZebraCrossControlledByTime(_CE2, CE2_P6_G, CE2_P5_R);
  dayModeTurnOnGreenZebraCrossControlledByTime(_CE2, CE2_P1_G, CE2_P0_R);

  dayModeTurnOnRedZebraCrossControlledByTime(_CE1, CE1_P3_G, CE1_P2_R);
  dayModeTurnOnRedZebraCrossControlledByTime(_CE2, CE2_P3_G, CE2_P2_R);

  delay(2000);
  dayModeTurnOnGreenDiodeAndOffRed(_LE1, LE1_P0_G, LE1_P1_Y, LE1_P2_R);// waiting for cars to go up-bottom
  dayModeTurnOnGreenDiodeAndOffRed(_LE2, LE2_P0_G, LE2_P1_Y, LE2_P2_R); 

  delay(2000);
  dayModeTurnOnRedDiodeAndOffGreen(_LE1, LE1_P0_G, LE1_P1_Y, LE1_P2_R);
  dayModeTurnOnRedDiodeAndOffGreen(_LE2, LE2_P0_G, LE2_P1_Y, LE2_P2_R);

  dayModeTurnOnGreenZebraCrossControlledByTime(_CE1, CE1_P3_G, CE1_P2_R);
  dayModeTurnOnGreenZebraCrossControlledByTime(_CE1, CE2_P3_G, CE2_P2_R);

  dayModeTurnOnRedZebraCrossControlledByTime(_CE1, CE1_P1_G, CE1_P0_R);
  dayModeTurnOnRedZebraCrossControlledByTime(_CE2, CE2_P6_G, CE2_P5_R);
  dayModeTurnOnRedZebraCrossControlledByTime(_CE2, CE2_P1_G, CE2_P0_R);

  delay(2000);
  dayModeTurnOnGreenDiodeAndOffRed(_LE1, LE1_P4_G, LE1_P5_Y, LE1_P6_R);
  dayModeTurnOnGreenDiodeAndOffRed(_LE2, LE2_P4_G, LE2_P5_Y, LE2_P6_R);
}

void TrafficLight::nightMode()
{
  if (digitalRead(B1) == LOW)
  {
    _isOnLeftCross = true;
  }
  if (digitalRead(B2) == LOW)
  {
    _isOnBottomCross = true;
  }
  if (digitalRead(B3) == LOW)
  {
    _isOnRightCross = true;
  }

  if (_isOnLeftCross)
  {
    nightModeTurnOnCrossLightAfterButtonClick(_CE1, CE1_P1_G, CE1_P0_R, LE2_P4_G, LE2_P5_Y, LE2_P6_R);
    _isOnLeftCross = false;
  }
  if (_isOnBottomCross)
  {
    nightModeTurnOnCrossLightAfterButtonClick(_CE2, CE2_P6_G, CE2_P5_R, LE2_P4_G, LE2_P5_Y, LE2_P6_R);
    _isOnLeftCross = false;
  }
  if (_isOnRightCross)
  {
    nightModeTurnOnCrossLightAfterButtonClick(_CE2, CE2_P1_G, CE2_P0_R, LE2_P4_G, LE2_P5_Y, LE2_P6_R);
    _isOnLeftCross = false;
  }

  nightModeTurnOnLightAfterAppropriateDistance(_CE1, _LE1, CE1_P3_G, CE1_P2_R, LE1_P0_G, LE1_P1_Y, LE1_P2_R, U3_TRIGGER, U3_ECHO, _wasUsedSonar1);
  nightModeTurnOnLightAfterAppropriateDistance(_CE1, _LE1, CE1_P3_G, CE1_P2_R, LE1_P0_G, LE1_P1_Y, LE1_P2_R, U4_TRIGGER, U4_ECHO, _wasUsedSonar1);
  nightModeTurnOnLightAfterAppropriateDistance(_CE2, _LE2, CE2_P3_G, CE2_P2_R, LE2_P0_G, LE2_P1_Y, LE2_P2_R, U1_TRIGGER, U1_ECHO, _wasUsedSonar2);
  nightModeTurnOnLightAfterAppropriateDistance(_CE2, _LE2, CE2_P3_G, CE2_P2_R, LE2_P0_G, LE2_P1_Y, LE2_P2_R, U2_TRIGGER, U2_ECHO, _wasUsedSonar2);
}