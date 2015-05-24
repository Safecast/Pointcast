// A3GS sample sketch.12 -- setDefaultProfile and getDefaultProfile

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
    int no;
    if (a3gs.getDefaultProfile(&no) == 0) {
      Serial.print("Default Profile Number is ");
      Serial.println(no);
    }

    if (no == 1)
      no = 2;
    else
      no = 1;

    if (a3gs.setDefaultProfile(no) == 0) {
      Serial.print("Set Default Profile Number as ");
      Serial.println(no);
    }
    else
      Serial.println("Failed.");

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
