#include "TrafficLight.h"

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

const int diodes[] = {RED_L, YELLOW_L, GREEN_L, RED_U_B, YELLOW_U_B, GREEN_U_B, RED_R, YELLOW_R, GREEN_R, RED_CROSS_L, GREEN_CROSS_L};

TrafficLight traffic;

void setup()
{
  Serial.begin(9600);

  int diodesLength = sizeof(diodes) / sizeof(*diodes);
  for (int index = 0; index < diodesLength; index++)
  {
    traffic.setDiodeToIndex(diodes[index], index);
  }

  traffic.setPinModes();
  setStartParameters();
}

void loop()
{
  delay(2000); // waiting for cars to go left-right
  traffic.turnOnRedDiodeAndOffGreen(GREEN_L, YELLOW_L, RED_L);
  traffic.turnOnRedDiodeAndOffGreen(GREEN_R, YELLOW_R, RED_R);

  traffic.turnOnZebraCrossControlledByTime(GREEN_CROSS_L, RED_CROSS_L);

  delay(2000);
  traffic.turnOnGreenDiodeAndOffRed(GREEN_U_B, YELLOW_U_B, RED_U_B); // waiting for cars to go up-bottom

  delay(2000);
  traffic.turnOnRedDiodeAndOffGreen(GREEN_U_B, YELLOW_U_B, RED_U_B);

  traffic.turnOnZebraCrossControlledByTime(GREEN_CROSS_L, RED_CROSS_L);

  delay(2000);
  traffic.turnOnGreenDiodeAndOffRed(GREEN_L, YELLOW_L, RED_L);
  traffic.turnOnGreenDiodeAndOffRed(GREEN_R, YELLOW_R, RED_R);
}

void setStartParameters()
{
  // setting traffic light
  digitalWrite(GREEN_L, HIGH);
  digitalWrite(GREEN_R, HIGH);
  digitalWrite(RED_U_B, HIGH);

  // setting zebra cross
  digitalWrite(RED_CROSS_L, HIGH);
}
