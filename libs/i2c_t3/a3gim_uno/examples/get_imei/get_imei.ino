// A3GS sample sketch.4 -- getIMEI

#include <SoftwareSerial.h>
#include "a3gim.h"

void setup()
{
  Serial.begin(9600);
  delay(3000);  // Wait for Start Serial Monitor
  Serial.println("Ready.");

  Serial.print("Initializing.. ");
  if (a3gs.start() == 0 && a3gs.begin() == 0) {
    Serial.println("Succeeded.");
    char imei[a3gsIMEI_SIZE];
    if (a3gs.getIMEI(imei) == 0) {
      Serial.print("IMEI: ");
      Serial.println(imei);
    }
  }
  else
    Serial.println("Failed.");

  Serial.println("Shutdown..");
  a3gs.end();
  a3gs.shutdown();
}

void loop()
{
}
