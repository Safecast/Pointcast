// A3GS sample sketch.5 -- getLocation

#include <SoftwareSerial.h>
#include "a3gim.h"

void setup()
{
  Serial.begin(9600);
  delay(3000);    // Wait for start serial monitor
  Serial.println("Ready.");

  Serial.print("Initializing.. ");
  if (a3gs.start() == 0 && a3gs.begin() == 0) {
    Serial.println("Succeeded. It maybe takes several minutes.");
    char lat[15], lng[15];
    if (a3gs.getLocation(a3gsMPBASED, lat, lng) == 0) {
      Serial.print("OK: ");
      Serial.print(lat);
      Serial.print(", ");
      Serial.println(lng);
    }
    else
      Serial.println("Sorry, I don't know this location.");
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
