#include <LiquidCrystal_I2C.h>
#define lcdAddress 0x27
#define lcdColumn 20
#define lcdRows 4
LiquidCrystal_I2C lcd(lcdAddress, lcdColumn, lcdRows);

#define gasPine 34

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  pinMode(gasPine, INPUT);
}

void loop() {
  const float BETA = 3950;
  int analogValue = analogRead(gasPine);
  float celsius = 1 / (log(1 / (4095. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
  int temp = static_cast<int>(celsius);
  lcd.setCursor(0, 0);
  lcd.print("   ");
  lcd.setCursor(0, 0);
  lcd.print(celsius,1);
  delay(500);
}
