// A3GS sample sketch.2 -- getServices

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
    int status;
    if (a3gs.getServices(status) == 0) {
      switch (status) {
        case a3gsSRV_NO :
          Serial.println("No Service.");
          break;
        case a3gsSRV_PS :
          Serial.println("Packet Service Only.");
          break;
        case a3gsSRV_CS :
          Serial.println("Voice Service Only.");  // also SMS
          break;
        case a3gsSRV_BOTH :
          Serial.println("Packet And Voice Services.");
          break;
        default :
          break;
      }
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
