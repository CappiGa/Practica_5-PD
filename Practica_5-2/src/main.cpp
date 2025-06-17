//    FILE: I2C_LCD_demo.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo I2C_LCD library
//     URL: https://github.com/RobTillaart/I2C_LCD

#include <Arduino.h>
#include <I2C_LCD.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;


//  test 20x4 + 16x2
#define BACKLIGHT_PIN   3
#define En_pin          2
#define Rw_pin          1
#define Rs_pin          0
#define D4_pin          4
#define D5_pin          5
#define D6_pin          6
#define D7_pin          7


I2C_LCD lcd(39);


void setup()
{
  lcd.home();
  lcd.setCursor(3,2);
  lcd.print(millis());
  delay(1000);

  lcd.home();
  lcd.setCursor(3,3);
  lcd.print("hola manuel, te sentaste en el pastel");
  delay(1000);
  Serial.begin(115200);
  while (!Serial);
  Serial.println(__FILE__);
  Serial.print("I2C_LCD_LIB_VERSION: ");
  Serial.println(I2C_LCD_LIB_VERSION);
  Serial.println();

  lcd.config(39, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin, BACKLIGHT_PIN, POSITIVE);

  Wire.begin(10,11);
  Wire.setClock(100000);
  lcd.begin(20, 4);
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");


}


void loop()
{

  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");
  
  lcd.home();
  lcd.setCursor(3,0);
  lcd.print("Temp:");
  lcd.setCursor(9,0);
  lcd.print(temp.temperature);
  lcd.home();
  lcd.setCursor(3,1);
  lcd.print("Hum:");
  lcd.setCursor(9,1);
  lcd.print(humidity.relative_humidity);
  

  delay(500);


}
  
