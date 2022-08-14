#include <Wire.h>
#include "TrafficLight.h"
#include "PCF8574.h"

#define RED_L 2
#define YELLOW_L 3
#define GREEN_L 4

#define RED_U_B A5
#define YELLOW_U_B A4
#define GREEN_U_B A3

#define RED_R 8
#define YELLOW_R 9
#define GREEN_R 10

#define RED_CROSS_L A0
#define GREEN_CROSS_L A1

#define GREEN_CROSS_U 5
#define RED_CROSS_U 6

#define GREEN_CROSS_CENTER 11
#define RED_CROSS_CENTER 12

#define EXPANDER1_RED_CROSS_L 0
#define EXPANDER1_GREEN_CROSS_L 1
#define EXPANDER1_RED_CROSS_B 2
#define EXPANDER1_GREEN_CROSS_B 3

#define EXPANDER2_RED_L 6
#define EXPANDER2_YELLOW_L 5
#define EXPANDER2_GREEN_L 4
#define EXPANDER2_RED_B 1
#define EXPANDER2_YELLOW_B 2
#define EXPANDER2_GREEN_B 3

PCF8574 expander1;
PCF8574 expander2;

const int diodes[] = {RED_L, YELLOW_L, GREEN_L, RED_U_B, YELLOW_U_B, GREEN_U_B, RED_R, YELLOW_R, GREEN_R, RED_CROSS_L, GREEN_CROSS_L};
const int expander1Diodes[] = {EXPANDER1_RED_CROSS_L, EXPANDER1_GREEN_CROSS_L, EXPANDER1_RED_CROSS_B, EXPANDER1_GREEN_CROSS_B};
const int expander2Diodes[] = {EXPANDER2_RED_L, EXPANDER2_YELLOW_L, EXPANDER2_GREEN_L, EXPANDER2_RED_B, EXPANDER2_YELLOW_B, EXPANDER2_GREEN_B};

TrafficLight traffic;

bool isDayMode = true;

void setup()
{
  expander1.begin(0x20);
  expander2.begin(0x21);

  Serial.begin(9600);

  int diodesLength = sizeof(diodes) / sizeof(*diodes);
  for (int index = 0; index < diodesLength; index++)
  {
    traffic.setDiodeToIndex(diodes[index], index);
  }
  traffic.setPinModes();

  int expanderDiodesLength = sizeof(expander1Diodes) / sizeof(*expander1Diodes);
  for (int index = 0; index < expanderDiodesLength; index++)
  {
    expander1.pinMode(expander1Diodes[index], OUTPUT);
  } 
  int expander2DiodesLength = sizeof(expander2Diodes) / sizeof(*expander2Diodes);
  for (int index = 0; index < expander2DiodesLength; index++)
  {
    expander2.pinMode(expander2Diodes[index], OUTPUT);
  }

  if (isDayMode) // to change cuz it run only once time, so always will execute setdDayModeStartParameters()
  {
    setDayModeStartParameters();
  }
  else
  {
    setNightModeStartParameters();
  }
}

void loop()
{
}

void turnOnNightMode()
{
  setNightModeStartParameters();
}

void turnOnDayMode()
{
  delay(2000); // waiting for cars to go left-right
  traffic.turnOnRedDiodeAndOffGreen(GREEN_L, YELLOW_L, RED_L);
  traffic.turnOnGreenZebraCrossControlledByTime(GREEN_CROSS_CENTER, RED_CROSS_CENTER);
  traffic.turnOnRedDiodeAndOffGreen(GREEN_R, YELLOW_R, RED_R);

  // turn on zebra lights
  traffic.turnOnGreenZebraCrossControlledByTime(GREEN_CROSS_L, RED_CROSS_L);
  traffic.turnOnRedZebraCrossControlledByTime(GREEN_CROSS_U, RED_CROSS_U);

  delay(2000);
  traffic.turnOnGreenDiodeAndOffRed(GREEN_U_B, YELLOW_U_B, RED_U_B); // waiting for cars to go up-bottom

  delay(2000);
  traffic.turnOnRedDiodeAndOffGreen(GREEN_U_B, YELLOW_U_B, RED_U_B);

  traffic.turnOnGreenZebraCrossControlledByTime(GREEN_CROSS_U, RED_CROSS_U);
  traffic.turnOnRedZebraCrossControlledByTime(GREEN_CROSS_L, RED_CROSS_L);

  delay(2000);
  traffic.turnOnRedZebraCrossControlledByTime(GREEN_CROSS_CENTER, RED_CROSS_CENTER);
  traffic.turnOnGreenDiodeAndOffRed(GREEN_L, YELLOW_L, RED_L);
  traffic.turnOnGreenDiodeAndOffRed(GREEN_R, YELLOW_R, RED_R);
}

void setDayModeStartParameters()
{
  // setting traffic light
  digitalWrite(GREEN_L, HIGH);
  digitalWrite(GREEN_R, HIGH);
  digitalWrite(RED_U_B, HIGH);

  // setting zebra cross for left up
  digitalWrite(RED_CROSS_L, HIGH);

  // setting zebra cross for down
  digitalWrite(GREEN_CROSS_U, HIGH);

  // setting center zebra cross
  digitalWrite(RED_CROSS_CENTER, HIGH);
}

void setNightModeStartParameters()
{
  // setup traffic lights
  digitalWrite(GREEN_L, HIGH);
  digitalWrite(GREEN_U_B, HIGH);

  // setup zebra cross
  digitalWrite(RED_CROSS_L, HIGH);
  digitalWrite(RED_CROSS_U, HIGH);
  digitalWrite(RED_CROSS_CENTER, HIGH);
}
