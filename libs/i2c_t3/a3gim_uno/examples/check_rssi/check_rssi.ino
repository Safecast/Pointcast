// A3GS sample skech.1 -- getRSSI

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
    int rssi;
    if (a3gs.getRSSI(rssi) == 0) {
      Serial.print("RSSI = ");
      Serial.print(rssi);
      Serial.println(" dBm");
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
