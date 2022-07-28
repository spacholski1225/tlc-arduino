#define RED_L 2
#define YELLOW_L 3
#define GREEN_L 4

#define RED_U_B A5
#define YELLOW_U_B A4
#define GREEN_U_B A3

#define RED_R 8
#define YELLOW_R 9
#define GREEN_R 10

int diodes[] = {RED_L, YELLOW_L, GREEN_L, RED_U_B, YELLOW_U_B, GREEN_U_B, RED_R, YELLOW_R, GREEN_R};

void setup()
{

  Serial.begin(9600);

  Serial.println("setup");
  setPinModes();
  setStartParameters();
}

void loop()
{
  delay(2000);//waiting for cars to go left-right
  turnOnRedDiode(GREEN_L, YELLOW_L, RED_L);
  turnOnRedDiode(GREEN_R, YELLOW_R, RED_R);

  delay(2000);
  turnOnGreenDiode(GREEN_U_B, YELLOW_U_B, RED_U_B); // waiting for cars to go up-bottom

  delay(2000);
  turnOnRedDiode(GREEN_U_B, YELLOW_U_B, RED_U_B);

  delay(2000);
  turnOnGreenDiode(GREEN_L, YELLOW_L, RED_L);
  turnOnGreenDiode(GREEN_R, YELLOW_R, RED_R);


}

void setPinModes()
{
  int diodesLength = sizeof(diodes) / sizeof(*diodes);

  Serial.println(diodesLength);
  for (int i = 0; i < diodesLength; i++)
  {
    Serial.println(i);
    pinMode(diodes[i], OUTPUT);
    delay(1);
  }
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

void setStartParameters()
{
  digitalWrite(GREEN_L, HIGH);
  digitalWrite(GREEN_R, HIGH);
  digitalWrite(RED_U_B, HIGH);
}
