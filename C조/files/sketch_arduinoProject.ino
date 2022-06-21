#include<DHT.h>             //DHT 센서용 라이브러리 헤더파일 (툴-라이브러리관리)
#include<LiquidCrystal.h>   //LCD 라이브러리 헤더파일
#define DCpin       10      //DC모터 핀 정의
#define pinTrigger  7       //초음파 센서 트리거 핀 정의
#define pinEcho     6       //초음파 센서 에코 핀 정의
#define pinDHT      8       //DHT입력핀 정의
#define swPin       13      //스위치 핀 정의
#define autoSW      9       //슬라이드 스위치 자동 설정


int temp;                   //온도 값 저장용 변수
int humi;                   //습도 값 저장용 변수
DHT dht(pinDHT,DHT11);      //DHT 제어용 객체(변수) 생성
LiquidCrystal lcd(12,11,5,4,3,2);      //LCD 제어 객체(변수) 생성, 초기화

//=========================================================================
void DCSwitch(int swDC){
  digitalWrite(DCpin, swDC);
}
//=========================================================================
void DCControl(unsigned long distance){
    if (distance <=30){
    DCSwitch(HIGH);
    Serial.println("작동");
  }
  else{
    DCSwitch(LOW);
    Serial.println("미작동");
  }
}
//=========================================================================
unsigned long getDistance(void){
   unsigned long distance;
  
  //초음파 발생시키는 코드
  digitalWrite(pinTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrigger,LOW);
  
  //초음파 수신 결과 읽어 거리 계산하기
  distance = pulseIn(pinEcho, HIGH)/2 * 0.034;
  return distance;
}
//=========================================================================
void setup()
{
   Serial.begin(9600);          //통신 설정
   pinMode(DCpin,OUTPUT);       // 모터와 제어용 변수와의 연결
   pinMode(swPin,INPUT_PULLUP); //스위치 값 읽어오기
   pinMode(pinTrigger, OUTPUT); //초음파 주파수 전송
   pinMode(pinEcho, INPUT);     //초음파 주파수 수신
   dht.begin();                 //DHT11 초기화
   lcd.begin(16,2);             //LCD 화면 크기 설정. 16글자 2줄.
}
//=========================================================================
void loop()
{
  //DC모터
  unsigned long distance;         //거리 변수 선언
  distance = getDistance();       //함수에서 계산 된 거리 값 대입
  
  int readValue =digitalRead(9);  // 9번 핀에서 자동/수동 값 넣어 변수 초기화
  Serial.println("readValue: " + String(readValue));      //시리얼 모니터에 출력
  
  if(readValue == HIGH) {         //읽어온 값이 HIGH인 경우 수동 모드이므로 
   digitalWrite(10, HIGH);        //즉시 팬 가동
  }
  else if(readValue == LOW){        //읽어온 값이 LOW인 경우 자동 모드이므로 
    DCControl(distance);          //초음파 센서가 읽은 거리 값에 따라 팬 가동
  }
  //거리값 화면에 출력하기
  Serial.println("Distance: " + String(distance) + "cm"); 
  //거리를 문자열로 바꿔야 출력 붙여서 가능
  delay(200);
//=========================================================================
  
  // 온도, 습도 값을 DHT11에서 읽는다.
  temp = dht.readTemperature();  //온도 읽기
  humi = dht.readHumidity();     //습도 읽기

  //읽은 값 출력
  Serial.print("temp: "); Serial.print(temp); Serial.print(", ");
  Serial.print("humi: "); Serial.println(humi); 

  
  // 1. 스위치의 값을 읽는다.
  int swValue = digitalRead(swPin);
  
  
  // 2. 스위치의 값을 시리얼 모니터에 값을 출력한다
  Serial.println(swValue);
  
  // 3. 스위치의 값에 따라 LCD에 출력한다.
  if(swValue == 0){
     //읽은 값 lcd에 출력하기
      lcd.clear();
      lcd.print("Temperature: "); lcd.print(temp);
      lcd.setCursor(0,1);  //두번째줄(1) 첫위치(0)으로 이동
      lcd.print("   Humidity: "); lcd.print(humi);
      delay(500);
  } else {
    lcd.clear();  //스위치 안누르면 값을 지운다.
  }
}
