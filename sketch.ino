#include <LiquidCrystal_I2C.h>

#define lcdAddress 0x27
#define lcdColumn 20
#define lcdRows 4
LiquidCrystal_I2C lcd(lcdAddress, lcdColumn, lcdRows);

#define greenLedPin 12
#define redLedPin 4
#define GAS_SENSOR_PIN 32   // Pin GPIO32 conectado a OUT0 del custom chip
#define ALERT_PIN 2         // Pin GPIO2 para recibir la alerta

void setup() {
  Serial.begin(9600); 
  lcd.init();
  lcd.backlight();

  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(ALERT_PIN, INPUT); // Este pin leerá la señal de OUT1
}

void loop() {
  // Leer el valor del gas del custom chip
  int gasValue = analogRead(GAS_SENSOR_PIN);
  float gasPercentage = (gasValue * 100.0) / 4095; 

  // Actualizar el valor en el LCD solo si cambia significativamente
  lcd.setCursor(0, 0);
  lcd.print("Gas: ");
  lcd.setCursor(5, 0);
  lcd.print(gasPercentage, 1); // Mostrar un decimal
  lcd.print("%   "); 

  // Imprimir valor en el Monitor Serial para depuración
  Serial.print("Gas Value Raw: ");
  Serial.print(gasValue);
  Serial.print(" Gas Percentage: ");
  Serial.println(gasPercentage);

  // Leer el estado de ALERT_PIN y controlar los LEDs
  int alertState = digitalRead(ALERT_PIN);
  if (alertState == HIGH) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
  } else {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
  }

  delay(500); 
}
