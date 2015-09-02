// A3GS sample sketch.7 -- getTime2
//   check result by http://www.epochconverter.com/

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
    uint32_t seconds;
    if (a3gs.getTime2(seconds) == 0) {
      Serial.print(seconds);
      Serial.println(" Sec.");
    }
    else
      Serial.println("Can't get seconds.");  
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
