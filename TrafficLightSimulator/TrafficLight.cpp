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

const int crossExpander1Diodes[] = {CE1_P0_R, CE1_P1_G, CE1_P2_R, CE1_P3_G};
const int lightExpander2Diodes[] = {LE2_P6_R, LE2_P5_Y, LE2_P4_G, LE2_P2_R, LE2_P1_Y, LE2_P0_G};
const int crossExpander2Diodes[] = {CE2_P0_R, CE2_P1_G, CE2_P2_R, CE2_P3_G, CE2_P5_R, CE2_P6_G};
const int lightExpander1Diodes[] = {LE1_P6_R, LE1_P5_Y, LE1_P4_G, LE1_P2_R, LE1_P1_Y, LE1_P0_G};

bool wasUsedSonar = false;

bool isOnLeftCross = false;
bool isOnBottomCross = true;

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

  int crossExpander1Length = sizeof(crossExpander1Diodes) / sizeof(*crossExpander1Diodes);
  for (int index = 0; index < crossExpander1Length; index++)
  {
    _CE1.pinMode(crossExpander1Diodes[index], OUTPUT);
  }

  int lightExpander2Length = sizeof(lightExpander2Diodes) / sizeof(*lightExpander2Diodes);
  for (int index = 0; index < lightExpander2Length; index++)
  {
    _LE2.pinMode(lightExpander2Diodes[index], OUTPUT);
  }

  int crossExpander2Length = sizeof(crossExpander2Diodes) / sizeof(*crossExpander2Diodes);
  for (int index = 0; index < crossExpander2Length; index++)
  {
    _CE2.pinMode(crossExpander2Diodes[index], OUTPUT);
  }

  int lightExpander1Length = sizeof(lightExpander1Diodes) / sizeof(*lightExpander1Diodes);
  for (int index = 0; index < lightExpander1Length; index++)
  {
    _LE1.pinMode(lightExpander1Diodes[index], OUTPUT);
  }
}

// int getDistance()
// {
//   delay(1000);
//   long time, distance;

//   digitalWrite(TRIGGER_1, LOW);
//   delayMicroseconds(20);
//   digitalWrite(TRIGGER_1, HIGH);
//   delayMicroseconds(100);
//   digitalWrite(TRIGGER_1, LOW);

//   time = pulseIn(ECHO_1, HIGH);
//   distance = time / 58;

//   return distance;
// }

void nightModeTurnOnRedDiodeAndOffGreen(int greenDiode, int yellowDiode, int redDiode)
{
  _LE2.digitalWrite(greenDiode, LOW);

  _LE2.digitalWrite(yellowDiode, HIGH);
  delay(2000);
  _LE2.digitalWrite(yellowDiode, LOW);
  _LE2.digitalWrite(redDiode, HIGH);
}

void nightModeTurnOnGreenDiodeAndOffRed(int greenDiode, int yellowDiode, int redDiode)
{

  _LE2.digitalWrite(redDiode, LOW);
  _LE2.digitalWrite(yellowDiode, HIGH);
  delay(500);

  _LE2.digitalWrite(yellowDiode, LOW);
  _LE2.digitalWrite(greenDiode, HIGH);
}

void nightModeTurnOnCrossLightAfterButtonClick(int crossGreenDiode, int crossRedDiode, int lightGreenDiode, int lightYellowDiode, int lightRedDiode)
{
  nightModeTurnOnRedDiodeAndOffGreen(lightGreenDiode, lightYellowDiode, lightRedDiode);

  _CE1.digitalWrite(crossRedDiode, LOW);
  _CE1.digitalWrite(crossGreenDiode, HIGH);

  delay(5000);

  _CE1.digitalWrite(crossRedDiode, HIGH);
  _CE1.digitalWrite(crossGreenDiode, LOW);

  nightModeTurnOnGreenDiodeAndOffRed(lightGreenDiode, lightYellowDiode, lightRedDiode);
}

/*void nightModeTurnOnLightAfterAppropriateDistance(int crossGreenDiode, int crossRedDiode, int lightGreenDiode, int lightYellowDiode, int lightRedDiode)
{
  int distance = getDistance();

  Serial.println(distance);

  if (distance < 5)
  {
    if (wasUsedSonar)
    {
      return;
    }

    _CE1.digitalWrite(crossRedDiode, HIGH);
    _CE1.digitalWrite(crossGreenDiode, LOW);

    nightModeTurnOnGreenDiodeAndOffRed(lightGreenDiode, lightYellowDiode, lightRedDiode);

    wasUsedSonar = true;

    return;
  }

  if (wasUsedSonar && distance > 5)
  {
    wasUsedSonar = false;

    nightModeTurnOnRedDiodeAndOffGreen(lightGreenDiode, lightYellowDiode, lightRedDiode);

    _CE1.digitalWrite(crossRedDiode, LOW);
    _CE1.digitalWrite(crossGreenDiode, HIGH);
  }
}
*/

