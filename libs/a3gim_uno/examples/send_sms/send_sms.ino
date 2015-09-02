// A3GS sample sketch.9 -- sendSMS (Japanease message version)
// PLEASE REPLACE "msn" WITH CORRECT TELEPHONE NUMBER BEFORE UPLOAD THIS SKETCH.

#include <SoftwareSerial.h>
#include "a3gim.h"

char *msn = "09012345678";    // Replace your phone number!
// char *msg = "TEST MESSAGE. HELLO!";  // ASCII String
char msg[] = { 0x53, 0x30, 0x8c,0x30, 0x6f, 0x30, 0xc6, 0x30, 0xb9, 0x30, 0xc8, 0x30, 0x67, 0x30, 0x59, 0x30, 0x00 };
  // Japanease written in UNICODE

void setup()
{
  Serial.begin(9600);
  delay(3000);  // Wait for Start Serial Monitor
  Serial.println("Ready.");

  Serial.print("Initializing.. ");
  if (a3gs.start() == 0 && a3gs.begin() == 0) {
    Serial.println("Succeeded.");
    Serial.print("SMS Sending.. ");
    if (a3gs.sendSMS(msn, msg, a3gsCS_UNICODE) == 0)
      Serial.println("OK!");
    else
      Serial.println("Can't send SMS.");
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
