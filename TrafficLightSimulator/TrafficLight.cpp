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

const int CHANGE_LIGHTS_TIME = 2000;
const int WAIT_FOR_YELLOW_TIME = 500;

bool _wasUsedSonar1 = false;
bool _wasUsedSonar2 = false;
bool _wasUsedSonar3 = false;
bool _wasUsedSonar4 = false;

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
    _CE1.digitalWrite(CROSS_EXPANDER1_DIODES[index], HIGH); //set up on off diode mode
  }

  int lightExpander2Length = sizeof(LIGHT_EXPANDER2_DIODES) / sizeof(*LIGHT_EXPANDER2_DIODES);
  for (int index = 0; index < lightExpander2Length; index++)
  {
    _LE2.pinMode(LIGHT_EXPANDER2_DIODES[index], OUTPUT);
    _LE2.digitalWrite(LIGHT_EXPANDER2_DIODES[index], HIGH); 
  }

  int crossExpander2Length = sizeof(CROSS_EXPANDER2_DIODES) / sizeof(*CROSS_EXPANDER2_DIODES);
  for (int index = 0; index < crossExpander2Length; index++)
  {
    _CE2.pinMode(CROSS_EXPANDER2_DIODES[index], OUTPUT);
    _CE2.digitalWrite(CROSS_EXPANDER2_DIODES[index], HIGH); 
  }

  int lightExpander1Length = sizeof(LIGHT_EXPANDER1_DIODES) / sizeof(*LIGHT_EXPANDER1_DIODES);
  for (int index = 0; index < lightExpander1Length; index++)
  {
    _LE1.pinMode(LIGHT_EXPANDER1_DIODES[index], OUTPUT);
    _LE1.digitalWrite(LIGHT_EXPANDER1_DIODES[index], HIGH); 
  }
}

int getDistance(int trigger, int echo)
{
  delay(1000);
  long time, distance;

  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  time = pulseIn(echo, HIGH);
  distance = time / 58;

  if(distance > 450 || distance <= 3)
  {
    distance = 0;
  }

  return distance;
}

void nightModeTurnOnRedDiodeAndOffGreen(PCF8574 expander, int greenDiode, int yellowDiode, int redDiode)
{
  expander.digitalWrite(greenDiode, HIGH);

  expander.digitalWrite(yellowDiode, LOW);
  delay(CHANGE_LIGHTS_TIME);
  expander.digitalWrite(yellowDiode, HIGH);
  expander.digitalWrite(redDiode, LOW);
}

void nightModeTurnOnGreenDiodeAndOffRed(PCF8574 expander, int greenDiode, int yellowDiode, int redDiode)
{
  expander.digitalWrite(yellowDiode, LOW);
  delay(WAIT_FOR_YELLOW_TIME);
  expander.digitalWrite(redDiode, HIGH);
  expander.digitalWrite(yellowDiode, HIGH);
  expander.digitalWrite(greenDiode, LOW);
}

void nightModeTurnOnRedLightAndOffGreenOnMainStreet()
{
  _LE2.digitalWrite(LE2_P4_G, HIGH);
  _LE1.digitalWrite(LE1_P4_G, HIGH);
  _LE2.digitalWrite(LE2_P5_Y, LOW);
  _LE1.digitalWrite(LE1_P5_Y, LOW);
  delay(WAIT_FOR_YELLOW_TIME);
  _LE2.digitalWrite(LE2_P5_Y, HIGH);
  _LE1.digitalWrite(LE1_P5_Y, HIGH);
  _LE2.digitalWrite(LE2_P6_R, LOW);
  _LE1.digitalWrite(LE1_P6_R, LOW);
}

void nightModeTurnOnGreenLightAndOffRedOnMainStreet()
{
  _LE2.digitalWrite(LE2_P5_Y, LOW);
  _LE1.digitalWrite(LE1_P5_Y, LOW);
  delay(WAIT_FOR_YELLOW_TIME);
  _LE2.digitalWrite(LE2_P6_R, HIGH);
  _LE1.digitalWrite(LE1_P6_R, HIGH);
  _LE2.digitalWrite(LE2_P5_Y, HIGH);
  _LE1.digitalWrite(LE1_P5_Y, HIGH);
  _LE2.digitalWrite(LE2_P4_G, LOW);
  _LE1.digitalWrite(LE1_P4_G, LOW);
}