void dayModeTurnOnGreenDiodeAndOffRed(int greenDiode, int yellowDiode, int redDiode)
{
  _LE2.digitalWrite(redDiode, LOW);
  _LE2.digitalWrite(yellowDiode, HIGH);
  delay(500);

  _LE2.digitalWrite(yellowDiode, LOW);
  _LE2.digitalWrite(greenDiode, HIGH);
}

void dayModeTurnOnRedDiodeAndOffGreen(int greenDiode, int yellowDiode, int redDiode)
{
  _LE2.digitalWrite(greenDiode, LOW);
  delay(200);

  _LE2.digitalWrite(yellowDiode, HIGH);
  delay(2000);
  _LE2.digitalWrite(yellowDiode, LOW);
  _LE2.digitalWrite(redDiode, HIGH);
}

void dayModeTurnOnRedZebraCrossControlledByTime(int greenDiode, int redDiode)
{
  _CE1.digitalWrite(greenDiode, LOW);
  _CE1.digitalWrite(redDiode, HIGH);

  return;
}

void dayModeTurnOnGreenZebraCrossControlledByTime(int greenDiode, int redDiode)
{
  _CE1.digitalWrite(redDiode, LOW);
  _CE1.digitalWrite(greenDiode, HIGH);

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
  dayModeTurnOnRedDiodeAndOffGreen(LE2_P4_G, LE2_P5_Y, LE2_P6_R);
  dayModeTurnOnRedDiodeAndOffGreen(LE1_P4_G, LE1_P5_Y, LE1_P6_R);

  // turn on zebra lights
  dayModeTurnOnGreenZebraCrossControlledByTime(CE1_P1_G, CE1_P0_R);
  dayModeTurnOnGreenZebraCrossControlledByTime(CE2_P6_G, CE2_P5_R);
  dayModeTurnOnGreenZebraCrossControlledByTime(CE2_P1_G, CE2_P0_R);

  dayModeTurnOnRedZebraCrossControlledByTime(CE1_P3_G, CE1_P2_R);
  dayModeTurnOnRedZebraCrossControlledByTime(CE2_P3_G, CE2_P2_R);

  delay(2000);
  dayModeTurnOnGreenDiodeAndOffRed(LE2_P0_G, LE2_P1_Y, LE2_P2_R); // waiting for cars to go up-bottom
  dayModeTurnOnGreenDiodeAndOffRed(LE1_P0_G, LE1_P1_Y, LE1_P2_R);

  delay(2000);
  dayModeTurnOnRedDiodeAndOffGreen(LE2_P0_G, LE2_P1_Y, LE2_P2_R);
  dayModeTurnOnRedDiodeAndOffGreen(LE1_P0_G, LE1_P1_Y, LE1_P2_R);

  dayModeTurnOnGreenZebraCrossControlledByTime(CE1_P3_G, CE1_P2_R);
  dayModeTurnOnGreenZebraCrossControlledByTime(CE2_P3_G, CE2_P2_R);

  dayModeTurnOnRedZebraCrossControlledByTime(CE1_P1_G, CE1_P0_R);
  dayModeTurnOnRedZebraCrossControlledByTime(CE2_P6_G, CE2_P5_R);
  dayModeTurnOnRedZebraCrossControlledByTime(CE2_P1_G, CE2_P0_R);

  delay(2000);
  dayModeTurnOnGreenDiodeAndOffRed(LE2_P4_G, LE2_P5_Y, LE2_P6_R);
  dayModeTurnOnGreenDiodeAndOffRed(LE1_P4_G, LE1_P5_Y, LE1_P6_R);
}

void TrafficLight::nightMode()
{
  if (digitalRead(B1) == LOW)
  {
    isOnLeftCross = true;
  }
  if (digitalRead(B2) == LOW)
  {
    isOnBottomCross = true;
  }

  if (isOnLeftCross)
  {
    nightModeTurnOnCrossLightAfterButtonClick(CE1_P1_G, CE1_P0_R, LE2_P4_G, LE2_P5_Y, LE2_P6_R);
    isOnLeftCross = false;
  }

  //nightModeTurnOnLightAfterAppropriateDistance(CE1_P3_G, CE1_P2_R, LE2_P0_G, LE2_P1_Y, LE2_P2_R);
}