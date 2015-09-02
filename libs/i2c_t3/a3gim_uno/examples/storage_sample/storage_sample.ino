// A3GS sample sketch -- put/get

#include <SoftwareSerial.h>
#include "a3gim.h"

#define SNO  3
uint8_t data[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    'A', 'R', 'D', 'U', 'I', 'N', 'O', 'U', 'N', 'O',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
};
uint8_t buffer[sizeof(data)];

void setup()
{
  Serial.begin(9600);
  delay(3000);    // Wait for start serial monitor
  Serial.println("Ready.");

  Serial.print("Initializing.. ");
  if (a3gs.start() == 0 && a3gs.begin() == 0) {
    Serial.println("Succeeded.");

    if (a3gs.put(SNO, data, sizeof(data)) == a3gsSUCCESS) {
        // put() succeed
        int sz = a3gs.get(SNO, buffer, sizeof(buffer));
        if (sz > 0) {
            for (int i = 0; i < sz; i++) {
                Serial.print(buffer[i], HEX);
                Serial.print(" ");
            }
            Serial.println("");
        }
        else
      Serial.println("Can't get data.");
    }
    else
      Serial.println("Can't put data.");
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
