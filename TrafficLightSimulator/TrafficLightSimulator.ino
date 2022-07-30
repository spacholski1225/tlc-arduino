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
  turnOnRedDiode(GREEN_L, YELLOW_L, RED_L);
  turnOnRedDiode(GREEN_R, YELLOW_R, RED_R);

  turnOnZebraCross(GREEN_CROSS_L, RED_CROSS_L);

  delay(2000);
  turnOnGreenDiode(GREEN_U_B, YELLOW_U_B, RED_U_B); // waiting for cars to go up-bottom

  delay(2000);
  turnOnRedDiode(GREEN_U_B, YELLOW_U_B, RED_U_B);

  turnOnZebraCross(GREEN_CROSS_L, RED_CROSS_L);

  delay(2000);
  turnOnGreenDiode(GREEN_L, YELLOW_L, RED_L);
  turnOnGreenDiode(GREEN_R, YELLOW_R, RED_R);
}

void turnOnRedDiode(int greenDiode, int yellowDiode, int redDiode)
{
  if (digitalRead(greenDiode) == HIGH)
  {
    digitalWrite(greenDiode, LOW);
    delay(200);
  }

  if (digitalRead(yellowDiode) == LOW && digitalRead(redDiode) == LOW)
  {
    digitalWrite(yellowDiode, HIGH);
    delay(2000);
    digitalWrite(yellowDiode, LOW);
    digitalWrite(redDiode, HIGH);
  }
}

void turnOnGreenDiode(int greenDiode, int yellowDiode, int redDiode)
{
  if (digitalRead(redDiode) == HIGH)
  {
    digitalWrite(redDiode, LOW);
    digitalWrite(yellowDiode, HIGH);
    delay(500);
  }

  if (digitalRead(redDiode) == LOW && digitalRead(yellowDiode) == HIGH)
  {
    digitalWrite(yellowDiode, LOW);
    digitalWrite(greenDiode, HIGH);
  }
}

void turnOnZebraCross(int greenDiode, int redDiode)
{
  if (digitalRead(greenDiode) == HIGH)
  {
    digitalWrite(greenDiode, LOW);
    digitalWrite(redDiode, HIGH);

    return;
  }

  if (digitalRead(redDiode) == HIGH)
  {
    digitalWrite(redDiode, LOW);
    digitalWrite(greenDiode, HIGH);

    return;
  }
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
