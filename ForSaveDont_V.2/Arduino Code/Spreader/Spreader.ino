#include <SoftwareSerial.h> 
  SoftwareSerial APC(3, 4); // RX and TX 

const int sensor1 = A0;
const int sensor2 = A1;
const int sensor3 = A2;
int minVal = 320;
int count = 0;

void setup() 
{ 
  Serial.begin(9600);
  APC.begin(9600); // start serial to APC 
} 
void loop() 
{
  int sensorVal = analogRead(sensor1);
  if(sensorVal > minVal){
    count++;
  }
  sensorVal = analogRead(sensor2);
  if(sensorVal > minVal){
    count++;
  }
  sensorVal = analogRead(sensor3);
  if(sensorVal > minVal){
    count++;
  }

  APC.print(count); 
  Serial.print(count);
  count = 0;
  delay(2000); 
} 