bool nightModeTurnOnLightAfterAppropriateDistanceForU1(bool wasUsedSonar)
{
  int distance = getDistance(U1_TRIGGER, U1_ECHO);

  if (distance == 0)
  {
    if (wasUsedSonar)
    {
      return wasUsedSonar;
    }
    _CE2.digitalWrite(CE2_P2_R, LOW);
    _CE2.digitalWrite(CE2_P3_G, HIGH);

    _LE2.digitalWrite(LE2_P1_Y, LOW);

    _LE1.digitalWrite(LE1_P5_Y, LOW);
    _LE1.digitalWrite(LE1_P4_G, HIGH);
    _LE2.digitalWrite(LE2_P5_Y, LOW);
    _LE2.digitalWrite(LE2_P4_G, HIGH);
    delay(WAIT_FOR_YELLOW_TIME);
    _LE1.digitalWrite(LE1_P5_Y, HIGH);
    _LE1.digitalWrite(LE1_P6_R, LOW);
    _LE2.digitalWrite(LE2_P5_Y, HIGH);
    _LE2.digitalWrite(LE2_P6_R, LOW);

    _LE2.digitalWrite(LE2_P2_R, HIGH);
    _LE2.digitalWrite(LE2_P1_Y, HIGH);
    _LE2.digitalWrite(LE2_P0_G, LOW);

    wasUsedSonar = true;

    return wasUsedSonar;
  }

  if (wasUsedSonar && distance != 0)
  {
    wasUsedSonar = false;

    _CE2.digitalWrite(CE2_P2_R, HIGH);
    _CE2.digitalWrite(CE2_P3_G, LOW);
    
    _LE2.digitalWrite(LE2_P0_G, HIGH);
    _LE2.digitalWrite(LE2_P1_Y, LOW);

    _LE1.digitalWrite(LE1_P5_Y, LOW);
    _LE2.digitalWrite(LE2_P5_Y, LOW);

    delay(CHANGE_LIGHTS_TIME);

    _LE1.digitalWrite(LE1_P5_Y, HIGH);
    _LE2.digitalWrite(LE2_P5_Y, HIGH);
    _LE1.digitalWrite(LE1_P6_R, HIGH);
    _LE2.digitalWrite(LE2_P6_R, HIGH);
    _LE1.digitalWrite(LE1_P4_G, LOW);
    _LE2.digitalWrite(LE2_P4_G, LOW);

    _LE2.digitalWrite(LE2_P1_Y, HIGH);
    _LE2.digitalWrite(LE2_P2_R, LOW);
  }
  return wasUsedSonar;
}

