#include <Servo.h>
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"
#define SERVO_PIN 8
#define RX 2
#define TX 3

#define TEST_LED 7

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* VAR */

Servo servo;
SoftwareSerial df_player_Module(10, 11); // DFPlayer 시리얼 통신용 객체 생성 >> 디지털 핀 두개 선택
SoftwareSerial bt(RX,TX);
DFRobotDFPlayerMini df_player; // DFPlayer 객체 생성
int servo_angle = 0; // 5º ~ 175º 사이 제어 권장
int servo_delay = 10;
bool servo_going_zero = false;
int light_power = 0;
bool processing = false;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Function */

void spin_motor() // 모터를 회전시키는 함수
{
//  servo.write(0); // 정방향으로 최고속도로 회전(0에 가까울 수록 정방향 최고속도, 180에 가까울 수록 역방향 최고속도)

    if(servo_going_zero == false)
    {
      servo_angle++;
      servo.write(servo_angle);
      delay(servo_delay);
      if(servo_angle >= 180) servo_going_zero = true;
    }
    else
    {
      servo_angle--;
      servo.write(servo_angle);
      delay(servo_delay);
      if(servo_angle <= 0) servo_going_zero = false;
    }
}

void light_sensor() // 조도센서 레이저 감지 함수
{
  light_power = analogRead(A1);
  light_power = map(light_power,0,1024,0,100);
  Serial.println(light_power);

  if(light_power > 77)
  {
    is_hit();
  }
  else
  {
    //digitalWrite(TEST_LED, LOW);
  }
}

void is_hit()
{
  df_player.play(1); // mp3 파일 폴더의 1번 파일 재생
  delay(1000);
}

void cmd_ctrl(char cmd)
{
  switch(cmd)
  {
    /******************************************/
    // 프로세싱 시작
    case 'g':
    
        df_player.play(2);
        processing = true;
        //digitalWrite(TEST_LED, HIGH);
        break;
    case 'G':
        df_player.play(2);
        processing = true;
        //digitalWrite(TEST_LED, HIGH);
        break;
    /******************************************/
    // 프로세싱 정지
    case 's':
        processing = false;
        //digitalWrite(TEST_LED, LOW);
        break;
    case 'S':
        processing = false; 
        //digitalWrite(TEST_LED, LOW);
        break;
    /******************************************/
    // 가상 피격
    case 'H':
        is_hit();
        break;
    case 'h':
        is_hit();
        break;
    /******************************************/
    // 파일 확인용 랜덤 전체 재생
    
    case 'A':
        df_player.randomAll();
        break;
    case 'a':
        df_player.randomAll();
        break;
    
    /******************************************/
  }
}

void bluetooth_process()
{
  /*
   AT
   AT+NAME
   AT+PIN
   AT+PASS(아이폰)
   */
   
  if(Serial.available() > 0)
  {
    /* PC에서 전달된 정보가 있으면 한 문자씩 읽어서, 블루투스 모듈에 전달 */
    char cmd = Serial.read();
    bt.write(cmd);
  }
  if(bt.available() > 0)
  {
    /* 블루투스 모듈 에서 전달된 정보가 있으면 PC에 전달 */
    char cmd = bt.read();
    Serial.write(cmd);
    cmd_ctrl(cmd);
  }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  df_player_Module.begin(9600);
  bt.begin(9600);
  servo.attach(SERVO_PIN); // 모터 핀 연결
  servo.write(servo_angle);

  /*****************************************************************/
  /* DFPlayer 객체 초기화 */
  if (!df_player.begin(df_player_Module), false)
  {
//    Serial.println(F("Unable to begin:"));
//    Serial.println(F("1.Please recheck the connection!"));
//    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  delay(1);
  df_player.volume(20); // 볼륨 지정
  df_player.setTimeOut(500); //시리얼 통신용 타임아웃 시간 지정
//  Serial.println(F("DFPlayer Mini online."));
  /*****************************************************************/
}

void loop() {

  bluetooth_process();
  
  if(processing == true)
  {
    spin_motor();
    light_sensor();
  }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
