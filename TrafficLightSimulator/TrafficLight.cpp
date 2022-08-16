#include <Wire.h>
#include "PCF8574.h"
#include "TrafficLight.h"

#define EXPANDER1_RED_CROSS_LR 0
#define EXPANDER1_GREEN_CROSS_LR 1
#define EXPANDER1_RED_CROSS_UB 2
#define EXPANDER1_GREEN_CROSS_UB 3

#define EXPANDER2_RED_LR 6
#define EXPANDER2_YELLOW_LR 5
#define EXPANDER2_GREEN_LR 4
#define EXPANDER2_RED_UB 3
#define EXPANDER2_YELLOW_UB 2
#define EXPANDER2_GREEN_UB 1

#define TRIGGER_1 7
#define ECHO_1 6

#define BUTTON_CROSS_L 10
#define BUTTON_CROSS_B 11

PCF8574 _crossExpander;
PCF8574 _lightExpander;

const int crossExpanderDiodes[] = {EXPANDER1_RED_CROSS_LR, EXPANDER1_GREEN_CROSS_LR, EXPANDER1_RED_CROSS_UB, EXPANDER1_GREEN_CROSS_UB};
const int lightExpanderDiodes[] = {EXPANDER2_RED_LR, EXPANDER2_YELLOW_LR, EXPANDER2_GREEN_LR, EXPANDER2_RED_UB, EXPANDER2_YELLOW_UB, EXPANDER2_GREEN_UB};

bool wasUsedSonar = false;

bool isOnLeftCross = false;
bool isOnBottomCross = true;

void TrafficLight::setUpTrafficLight(PCF8574 crossExpander, PCF8574 lightExpander)
{
  _lightExpander = lightExpander;
  _crossExpander = crossExpander;

  pinMode(TRIGGER_1, OUTPUT);
  pinMode(ECHO_1, INPUT);

  pinMode(BUTTON_CROSS_L, INPUT_PULLUP);
  pinMode(BUTTON_CROSS_B, INPUT_PULLUP);

  int crossExpanderLength = sizeof(crossExpanderDiodes) / sizeof(*crossExpanderDiodes);
  for (int index = 0; index < crossExpanderLength; index++)
  {
    _crossExpander.pinMode(crossExpanderDiodes[index], OUTPUT);
  }

  int lightExpanderLength = sizeof(lightExpanderDiodes) / sizeof(*lightExpanderDiodes);
  for (int index = 0; index < lightExpanderLength; index++)
  {
    _lightExpander.pinMode(lightExpanderDiodes[index], OUTPUT);
  }
}

int getDistance()
{
  delay(1000);
  long time, distance;

  digitalWrite(TRIGGER_1, LOW);
  delayMicroseconds(20);
  digitalWrite(TRIGGER_1, HIGH);
  delayMicroseconds(100);
  digitalWrite(TRIGGER_1, LOW);

  time = pulseIn(ECHO_1, HIGH);
  distance = time / 58;

  return distance;
}

void nightModeTurnOnRedDiodeAndOffGreen(int greenDiode, int yellowDiode, int redDiode)
{
  _lightExpander.digitalWrite(greenDiode, LOW);

  _lightExpander.digitalWrite(yellowDiode, HIGH);
  delay(2000);
  _lightExpander.digitalWrite(yellowDiode, LOW);
  _lightExpander.digitalWrite(redDiode, HIGH);
}

void nightModeTurnOnGreenDiodeAndOffRed(int greenDiode, int yellowDiode, int redDiode)
{

  _lightExpander.digitalWrite(redDiode, LOW);
  _lightExpander.digitalWrite(yellowDiode, HIGH);
  delay(500);

  _lightExpander.digitalWrite(yellowDiode, LOW);
  _lightExpander.digitalWrite(greenDiode, HIGH);
}

void nightModeTurnOnCrossLightAfterButtonClick(int crossGreenDiode, int crossRedDiode, int lightGreenDiode, int lightYellowDiode, int lightRedDiode)
{
  nightModeTurnOnRedDiodeAndOffGreen(lightGreenDiode, lightYellowDiode, lightRedDiode);

  _crossExpander.digitalWrite(crossRedDiode, LOW);
  _crossExpander.digitalWrite(crossGreenDiode, HIGH);

  delay(5000);

  _crossExpander.digitalWrite(crossRedDiode, HIGH);
  _crossExpander.digitalWrite(crossGreenDiode, LOW);

  nightModeTurnOnGreenDiodeAndOffRed(lightGreenDiode, lightYellowDiode, lightRedDiode);
}

void nightModeTurnOnLightAfterAppropriateDistance(int crossGreenDiode, int crossRedDiode, int lightGreenDiode, int lightYellowDiode, int lightRedDiode)
{
  int distance = getDistance();

  Serial.println(distance);

  if (distance < 5)
  {
    if (wasUsedSonar)
    {
      return;
    }

    _crossExpander.digitalWrite(crossRedDiode, HIGH);
    _crossExpander.digitalWrite(crossGreenDiode, LOW);

    nightModeTurnOnGreenDiodeAndOffRed(lightGreenDiode, lightYellowDiode, lightRedDiode);

    wasUsedSonar = true;

    return;
  }

  if (wasUsedSonar && distance > 5)
  {
    wasUsedSonar = false;

    nightModeTurnOnRedDiodeAndOffGreen(lightGreenDiode, lightYellowDiode, lightRedDiode);

    _crossExpander.digitalWrite(crossRedDiode, LOW);
    _crossExpander.digitalWrite(crossGreenDiode, HIGH);
  }
}

