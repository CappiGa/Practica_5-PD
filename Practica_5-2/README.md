
# Práctica 5-2 - Buses de comunicación I (I2C) 
## **1. Descripción**  
Esta práctica implementa el uso del bus **I2C** para controlar una pantalla **OLED SSD1306** y un sensor **AHT20/AHT10** (temperatura y humedad) con el **ESP32-S3**. Se demuestra cómo mostrar datos en la pantalla OLED y enviar información de depuración al monitor serie.  

---

## **2. Objetivos**  
- Controlar una **pantalla OLED SSD1306** mediante I2C.  
- Leer datos de un sensor **AHT20/AHT10** (temperatura y humedad).  
- Mostrar información en la pantalla OLED y en el **monitor serie** para depuración.  
- Analizar el código y el flujo de comunicación I2C.  

---

## **3. Materiales**  
- **ESP32-S3**  
- **Pantalla OLED SSD1306** (I2C)  
- **Sensor AHT20/AHT10** (I2C)
- **Cable USB-C**  

---

## **4. Desarrollo**  

### **4.1 Preparación del entorno**
1. Crear un nuevo proyecto en PlatformIO:   
2. Configurar el archivo `platformio.ini`con el siguiente contenido:  
   ```ini
   [env:esp32s3]
   platform = espressif32
   board = esp32-s3-devkitc-1
   framework = arduino
   monitor_speed = 115200
   lib_deps = 
       robtillaart/I2C_LCD@^0.2.4
       adafruit/Adafruit AHTX0@^2.0.5
   ```  

### **4.2 Programación**  
Código en `main.cpp`:  
```cpp
#include <Arduino.h>
#include <I2C_LCD.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;  // Objeto para el sensor AHT20/AHT10

// Configuración pines LCD (ajustar según conexión)
#define BACKLIGHT_PIN   3
#define En_pin          2
#define Rw_pin          1
#define Rs_pin          0
#define D4_pin          4
#define D5_pin          5
#define D6_pin          6
#define D7_pin          7

I2C_LCD lcd(39);  // Dirección I2C del LCD (0x27 en hexadecimal)

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Esperar conexión serie (solo para debug)
  Serial.println("Iniciando pantalla OLED y sensor AHT...");

  // Configurar LCD
  lcd.config(39, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin, BACKLIGHT_PIN, POSITIVE);
  Wire.begin(10, 11);  // Pines SDA=10, SCL=11 (ajustar según placa)
  Wire.setClock(100000);  // Frecuencia I2C: 100 kHz
  lcd.begin(20, 4);  // Inicializar LCD 20x4

  // Inicializar sensor AHT
  if (!aht.begin()) {
    Serial.println("Error: Sensor AHT no encontrado. Verificar conexiones.");
    while (1);  // Bucle infinito si falla
  }
  Serial.println("Sensor AHT20/AHT10 detectado correctamente.");
}

void loop() {
  // Leer datos del sensor
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  // Mostrar datos en el monitor serie
  Serial.print("Temperatura: "); 
  Serial.print(temp.temperature); 
  Serial.println(" °C");
  Serial.print("Humedad: "); 
  Serial.print(humidity.relative_humidity); 
  Serial.println("% rH");

  // Mostrar datos en la pantalla OLED
  lcd.home();
  lcd.setCursor(3, 0);
  lcd.print("Temp:");
  lcd.setCursor(9, 0);
  lcd.print(temp.temperature);
  lcd.print(" C");

  lcd.setCursor(3, 1);
  lcd.print("Hum:");
  lcd.setCursor(9, 1);
  lcd.print(humidity.relative_humidity);
  lcd.print("%");

  // Mensaje adicional en LCD
  lcd.setCursor(3, 3);
  lcd.print("Practica I2C");

  delay(2000);  // Esperar 2 segundos entre actualizaciones
}
```  

**Explicación del código**:  
- **Inicialización**:  
  - `Wire.begin(10, 11)`: Configura los pines I2C (SDA=10, SCL=11).  
  - `lcd.begin(20, 4)`: Inicializa un LCD virtual de 20 columnas x 4 filas.  
- **Lectura del sensor**:  
  - `aht.getEvent()` obtiene los valores de temperatura y humedad.  
- **Salidas**:  
  - **Monitor serie**: Muestra los datos del sensor para depuración.  
  - **Pantalla OLED**: Muestra un texto personalizado, la temperatura y humedad.  

### **4.3 Conexión física**  
1. Conectar **SDA** y **SCL** del ESP32-S3 a la pantalla OLED y al sensor AHT20.  
2. Alimentar los dispositivos con **3.3V**.  

---

## **5. Resultados**  
- **Salida del monitor serie**:  
  ```plaintext
  Iniciando pantalla OLED y sensor AHT...
  Sensor AHT20/AHT10 detectado correctamente.
  Temperatura: 25.3 °C
  Humedad: 45.2% rH
  ```  
- **Pantalla OLED**:  
  ```plaintext
  "hola manuel, te sentaste en el pastel"
  Temp: 25.3 C
  Hum: 45.2%
  ```  

---