bool nightModeTurnOnLightAfterAppropriateDistanceForU2(bool wasUsedSonar)
{
  int distance = getDistance(U2_TRIGGER, U2_ECHO);

  if (distance == 0)
  {
    if (wasUsedSonar)
    {
      return wasUsedSonar;
    }
    _CE2.digitalWrite(CE2_P2_R, LOW);
    _CE2.digitalWrite(CE2_P3_G, HIGH);

    _LE2.digitalWrite(LE2_P1_Y, LOW);

    _LE1.digitalWrite(LE1_P5_Y, LOW);
    _LE1.digitalWrite(LE1_P4_G, HIGH);
    _LE2.digitalWrite(LE2_P5_Y, LOW);
    _LE2.digitalWrite(LE2_P4_G, HIGH);
    delay(WAIT_FOR_YELLOW_TIME);
    _LE1.digitalWrite(LE1_P5_Y, HIGH);
    _LE1.digitalWrite(LE1_P6_R, LOW);
    _LE2.digitalWrite(LE2_P5_Y, HIGH);
    _LE2.digitalWrite(LE2_P6_R, LOW);

    _LE2.digitalWrite(LE2_P2_R, HIGH);
    _LE2.digitalWrite(LE2_P1_Y, HIGH);
    _LE2.digitalWrite(LE2_P0_G, LOW);

    wasUsedSonar = true;

    return wasUsedSonar;
  }

  if (wasUsedSonar && distance != 0)
  {
    wasUsedSonar = false;

    _CE2.digitalWrite(CE2_P2_R, HIGH);
    _CE2.digitalWrite(CE2_P3_G, LOW);
    
    _LE2.digitalWrite(LE2_P0_G, HIGH);
    _LE2.digitalWrite(LE2_P1_Y, LOW);

    _LE1.digitalWrite(LE1_P5_Y, LOW);
    _LE2.digitalWrite(LE2_P5_Y, LOW);

    delay(CHANGE_LIGHTS_TIME);

    _LE1.digitalWrite(LE1_P5_Y, HIGH);
    _LE2.digitalWrite(LE2_P5_Y, HIGH);
    _LE1.digitalWrite(LE1_P6_R, HIGH);
    _LE2.digitalWrite(LE2_P6_R, HIGH);
    _LE1.digitalWrite(LE1_P4_G, LOW);
    _LE2.digitalWrite(LE2_P4_G, LOW);

    _LE2.digitalWrite(LE2_P1_Y, HIGH);
    _LE2.digitalWrite(LE2_P2_R, LOW);
  }
  return wasUsedSonar;
}

bool nightModeTurnOnLightAfterAppropriateDistanceForU3(bool wasUsedSonar)
{
  int distance = getDistance(U3_TRIGGER, U3_ECHO);

  if (distance == 0)
  {
    if (wasUsedSonar)
    {
      return wasUsedSonar;
    }

    _CE1.digitalWrite(CE2_P2_R, LOW);
    _CE1.digitalWrite(CE2_P3_G, HIGH);

    _LE1.digitalWrite(LE1_P1_Y, LOW);

    _LE1.digitalWrite(LE1_P5_Y, LOW);
    _LE1.digitalWrite(LE1_P4_G, HIGH);
    _LE2.digitalWrite(LE2_P5_Y, LOW);
    _LE2.digitalWrite(LE2_P4_G, HIGH);
    delay(WAIT_FOR_YELLOW_TIME);
    _LE1.digitalWrite(LE1_P5_Y, HIGH);
    _LE1.digitalWrite(LE1_P6_R, LOW);
    _LE2.digitalWrite(LE2_P5_Y, HIGH);
    _LE2.digitalWrite(LE2_P6_R, LOW);

    _LE1.digitalWrite(LE1_P2_R, HIGH);
    _LE1.digitalWrite(LE1_P1_Y, HIGH);
    _LE1.digitalWrite(LE1_P0_G, LOW);

    wasUsedSonar = true;

    return wasUsedSonar;
  }

  if (wasUsedSonar && distance != 0)
  {
    wasUsedSonar = false;

    _CE1.digitalWrite(CE2_P2_R, HIGH);
    _CE1.digitalWrite(CE2_P3_G, LOW);
    
    _LE1.digitalWrite(LE1_P0_G, HIGH);
    _LE1.digitalWrite(LE1_P1_Y, LOW);

    _LE1.digitalWrite(LE1_P5_Y, LOW);
    _LE2.digitalWrite(LE2_P5_Y, LOW);

    delay(CHANGE_LIGHTS_TIME);

    _LE1.digitalWrite(LE1_P5_Y, HIGH);
    _LE2.digitalWrite(LE2_P5_Y, HIGH);
    _LE1.digitalWrite(LE1_P6_R, HIGH);
    _LE2.digitalWrite(LE2_P6_R, HIGH);
    _LE1.digitalWrite(LE1_P4_G, LOW);
    _LE2.digitalWrite(LE2_P4_G, LOW);

    _LE1.digitalWrite(LE1_P1_Y, HIGH);
    _LE1.digitalWrite(LE1_P2_R, LOW);
  }
  return wasUsedSonar;
}

