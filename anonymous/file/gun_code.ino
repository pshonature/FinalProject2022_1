#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"
#include <LiquidCrystal.h>
LiquidCrystal LCD(13,12,5,4,3,2);
#define laser 7          //레이저 모듈 핀설정
#define button 6        //버튼 핀 설정
#define vibe 8        //진동센서 핀 설정
int vibelimit = 1022; //진동센서 민감도
int buttonState= 0;    //현재 버튼값 초기화
int lastButtonState = 0; //과거 버튼값 초기화
int bullet = 8;         //총알 갯수 초기화

SoftwareSerial df_player_Module(10, 11); // DFPlayer 시리얼 통신용 객체 생성 >> 디지털 핀 두개 선택
DFRobotDFPlayerMini df_player; // DFPlayer 객체 생성
bool processing = false;




void setup() {
  
  pinMode(button,INPUT);  //버튼 입력모드
  pinMode(laser, OUTPUT); //레이저 출력모드
  pinMode(vibe,INPUT);    //진동센서 입력모드
  Serial.begin(9600);
  df_player_Module.begin(9600); //df플레이어 모듈
  LCD.begin(16,2);        //lcd 16,2설정
 //
 if (!df_player.begin(df_player_Module), false)
  {
//    Serial.println(F("Unable to begin:"));
//    Serial.println(F("1.Please recheck the connection!"));
//    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  delay(1);
  df_player.volume(20); // 볼륨 지정
//  df_player.setTimeOut(500); //시리얼 통신용 타임아웃 시간 지정


}



void loop() {
          
  while (bullet != 0) { //총알이 0발 될때까지 반복
          LCD.setCursor(0,0);                             //lcd커서를 0,0으로 두고
          LCD.print("bullet :"); LCD.print(bullet);       //lcd
    
     buttonState = digitalRead(button);    //푸쉬버튼값 읽기
    if (buttonState != lastButtonState) { //푸쉬버튼의 상태가 변했다면 ex) HIGH > LOW or LOW > HIGH

      if (buttonState == HIGH) {   //바뀐값이 HIGH일때
        shoot(3);                   //발사음(0003.mp3)출력하고
        digitalWrite(laser, HIGH);  //레이저를
        delay(100);                 //0.1초동안 HIGH상태로 바꾸고
        digitalWrite(laser, LOW);   //다시 LOW로 바꾼다.

        bullet--;                   //총알을 -1한다.
               
    Serial.println(bullet);         //시리얼모니터로 총알읽기                  
      }
      delay(50);                   // 다음 발사 시간까지의 대기시간
    }
    lastButtonState = buttonState; //과거값 갱신
  }
      LCD.clear();                  //lcd초기화
      LCD.setCursor(0,0);           //커서 0,0으로 이동
      LCD.print("bullet :");LCD.print(bullet);    //총알개수 출력
      buttonState = digitalRead(button);    //푸쉬버튼값 읽기
      
    if (buttonState != lastButtonState) { //푸쉬버튼의 상태가 변했다면 ex) HIGH > LOW or LOW > HIGH
      if (buttonState == HIGH)    //바뀐값이 HIGH일때        
               shoot(1);          //빈총알소리(0001.mp3)출력
      
    }
      lastButtonState = buttonState; //과거값 갱신
      
      if (digitalRead(vibe) == HIGH) { //진동감지센서로 진동감지 후

      shoot(2);     //재장전소리(0002.mp3)출력
      bullet = 8;   //재장전

  }   
      
                    
  }

  void shoot (int num) {
    df_player.play(num); //mp3출력 함수
  }
