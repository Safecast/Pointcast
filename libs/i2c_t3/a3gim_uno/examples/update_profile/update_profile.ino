// A3GS sample sketch -- updateProfile

#include <SoftwareSerial.h>
#include <a3gim.h>

uint8_t ep[] = "\x46\xD9\x35\x52\x22\x47\xD1\xAC\x3E\x84\xFD\x8\x13\xD7\x7A\x4D\x1B\xAB\x42\x5F\xFE\xFF\xA3\xAA\x14\x4E\xA1\x68\x8D\x5F\x3F\x3A\xD1\x5D\x11\x8\xF4\xF7\x37\x4C\xBB\x95\xA4\xD7\x2B\x8\x81\x9F\xF8\x63\x9E\x60\x59\x5A\x44\x69\x91\x31";
        // Original profile data is "iijmio.jp" as No.15 

void setup()
{
  Serial.begin(9600);
  delay(3000);  // Wait for Start Serial Monitor
  Serial.println("Ready.");

  Serial.print("Initializing.. ");
  if (a3gs.start() == 0 && a3gs.begin() == 0)
    Serial.println("Succeeded.");
  else {
    Serial.println("Failed.");
    Serial.println("Shutdown..");
  }

  if (a3gs.updateProfile(ep, (int)sizeof(ep)) == 0)
    Serial.println("updateProfile(): ok");
  else
    Serial.println("updateProfile(): error");
}

void loop()
{
}

// END
