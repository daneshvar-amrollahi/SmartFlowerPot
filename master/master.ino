#include <SoftwareSerial.h>

SoftwareSerial hc05(2, 3); // RX | TX

void setup() {
  Serial.begin(9600);
  hc05.begin(9600);
}

void loop() {
  while (hc05.available()) {
    delay(10);
    if (hc05.available() > 0) {
      char c = hc05.read();  //gets one byte from serial buffer
      Serial.print("Received ");
      Serial.print(c);
      Serial.println("");
    }
  }
  Serial.println("No luck so far");
  delay(200);
}
