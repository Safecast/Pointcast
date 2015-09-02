// A3GS sample sketch -- getStatus
//  [[Note]] Function "getStatus()" is beta version.

#include <SoftwareSerial.h>
#include <a3gim.h>

void setup()
{
  Serial.begin(9600);
  delay(3000);  // Wait for Start Serial Monitor
  Serial.println("Ready.");

  Serial.print("Initializing.. ");

  if (a3gs.start() == 0 && a3gs.begin() == 0) {
    Serial.println("Succeeded.");
    int  status;
    status = a3gs.getStatus();
    Serial.print("Status is ");
    switch (status) {
      case A3GS::ERROR :
        Serial.println("ERROR");
        break;
      case A3GS::IDLE :
        Serial.println("IDLE");
        break;
      case A3GS::READY :
        Serial.println("READY");
        break;
      case A3GS::TCPCONNECTEDCLIENT :
        Serial.println("TCPCONNECTEDCLIENT");
        break;
      default :
        Serial.println("Unknown");
        break;
    }
  }   

  Serial.println("Shutdown..");
  a3gs.end();
  a3gs.shutdown();
}

void loop()
{
}
