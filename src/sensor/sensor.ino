#include <Wire.h>
#include <SoftwareSerial.h>

// SHT25 I2C address is 0x40(64)
#define Addr 0x40

SoftwareSerial virtualMonitor(2, 3); // RX | TX

void setup() {
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);
  virtualMonitor.begin(9600);
}

struct SensorData {
  float humidity;
  float cTemp;
  float fTemp;
};

struct SensorData PollSHT25() {
  struct SensorData result;
  unsigned int data[2];
  
  // Each of the following 3 lines will do
  // one of the following actions, respectively:
  // 1. Start I2C transmission
  // 2. Send humidity measurement command, NO HOLD master
  // 3. Stop I2C transmission
  
  Wire.beginTransmission(Addr);
  Wire.write(0xF5);
  Wire.endTransmission();
  delay(500);
  
  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);
  
  // Read 2 bytes of data in the following format:
  // [humidity msb, humidity lsb]
  if(Wire.available() == 2) {
    data[0] = Wire.read();
    data[1] = Wire.read();
    
    // Convert the data
    float humidity = (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;
    result.humidity = humidity;
    // Output data to Serial virtualMonitor
    virtualMonitor.print("Relative Humidity :");
    virtualMonitor.print(humidity);
    virtualMonitor.println(" %RH");
  }

  // Each of the following 3 lines will do
  // one of the following actions, respectively:
  // 1. Start I2C transmission
  // 2. Send temperature measurement command, NO HOLD master
  // 3. Stop I2C transmission
  
  Wire.beginTransmission(Addr);
  Wire.write(0xF3);
  Wire.endTransmission();
  delay(500);
  
  // Request 2 bytes of data  
  Wire.requestFrom(Addr, 2);
  
  // Read 2 bytes of data in the following format:
  // [temp msb, temp lsb]
  if(Wire.available() == 2) {
    data[0] = Wire.read();    
    data[1] = Wire.read();
    
    // Convert the data
    float cTemp = (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;
    float fTemp = (cTemp * 1.8) + 32;

    result.cTemp = cTemp;
    result.fTemp = fTemp;
    
    // Output data to Serial virtualMonitor
    virtualMonitor.print("Temperature in Celsius :");
    virtualMonitor.print(cTemp);
    virtualMonitor.println(" C");
    virtualMonitor.print("Temperature in Fahrenheit :");
    virtualMonitor.print(fTemp);
    virtualMonitor.println(" F");
    virtualMonitor.println("---");
  }

  return result;
}

void sendSerial(struct SensorData* sensorData) {
  // Data will be sent in the following format:
  // "H<humidity>C<temperature>!"
  
  Serial.print("H");
  Serial.print((int) sensorData->humidity);
  Serial.print("C");
  Serial.print((int) sensorData->cTemp);
  Serial.print("!");
}

void loop() {
  struct SensorData sensorData = PollSHT25();
  sendSerial(&sensorData);

  delay(1000);
}
