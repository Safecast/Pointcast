// A3GS sample sketch.13-- setLED

#include <SoftwareSerial.h>
#include <a3gim.h>

#define  INTERVAL  1000  // Blink interval

void setup()
{
  Serial.begin(9600);
  delay(3000);  // Wait for Start Serial Monitor
  Serial.println("Ready.");

 _retry:
  Serial.print("Initializing.. ");
  if (a3gs.start() == 0 && a3gs.begin() == 0)
    Serial.println("Succeeded.");
  else {
    Serial.println("Failed.");
    Serial.println("Shutdown..");
    a3gs.end();
    a3gs.shutdown();
    delay(30000);
    goto _retry;  // Repeat
  }
  Serial.println("Blinking..");
}

void loop()
{
  a3gs.setLED(true);
  delay(INTERVAL);
  a3gs.setLED(false);
  delay(INTERVAL);
}

// END