bool nightModeTurnOnLightAfterAppropriateDistanceForU4(bool wasUsedSonar)
{
  int distance = getDistance(U4_TRIGGER, U4_ECHO);

  if (distance == 0)
  {
    if (wasUsedSonar)
    {
      return wasUsedSonar;
    }
    _CE1.digitalWrite(CE2_P2_R, LOW);
    _CE1.digitalWrite(CE2_P3_G, HIGH);

    _LE1.digitalWrite(LE1_P1_Y, LOW);

    _LE1.digitalWrite(LE1_P5_Y, LOW);
    _LE1.digitalWrite(LE1_P4_G, HIGH);
    _LE2.digitalWrite(LE2_P5_Y, LOW);
    _LE2.digitalWrite(LE2_P4_G, HIGH);
    delay(WAIT_FOR_YELLOW_TIME);
    _LE1.digitalWrite(LE1_P5_Y, HIGH);
    _LE1.digitalWrite(LE1_P6_R, LOW);
    _LE2.digitalWrite(LE2_P5_Y, HIGH);
    _LE2.digitalWrite(LE2_P6_R, LOW);

    _LE1.digitalWrite(LE1_P2_R, HIGH);
    _LE1.digitalWrite(LE1_P1_Y, HIGH);
    _LE1.digitalWrite(LE1_P0_G, LOW);

    wasUsedSonar = true;

    return wasUsedSonar;
  }

  if (wasUsedSonar && distance != 0)
  {
    wasUsedSonar = false;

    _CE1.digitalWrite(CE2_P2_R, HIGH);
    _CE1.digitalWrite(CE2_P3_G, LOW);
    
    _LE1.digitalWrite(LE1_P0_G, HIGH);
    _LE1.digitalWrite(LE1_P1_Y, LOW);

    _LE1.digitalWrite(LE1_P5_Y, LOW);
    _LE2.digitalWrite(LE2_P5_Y, LOW);

    delay(CHANGE_LIGHTS_TIME);

    _LE1.digitalWrite(LE1_P5_Y, HIGH);
    _LE2.digitalWrite(LE2_P5_Y, HIGH);
    _LE1.digitalWrite(LE1_P6_R, HIGH);
    _LE2.digitalWrite(LE2_P6_R, HIGH);
    _LE1.digitalWrite(LE1_P4_G, LOW);
    _LE2.digitalWrite(LE2_P4_G, LOW);

    _LE1.digitalWrite(LE1_P1_Y, HIGH);
    _LE1.digitalWrite(LE1_P2_R, LOW);
  }
  return wasUsedSonar;
}

void dayModeTurnOnGreenZebraCrossAndOffRedOnMainRoad()
{
  _CE1.digitalWrite(CE1_P0_R, HIGH);
  _CE1.digitalWrite(CE1_P1_G, LOW);
  delay(100);
  _CE2.digitalWrite(CE2_P5_R, HIGH);
  _CE2.digitalWrite(CE2_P6_G, LOW);
  delay(100);
  _CE2.digitalWrite(CE2_P0_R, HIGH);
  _CE2.digitalWrite(CE2_P1_G, LOW);
}

void dayModeTurnOnRedZebraCrossAndOffGreenOnSuboRoad()
{
  _CE1.digitalWrite(CE1_P3_G, HIGH);
  _CE1.digitalWrite(CE1_P2_R, LOW);
  
  _CE2.digitalWrite(CE2_P3_G, HIGH);
  _CE2.digitalWrite(CE2_P2_R, LOW);
}

void dayModeTurnOnRedLightAndOffGreenOnMainRoad()
{
  _LE1.digitalWrite(LE1_P4_G, HIGH);
  _LE1.digitalWrite(LE1_P5_Y, LOW);
  delay(CHANGE_LIGHTS_TIME);
  _LE1.digitalWrite(LE1_P5_Y, HIGH);
  _LE1.digitalWrite(LE1_P6_R, LOW);

  _LE2.digitalWrite(LE2_P4_G, HIGH);
  _LE2.digitalWrite(LE2_P5_Y, LOW);
  delay(CHANGE_LIGHTS_TIME);
  _LE2.digitalWrite(LE2_P5_Y, HIGH);
  _LE2.digitalWrite(LE2_P6_R, LOW);
}

