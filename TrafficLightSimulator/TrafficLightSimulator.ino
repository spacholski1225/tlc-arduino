#include <Wire.h>
#include "TrafficLight.h"
#include "PCF8574.h"

#define EXPANDER1_RED_CROSS_L 0
#define EXPANDER1_GREEN_CROSS_L 1
#define EXPANDER1_RED_CROSS_B 2
#define EXPANDER1_GREEN_CROSS_B 3

#define EXPANDER2_RED_L 6
#define EXPANDER2_YELLOW_L 5
#define EXPANDER2_GREEN_L 4
#define EXPANDER2_RED_B 3
#define EXPANDER2_YELLOW_B 2
#define EXPANDER2_GREEN_B 1

#define BUTTON_CROSS_L 10
#define BUTTON_CROSS_B 11

PCF8574 crossExpander;
PCF8574 lightExpander;

const int expander1Diodes[] = {EXPANDER1_RED_CROSS_L, EXPANDER1_GREEN_CROSS_L, EXPANDER1_RED_CROSS_B, EXPANDER1_GREEN_CROSS_B};
const int expander2Diodes[] = {EXPANDER2_RED_L, EXPANDER2_YELLOW_L, EXPANDER2_GREEN_L, EXPANDER2_RED_B, EXPANDER2_YELLOW_B, EXPANDER2_GREEN_B};

TrafficLight traffic(crossExpander, lightExpander);

bool isDayMode = false;

bool isOnLeftCross = false;
bool isOnBottomCross = true;

void setup()
{
  crossExpander.begin(0x20);
  lightExpander.begin(0x21);

  pinMode(BUTTON_CROSS_L, INPUT_PULLUP);
  pinMode(BUTTON_CROSS_B, INPUT_PULLUP);

  Serial.begin(9600);

  setUpExpanderDiodes();

  if (isDayMode) // to change cuz it run only once time, so always will execute setdDayModeStartParameters()
  {
    // setDayModeStartParameters();
  }
  else
  {
    setNightModeStartParameters();
  }
}

void loop()
{
  turnOnNightMode();
}

void turnOnCrossLightAfterButtonClick(int crossGreenDiode, int crossRedDiode, int lightGreenDiode, int lightYellowDiode, int lightRedDiode)
{
  crossExpander.digitalWrite(crossRedDiode, LOW);
  crossExpander.digitalWrite(crossGreenDiode, HIGH);

  turnOnRedDiodeAndOffGreen(lightGreenDiode, lightYellowDiode, lightRedDiode);

  delay(5000);

  crossExpander.digitalWrite(crossRedDiode, HIGH);
  crossExpander.digitalWrite(crossGreenDiode, LOW);

  turnOnGreenDiodeAndOffRed(lightGreenDiode, lightYellowDiode, lightRedDiode);
}

void setUpExpanderDiodes()
{
  int expanderDiodesLength = sizeof(expander1Diodes) / sizeof(*expander1Diodes);
  for (int index = 0; index < expanderDiodesLength; index++)
  {
    crossExpander.pinMode(expander1Diodes[index], OUTPUT);
  }

  int expander2DiodesLength = sizeof(expander2Diodes) / sizeof(*expander2Diodes);
  for (int index = 0; index < expander2DiodesLength; index++)
  {
    lightExpander.pinMode(expander2Diodes[index], OUTPUT);
  }
}

void turnOnNightMode()
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
    turnOnCrossLightAfterButtonClick(EXPANDER1_GREEN_CROSS_L, EXPANDER1_RED_CROSS_L, EXPANDER2_GREEN_L, EXPANDER2_YELLOW_L, EXPANDER2_RED_L);
    isOnLeftCross = false;
  }
}

void turnOnRedDiodeAndOffGreen(int greenDiode, int yellowDiode, int redDiode) // to jest zjebana metoda trzeba ja przerobic!
{
  if (lightExpander.digitalRead(greenDiode) == HIGH)
  {
    lightExpander.digitalWrite(greenDiode, LOW);
    delay(200);
  }

  if (lightExpander.digitalRead(yellowDiode) == LOW && lightExpander.digitalRead(redDiode) == LOW)
  {
    lightExpander.digitalWrite(greenDiode, LOW); // only to check

    lightExpander.digitalWrite(yellowDiode, HIGH);
    delay(2000);
    lightExpander.digitalWrite(yellowDiode, LOW);
    lightExpander.digitalWrite(redDiode, HIGH);
  }
}

void turnOnGreenDiodeAndOffRed(int greenDiode, int yellowDiode, int redDiode)
{

  lightExpander.digitalWrite(redDiode, LOW);
  lightExpander.digitalWrite(yellowDiode, HIGH);
  delay(500);

  lightExpander.digitalWrite(yellowDiode, LOW);
  lightExpander.digitalWrite(greenDiode, HIGH);
}

// void turnOnDayMode()
// {
//   delay(2000); // waiting for cars to go left-right
//   traffic.turnOnRedDiodeAndOffGreen(GREEN_L, YELLOW_L, RED_L);
//   traffic.turnOnGreenZebraCrossControlledByTime(GREEN_CROSS_CENTER, RED_CROSS_CENTER);
//   traffic.turnOnRedDiodeAndOffGreen(GREEN_R, YELLOW_R, RED_R);

//   // turn on zebra lights
//   traffic.turnOnGreenZebraCrossControlledByTime(GREEN_CROSS_L, RED_CROSS_L);
//   traffic.turnOnRedZebraCrossControlledByTime(GREEN_CROSS_U, RED_CROSS_U);

//   delay(2000);
//   traffic.turnOnGreenDiodeAndOffRed(GREEN_U_B, YELLOW_U_B, RED_U_B); // waiting for cars to go up-bottom

//   delay(2000);
//   traffic.turnOnRedDiodeAndOffGreen(GREEN_U_B, YELLOW_U_B, RED_U_B);

//   traffic.turnOnGreenZebraCrossControlledByTime(GREEN_CROSS_U, RED_CROSS_U);
//   traffic.turnOnRedZebraCrossControlledByTime(GREEN_CROSS_L, RED_CROSS_L);

//   delay(2000);
//   traffic.turnOnRedZebraCrossControlledByTime(GREEN_CROSS_CENTER, RED_CROSS_CENTER);
//   traffic.turnOnGreenDiodeAndOffRed(GREEN_L, YELLOW_L, RED_L);
//   traffic.turnOnGreenDiodeAndOffRed(GREEN_R, YELLOW_R, RED_R);
// }

// void setDayModeStartParameters()
// {
//   // setting traffic light
//   digitalWrite(GREEN_L, HIGH);
//   digitalWrite(GREEN_R, HIGH);
//   digitalWrite(RED_U_B, HIGH);

//   // setting zebra cross for left up
//   digitalWrite(RED_CROSS_L, HIGH);

//   // setting zebra cross for down
//   digitalWrite(GREEN_CROSS_U, HIGH);

//   // setting center zebra cross
//   digitalWrite(RED_CROSS_CENTER, HIGH);
// }

void setNightModeStartParameters()
{
  // setup traffic lights
  lightExpander.digitalWrite(EXPANDER2_GREEN_L, HIGH);
  lightExpander.digitalWrite(EXPANDER2_RED_B, HIGH);

  // setup zebra cross
  crossExpander.digitalWrite(EXPANDER1_RED_CROSS_L, HIGH);
  crossExpander.digitalWrite(EXPANDER1_GREEN_CROSS_B, HIGH);
}
