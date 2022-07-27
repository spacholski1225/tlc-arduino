#define RED_L 2
#define YELLOW_L 3
#define GREEN_L 4

#define RED_U 5
#define YELLOW_U 6
#define GREEN_U 7

#define RED_B A5
#define YELLOW_B A4
#define GREEN_B A3

#define RED_R 8
#define YELLOW_R 9
#define GREEN_R 10

int diodes[] = { RED_L, YELLOW_L, GREEN_L, RED_U, YELLOW_U, GREEN_U, RED_B, YELLOW_B, GREEN_B, RED_R, YELLOW_R, GREEN_R };

int diodesLength;

void setup() 
{
  diodesLength = sizeof(diodes)/sizeof(*diodes);

  Serial.begin(9600);
  
  Serial.println("setup");
  setPinModes();
  turnOnDiodes();
}

void loop() 
{
}

void setPinModes()
{
  Serial.println(diodesLength);
  for(int i = 0; i < diodesLength; i++)
  {
    Serial.println(i);
    pinMode(diodes[i], OUTPUT);
    delay(1);
  }
}

void turnOnDiodes()
{  
  for(int i = 0; i < diodesLength; i++)
  {
    digitalWrite(diodes[i], HIGH);
    delay(100);
  }
}
