// A3GS sample sketch.10 -- tweet
//  [[Note]] REPLACE "token" and "message" WITH YOUR GOT TOKEN AND MESSAGE BEFORE UPLOAD THIS SKETCH.
//           THIS TEST SKETCH USE http://arduino-tweet.appspot.com/ 's SERVICE. PLEASE CHECK DETAIL IN THIS SITE.

#include <SoftwareSerial.h>
#include "a3gim.h"

const char *token = "YOUR_TOKEN_HERE";
const char *message = "TWEET_MESSAGE_HERE";
  //-- Note: can't tweet same message continuously.

void setup()
{
  Serial.begin(9600);
  delay(3000);  // Wait for Start Serial Monitor
  Serial.println("Ready.");

  Serial.print("Initializing.. ");
  if (a3gs.start() == 0 && a3gs.begin() == 0) {
    Serial.println("Succeeded.");
    Serial.print("tweet() requesting.. ");
    if (a3gs.tweet(token, message) == 0)
      Serial.println("OK!");
    else
      Serial.println("Can't tweet.");
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
