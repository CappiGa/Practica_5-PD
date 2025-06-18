# **Práctica 5-1 - Buses de comunicación I (I2C)**  

## **1. Descripción**  
Esta práctica introduce el uso del bus **I2C** en el **ESP32-S3** para detectar dispositivos conectados mediante un escáner de direcciones. Se explora el protocolo I2C, sus características y su implementación en entornos embebidos, utilizando la librería `Wire.h` de Arduino.  

---

## **2. Objetivos**  
- Comprender el funcionamiento del bus **I2C** y su arquitectura maestro-esclavo.  
- Implementar un **escáner de direcciones I2C** para identificar dispositivos conectados.  
- Analizar las ventajas y limitaciones del protocolo I2C en comparación con otros buses.  

## **3. Introducción teórica**  
#### **Introducción a los buses**  
Los buses permiten la comunicación entre componentes mediante señales eléctricas y protocolos estandarizados. Se clasifican en:  
- **Paralelos**: Múltiples líneas de datos (mayor ancho de banda, pero susceptible a interferencias).  
- **Serie**: Un único canal de datos (menos cables, ideal para distancias largas).  

#### **Bus I2C**  
- **Características**:  
  - Protocolo **síncrono** y **half-duplex** (2 cables: SDA para datos, SCL para reloj).  
  - Arquitectura **maestro-esclavo** (hasta 112 dispositivos con direcciones de 7 bits).  
  - Velocidades estándar: **100 kHz** (modo normal) o **400 kHz** (modo rápido).  
- **Ventajas**:  
  - Conexión simple (2 cables + resistencias pull-up).  
  - Mecanismos de ACK/NACK para verificar recepción.  
- **Desventajas**:  
  - Velocidad limitada.  
  - No es full-duplex.  

---

## **4. Materiales**  
- **ESP32-S3**  
- **Dispositivos I2C** (ej. LCD, sensores).   
- **Cable USB-C**.  

![Image](https://github.com/user-attachments/assets/4052c0ac-19b0-4646-9070-ae7de512deca)
---

## **5. Desarrollo**  

### **5.1 Preparación del entorno**  
1. Crear un nuevo proyecto en PlatformIO
2. Configurar el archivo `platformio.ini` con el siguiente contenido: 
   ```ini
   [env:esp32-s3-devkitm-1] 
   platform = espressif32
   board = esp32-s3-devkitm-1
   framework = arduino
   monitor_speed = 115200
   lib_deps = 
       robtillaart/I2C_LCD@^0.2.4
       adafruit/Adafruit AHTX0@^2.0.5
   ```  

### **5.2 Programación**  
Código en `main.cpp`:  
```cpp
#include <Arduino.h>
#include <Wire.h>

void setup() {
  Wire.begin(); // Inicia I2C (SDA=GPIO21, SCL=GPIO22)
  Serial.begin(115200);
  while (!Serial); // Espera conexión serie (solo para placas con USB nativo)
  Serial.println("\nI2C Scanner");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) { // Dispositivo encontrado
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    } else if (error == 4) { // Error desconocido
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("Scan completed\n");

  delay(5000); // Espera 5 segundos entre escaneos
}
```  

**Explicación:**  
- **`Wire.begin()`**: Inicializa el bus I2C con los pines por defecto (SDA=21, SCL=22).  
- **Escaneo de direcciones**:  
  - El bucle `for` prueba cada dirección I2C (1-127).  
  - `Wire.beginTransmission(address)` intenta comunicarse con el dispositivo.  
  - Si `error == 0`, el dispositivo responde (dirección válida).  
- **Salida serie**: Muestra las direcciones de los dispositivos detectados en hexadecimal (ej. `0x27` para un LCD común).  

### **5.3 Conexión física**  
1. Conectar **SDA** (GPIO21) y **SCL** (GPIO22) a los dispositivos I2C.    
2. Alimentar los dispositivos con **3.3V** (compatible con ESP32-S3).  

---

## **6. Resultados**  
- **Salida del monitor serie** (ejemplo con un sensor y un LCD conectados):  
  ```plaintext
  I2C Scanner
  Scanning...
  I2C device found at address 0x27 !
  I2C device found at address 0x38 !
  Scan completed
  ```
  ![Image](https://github.com/user-attachments/assets/bb424363-8d83-4c80-8199-44ba1a82ffa3)
  
- **Funcionamiento**:  
  - El escáner identifica las direcciones de los dispositivos cada 5 segundos.  
  - Si no hay dispositivos, muestra `No I2C devices found`.  

---
  