void dayModeTurnOnGreenDiodeAndOffRed(int greenDiode, int yellowDiode, int redDiode)
{
  _lightExpander.digitalWrite(redDiode, LOW);
  _lightExpander.digitalWrite(yellowDiode, HIGH);
  delay(500);

  _lightExpander.digitalWrite(yellowDiode, LOW);
  _lightExpander.digitalWrite(greenDiode, HIGH);
}

void dayModeTurnOnRedDiodeAndOffGreen(int greenDiode, int yellowDiode, int redDiode)
{
  _lightExpander.digitalWrite(greenDiode, LOW);
  delay(200);

  _lightExpander.digitalWrite(yellowDiode, HIGH);
  delay(2000);
  _lightExpander.digitalWrite(yellowDiode, LOW);
  _lightExpander.digitalWrite(redDiode, HIGH);
}

void dayModeTurnOnRedZebraCrossControlledByTime(int greenDiode, int redDiode)
{
  _crossExpander.digitalWrite(greenDiode, LOW);
  _crossExpander.digitalWrite(redDiode, HIGH);

  return;
}

void dayModeTurnOnGreenZebraCrossControlledByTime(int greenDiode, int redDiode)
{
  _crossExpander.digitalWrite(redDiode, LOW);
  _crossExpander.digitalWrite(greenDiode, HIGH);

  return;
}

void TrafficLight::setNightModeStartParameters()
{
  // setup traffic lights
  _lightExpander.digitalWrite(EXPANDER2_GREEN_LR, HIGH);
  _lightExpander.digitalWrite(EXPANDER2_RED_UB, HIGH);

  // setup zebra cross
  _crossExpander.digitalWrite(EXPANDER1_RED_CROSS_LR, HIGH);
  _crossExpander.digitalWrite(EXPANDER1_GREEN_CROSS_UB, HIGH);
}

void TrafficLight::setDayModeStartParameters()
{

  // setting traffic light
  _lightExpander.digitalWrite(EXPANDER2_GREEN_LR, HIGH);
  // digitalWrite(GREEN_R, HIGH);
  _lightExpander.digitalWrite(EXPANDER2_RED_UB, HIGH);

  // setting zebra cross for left up
  _crossExpander.digitalWrite(EXPANDER1_RED_CROSS_LR, HIGH);

  // setting zebra cross for down
  _crossExpander.digitalWrite(EXPANDER1_GREEN_CROSS_UB, HIGH);

  // setting center zebra cross
  // digitalWrite(RED_CROSS_CENTER, HIGH);
}

void TrafficLight::dayMode()
{
  delay(2000); // waiting for cars to go left-right
  dayModeTurnOnRedDiodeAndOffGreen(EXPANDER2_GREEN_LR, EXPANDER2_YELLOW_LR, EXPANDER2_RED_LR);
  // dayModeTurnOnGreenZebraCrossControlledByTime(GREEN_CROSS_CENTER, RED_CROSS_CENTER);
  // dayModeTurnOnRedDiodeAndOffGreen(GREEN_R, YELLOW_R, RED_R);

  // turn on zebra lights
  dayModeTurnOnGreenZebraCrossControlledByTime(EXPANDER1_GREEN_CROSS_LR, EXPANDER1_RED_CROSS_LR);
  dayModeTurnOnRedZebraCrossControlledByTime(EXPANDER1_GREEN_CROSS_UB, EXPANDER1_RED_CROSS_UB);

  delay(2000);
  dayModeTurnOnGreenDiodeAndOffRed(EXPANDER2_GREEN_UB, EXPANDER2_YELLOW_UB, EXPANDER2_RED_UB); // waiting for cars to go up-bottom

  delay(2000);
  dayModeTurnOnRedDiodeAndOffGreen(EXPANDER2_GREEN_UB, EXPANDER2_YELLOW_UB, EXPANDER2_RED_UB);

  dayModeTurnOnGreenZebraCrossControlledByTime(EXPANDER1_GREEN_CROSS_UB, EXPANDER1_RED_CROSS_UB);
  dayModeTurnOnRedZebraCrossControlledByTime(EXPANDER1_GREEN_CROSS_LR, EXPANDER1_RED_CROSS_LR);

  delay(2000);
  // dayModeTurnOnRedZebraCrossControlledByTime(GREEN_CROSS_CENTER, RED_CROSS_CENTER);
  dayModeTurnOnGreenDiodeAndOffRed(EXPANDER2_GREEN_LR, EXPANDER2_YELLOW_LR, EXPANDER2_RED_LR);
  // dayModeTurnOnGreenDiodeAndOffRed(GREEN_R, YELLOW_R, RED_R);
}

void TrafficLight::nightMode()
{
  if (digitalRead(BUTTON_CROSS_L) == LOW)
  {
    isOnLeftCross = true;
  }
  if (digitalRead(BUTTON_CROSS_B) == LOW)
  {
    isOnBottomCross = true;
  }

  if (isOnLeftCross)
  {
    nightModeTurnOnCrossLightAfterButtonClick(EXPANDER1_GREEN_CROSS_LR, EXPANDER1_RED_CROSS_LR, EXPANDER2_GREEN_LR, EXPANDER2_YELLOW_LR, EXPANDER2_RED_LR);
    isOnLeftCross = false;
  }

  nightModeTurnOnLightAfterAppropriateDistance(EXPANDER1_GREEN_CROSS_UB, EXPANDER1_RED_CROSS_UB, EXPANDER2_GREEN_UB, EXPANDER2_YELLOW_UB, EXPANDER2_RED_UB);
}