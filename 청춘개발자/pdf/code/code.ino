#include <LiquidCrystal_I2C.h>/*LiquidCrystal_I2C*/

#define Humi      A0
#define ledRed    10
#define ledGreen  9
#define ledBlue   11

#define LCD_I2C_ADDR 0x27

int relay = 8;
int soilHumidity;
LiquidCrystal_I2C lcd(LCD_I2C_ADDR, 16, 2);

/*LCD 습도 프린트하기*/
void printLcd() {
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Present: 50%");
  lcd.setCursor(0, 1);
  lcd.print("Moisture: ");
  lcd.print(soilHumidity);
  lcd.print("%");
}

/*LCD 초기화하기*/
void initLcd() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
}

/*토양습도 계산하기*/
void calcSoilHumidity() {
  soilHumidity = map(analogRead(Humi), 1000, 400, 0, 100);
  if(soilHumidity > 100) soilHumidity = 100;
  else if(soilHumidity < 0) soilHumidity = 0;
}

void writeRGB(bool R, bool G, bool B) {
  digitalWrite(ledRed, R);
  digitalWrite(ledGreen, G);
  digitalWrite(ledBlue, B);
}

void sensor() {
  /* 습도가 낮아지면, 펌프가 동작되고
     습도가 높아지면, 펌프가 꺼집니다. */  
  if (soilHumidity < 20)  {
    writeRGB(HIGH, LOW, LOW);
    delay(300);
    digitalWrite(relay, LOW);
    delay(400);  
    digitalWrite(relay, HIGH);
    delay(2000);
  } else {
    writeRGB(HIGH, LOW, HIGH);
    digitalWrite(relay, HIGH);
    delay(2000);
  }
}

void setup() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(relay, OUTPUT);

  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledBlue, LOW);
  
  initLcd();
  
  delay(2000);
}

void loop() {
  calcSoilHumidity();
  printLcd();
  sensor();
} 
