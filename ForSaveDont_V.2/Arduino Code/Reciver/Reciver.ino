#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,20,4);

#include <TimeLib.h>
#include <Time.h>

#include <Servo.h>
Servo myservo;

#include <DFPlayer_Mini_Mp3.h>

#include <SoftwareSerial.h>
  SoftwareSerial APC(3, 4); // RX and TX

int warning = 0; // 0 : 안전신호, 1 : 주의신호, 2 : 경고신호, 3 : 위험신호
int safeTime = 0; // 3번이상 해당 신호를 받아야 안전
int cautionTime = 0; // 2번이상 해당 신호를 받아야 주의
int warningTime = 0; // 2번이상 해당 신호를 받아야 경고
int dangerTime = 0; // 2번이상 해당 신호를 받아야 위험
int alarmZero = 0;

int audioTerm= 0 ; //10마다 오디오 재생
int state = 0; //0 : 안전상태, 1 : 주의상태, 2 : 경고상태, 3 : 위험상태
int pos = 0;

void setup() {
  pinMode(13, OUTPUT); //LED : 13번핀
  
  setTime(1,21,0,12,7,18);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  
  myservo.attach(9);  //서보 : 9번핀
  
  mp3_set_serial (Serial);      // DFPlayer-mini mp3 module 시리얼 세팅
  delay(1);                     // 볼륨값 적용을 위한 delay
  mp3_set_volume (30); 

  APC.begin(9600);
  Serial.begin(9600);
}

void loop() {
  while(APC.available()) {
    char ch = (char)APC.read();
    warning = APC.parseInt();
    if(warning != state){ //단계가 달라졌을 경우
      switch(warning){
        case 0:
          safeTime++;
          cautionTime = 0;
          warningTime = 0;
          dangerTime = 0;
          break;
        case 1:
          safeTime = 0;
          cautionTime++;
          warningTime = 0;
          dangerTime = 0;
          break;
        case 2:
          safeTime=0;
          cautionTime = 0;
          warningTime++;
          dangerTime = 0;
          break;
        case 3:
          safeTime=0;
          cautionTime = 0;
          warningTime = 0;
          dangerTime++;
          break;
      }
    }
    else{ //똑같은 신호가 올 경우 초기화
      safeTime = 0;
      cautionTime = 0;
      warningTime = 0;
      dangerTime = 0;
    }
  }
  if(alarmZero > 0){
    initDisplay();
    digitalClockDisplay();
    lcd.setCursor(0,3);
    lcd.print("Alarm Release");
    alarmZero--;
  }
  else{
    switch(state){
      case 0:
        audioTerm = 0;
        
        initDisplay();
        digitalClockDisplay();
        lcd.setCursor(0,3);
        lcd.print("Have a nice day! ^^");
      break;
      case 1:
        audioTerm = 0;
        
        initDisplay();
        digitalClockDisplay();
        lcd.setCursor(0,3);
        lcd.print("Flooding Level 1");
      break;
      case 2:
        audioTerm = 0;
        
        initDisplay();
        digitalClockDisplay();
        lcd.setCursor(0,3);
        lcd.print("Flooding Level 2");
      break;
      case 3:
        turnOn();
        
        initDisplay();
        digitalClockDisplay();
        lcd.setCursor(0,3);
        lcd.print("Flooding Level 3");
        
        delay(100);
        if(audioTerm > 18){
          mp3_play(3);
          audioTerm = 0;
        }else{
          audioTerm++;
        }
      break;
    }
  }
    
  if(cautionTime == 3){ //위험!!
    state = 1;
    digitalWrite(13,HIGH);
    
    initDisplay();
    digitalClockDisplay();
    lcd.setCursor(0,3);
    lcd.print("Flooding Level 1");
    
    delay(100);
    mp3_play(1);
  }
  if(warningTime == 3){ //위험!! 
    state = 2;
    digitalWrite(13,HIGH);
    
    initDisplay();
    digitalClockDisplay();
    lcd.setCursor(0,3);
    lcd.print("Flooding Level 2");
    
    delay(100);
    mp3_play(2);
  }
  if(dangerTime == 3){ //위험!!
    state = 3;
    digitalWrite(13,HIGH);
    
    initDisplay();
    digitalClockDisplay();
    lcd.setCursor(0,3);
    lcd.print("Flooding Level 3");
    
    delay(100);
    mp3_play(3);
  }
  if(safeTime == 3){
    state = 0;
    digitalWrite(13,LOW);
    alarmZero = 2;
    
    initDisplay();
    digitalClockDisplay();
    lcd.setCursor(0,3);
    lcd.print("Alarm Release");
    
    delay(100);
    mp3_play(4);
  }
  delay(3000);
}

void initDisplay(){
    lcd.setCursor(0,0);
    lcd.print("                    ");
    lcd.setCursor(0,3);
    lcd.print("                    ");
}

 void digitalClockDisplay(){
    lcd.setCursor(0,0);
    lcd.print(year());
    lcd.print("/");
    lcd.print(month());
    lcd.print("/");
    lcd.print(day());
    lcd.print(" ");
    lcd.print(hour());
    printDigits(minute());
    printDigits(second());
 }
 
 void turnOn(){
    for(pos = 0; pos < 180; pos += 1)
    {
        myservo.write(pos);
        delay(30);
    } 
 }

 void printDigits(int digits){
    lcd.print(":");
    if (digits < 10)
       lcd.print('0');
    lcd.print(digits);
 }
