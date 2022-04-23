#include <SoftwareSerial.h>

SoftwareSerial vmonitor(2, 3); // RX | TX

String msg = "";

void setup() {
  Serial.begin(9600);
  vmonitor.begin(9600);
}

char pollSerial() {
  if (Serial.available()) {
    char c = Serial.read(); // gets one byte from serial buffer
    return c;
  }

  return ((char) 0);
}

void loop() {
    char c = pollSerial();
    
    if (c)
      msg += c;
    
    if (c == '!') {
      vmonitor.print("MASTER: Received: ");
      vmonitor.println(msg);
      msg = "";
    }
}
