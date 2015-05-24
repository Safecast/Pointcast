// A3GS sample skech.1 -- getRSSI

#include <SoftwareSerial.h>
#include "a3gim.h"

void setup()
{

}

void loop()
{
    read_rssi();
}

float read_rssi()
{
  Serial.begin(9600);
  unsigned int result = 0;
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
        // return the result:
        result = rssi;
       return(result);
    }
  }
  else
    Serial.println("Failed.");

  Serial.println("Shutdown..");
  a3gs.end();
  a3gs.shutdown();
}
