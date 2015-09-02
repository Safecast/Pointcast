// A3GS sample sketch -- check current baudrate

#include <SoftwareSerial.h>
#include "a3gim.h"

long baudrates[7] = { 2400, 4800, 9600, 19200, 38400, 57600, 115200 };

void setup()
{
  Serial.begin(9600);
  Serial.println("Ready.");

  Serial.println("Initializing.. ");
  for (int i = 0; i < 7; i++) {
    if (a3gs.start() == 0) {
      Serial.print("Try baudrate: ");
      Serial.println(baudrates[i]);
      if (a3gs.begin(0, baudrates[i]) == 0) {
        Serial.println("Recognize succeeded.");
        Serial.print("Current baudrate is ");
        Serial.print(baudrates[i]);
        Serial.println(" bps.");
        return;
      }
      Serial.println("Failed.");
      a3gs.end();
      a3gs.shutdown();
    }
  }
  Serial.println("Can't recognize baudrate.");
}

void loop()
{
}