void dayModeTurnOnGreenAndOffRedOnSuboRoad()
{
  _LE1.digitalWrite(LE1_P1_Y, LOW);
  delay(WAIT_FOR_YELLOW_TIME);

  _LE1.digitalWrite(LE1_P2_R, HIGH);
  _LE1.digitalWrite(LE1_P1_Y, HIGH);
  _LE1.digitalWrite(LE1_P0_G, LOW);
  _LE2.digitalWrite(LE2_P1_Y, LOW);
  delay(WAIT_FOR_YELLOW_TIME);

  _LE2.digitalWrite(LE2_P2_R, HIGH);
  _LE2.digitalWrite(LE2_P1_Y, HIGH);
  _LE2.digitalWrite(LE2_P0_G, LOW);
}

void dayModeTurnOnRedZebraCrossAndOffGreenOnMainRoad()
{
  _CE1.digitalWrite(CE1_P2_R, HIGH);
  _CE1.digitalWrite(CE1_P3_G, LOW);

  _CE2.digitalWrite(CE2_P2_R, HIGH);
  _CE2.digitalWrite(CE2_P3_G, LOW);
}

void dayModeTurnOnGreenAndOffReadZebraCrossOnSuboRoad()
{
  _CE1.digitalWrite(CE1_P1_G, HIGH);
  _CE1.digitalWrite(CE1_P0_R, LOW);

  _CE2.digitalWrite(CE2_P6_G, HIGH);
  _CE2.digitalWrite(CE2_P5_R, LOW);

  _CE2.digitalWrite(CE2_P1_G, HIGH);
  _CE2.digitalWrite(CE2_P0_R, LOW);
}

void dayModeTurnOnGreenAndOffRedLightsOnMainRoad()
{
  _LE1.digitalWrite(LE1_P5_Y, LOW);
  delay(WAIT_FOR_YELLOW_TIME);
  _LE1.digitalWrite(LE1_P6_R, HIGH);
  _LE1.digitalWrite(LE1_P5_Y, HIGH);
  _LE1.digitalWrite(LE1_P4_G, LOW);
  
  _LE2.digitalWrite(LE2_P5_Y, LOW);
  delay(WAIT_FOR_YELLOW_TIME);
  _LE2.digitalWrite(LE2_P6_R, HIGH);
  _LE2.digitalWrite(LE2_P5_Y, HIGH);
  _LE2.digitalWrite(LE2_P4_G, LOW);
}

void dayModeTurnOnRedDiodeAndOffGreenOnSuboRoad()
{
  _LE1.digitalWrite(LE1_P0_G, HIGH);
  _LE1.digitalWrite(LE1_P1_Y, LOW);
  delay(WAIT_FOR_YELLOW_TIME);
  _LE1.digitalWrite(LE1_P1_Y, HIGH);
  _LE1.digitalWrite(LE1_P2_R, LOW);
  
  _LE2.digitalWrite(LE2_P0_G, HIGH);
  _LE2.digitalWrite(LE2_P1_Y, LOW);
  delay(WAIT_FOR_YELLOW_TIME);
  _LE2.digitalWrite(LE2_P1_Y, HIGH);
  _LE2.digitalWrite(LE2_P2_R, LOW);
}

void TrafficLight::setNightModeStartParameters()
{
  // setup traffic lights
  _LE2.digitalWrite(LE2_P4_G, LOW);
  _LE1.digitalWrite(LE1_P4_G, LOW);
  _LE2.digitalWrite(LE2_P2_R, LOW);
  _LE1.digitalWrite(LE1_P2_R, LOW);

  // setup zebra cross
  _CE1.digitalWrite(CE1_P0_R, LOW);
  _CE2.digitalWrite(CE2_P0_R, LOW);
  _CE1.digitalWrite(CE1_P3_G, LOW);
  _CE2.digitalWrite(CE2_P3_G, LOW);
  _CE2.digitalWrite(CE2_P5_R, LOW);
}

