/*
   Simple library to handle Pointcast configuration from file and EEPROM
*/

#ifndef _Pointcast_SETUP_H
#define _Pointcast_SETUP_H

// Link to arduino library
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <EEPROM.h>
#include <SoftwareSerial.h>

#define BMRDD_EEPROM_SETUP 500
#define BMRDD_EEPROM_MARKER 0x5afeF00d

#define ENABLE_3G             0
#define ENABLE_ETHERNET       1
#define ENABLE_DEV            1
#define ENABLE_API            0

#define HEADER_SENSOR  "PNTXS"
#define HEADER  "PNTDD"

typedef enum {
  SENSOR_ENABLED_FALSE = 0,
  SENSOR_ENABLED_TRUE,
} SensorEnabled;

typedef struct {
  unsigned long marker;     // set at first run
  unsigned int devid;     // did
  unsigned int user_id;     // uid
  unsigned int user_id2;     // uid
  char api_key[24];         // api
  char latitude[16];        // lat
  char longitude[16];       // lon
  byte sensor1_enabled;     // s1e
  float sensor1_cpm_factor; // s1f
  byte sensor2_enabled;     // s2e
  float sensor2_cpm_factor; // s2f
  byte dev;                 // dev
  char gw1[16];             // gw1
  char gw2[16];             // gw1
  char intf[2];             // intf
  unsigned int tws;         // tws
  unsigned int alt;         //alt
  byte autow;               //auto
  unsigned int alm;         //alm
  char tz[3];                // in hours
  char ssid[16];            //SSID of WiFi
  char pwd[16];             //password of WiFi
  char gwn[16];             //name of gateway
  char s1i[8];              //Sensor 1 Isotope
  char s2i[8];              //Sensor 2 Isotope
  byte aux;                 //auto
  byte trb;                 //troubleshooting

  
} ConfigType;

// Write a template value into EEPROM address [ee]
template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

// Read a template value from EEPROM address [ee]
template <class T> int EEPROM_readAnything(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}

class PointcastSetup {

public:
  PointcastSetup(SoftwareSerial &openlog, 
        ConfigType &config,
        char * buffer, size_t buffer_size);
  void initialize();
  void loadFromFile(char * setupFile);

private:
  SoftwareSerial &mOpenlog;
  ConfigType &mConfig;

  char * mBuffer;
  size_t mBufferSize;
};

#endif