void TrafficLight::setDayModeStartParameters()
{
  // setting traffic light
  _LE2.digitalWrite(LE2_P4_G, LOW);
  _LE1.digitalWrite(LE1_P4_G, LOW);
  _LE2.digitalWrite(LE2_P2_R, LOW);
  _LE1.digitalWrite(LE1_P2_R, LOW);

  // setting zebra cross for left up
  _CE1.digitalWrite(CE1_P0_R, LOW);
  _CE2.digitalWrite(CE2_P0_R, LOW);

  // setting zebra cross for down
  _CE1.digitalWrite(CE1_P3_G, LOW);
  _CE2.digitalWrite(CE2_P3_G, LOW);

  // setting center zebra cross
  _CE2.digitalWrite(CE2_P5_R, LOW);
}

void TrafficLight::dayMode()
{
  delay(CHANGE_LIGHTS_TIME);
  dayModeTurnOnRedLightAndOffGreenOnMainRoad();
  dayModeTurnOnGreenZebraCrossAndOffRedOnMainRoad();

  delay(CHANGE_LIGHTS_TIME);
  dayModeTurnOnRedZebraCrossAndOffGreenOnSuboRoad();

  delay(CHANGE_LIGHTS_TIME);
  dayModeTurnOnGreenAndOffRedOnSuboRoad();

  delay(CHANGE_LIGHTS_TIME);
  dayModeTurnOnRedDiodeAndOffGreenOnSuboRoad();
  dayModeTurnOnRedZebraCrossAndOffGreenOnMainRoad();
  dayModeTurnOnGreenAndOffReadZebraCrossOnSuboRoad();

  delay(CHANGE_LIGHTS_TIME);
  dayModeTurnOnGreenAndOffRedLightsOnMainRoad();
}

void TrafficLight::nightMode()
{
  delay(1);
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
  delay(10);

  if (_isOnLeftCross)
  {
    nightModeTurnOnRedLightAndOffGreenOnMainStreet();
    _CE1.digitalWrite(CE1_P0_R, HIGH);
    _CE1.digitalWrite(CE1_P1_G, LOW);

    delay(CHANGE_LIGHTS_TIME);

    _CE1.digitalWrite(CE1_P0_R, LOW);
    _CE1.digitalWrite(CE1_P1_G, HIGH);
    nightModeTurnOnGreenLightAndOffRedOnMainStreet();

    _isOnLeftCross = false;
  }
  if (_isOnBottomCross)
  {
    nightModeTurnOnRedLightAndOffGreenOnMainStreet();
    _CE2.digitalWrite(CE2_P5_R, HIGH);
    _CE2.digitalWrite(CE2_P6_G, LOW);

    delay(CHANGE_LIGHTS_TIME);

    _CE2.digitalWrite(CE2_P5_R, LOW);
    _CE2.digitalWrite(CE2_P6_G, HIGH);
    nightModeTurnOnGreenLightAndOffRedOnMainStreet();

    _isOnBottomCross = false;
  }
  if (_isOnRightCross)
  {
    nightModeTurnOnRedLightAndOffGreenOnMainStreet();
    _CE2.digitalWrite(CE2_P0_R, HIGH);
    _CE2.digitalWrite(CE2_P1_G, LOW);

    delay(CHANGE_LIGHTS_TIME);

    _CE2.digitalWrite(CE2_P0_R, LOW);
    _CE2.digitalWrite(CE2_P1_G, HIGH);
    nightModeTurnOnGreenLightAndOffRedOnMainStreet();

    _isOnRightCross = false;
  }

  _wasUsedSonar1 = nightModeTurnOnLightAfterAppropriateDistanceForU1(_wasUsedSonar1);
  _wasUsedSonar2 = nightModeTurnOnLightAfterAppropriateDistanceForU2(_wasUsedSonar2);
  _wasUsedSonar3 = nightModeTurnOnLightAfterAppropriateDistanceForU3(_wasUsedSonar3);
  _wasUsedSonar4 = nightModeTurnOnLightAfterAppropriateDistanceForU4(_wasUsedSonar4);
}
