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

#define BMRDD_EEPROM_DOSE 200
// geiger dose
// write every hours (eeprom ~ 100000 cycles -> ~ 11 years)
#define BMRDD_EEPROM_DOSE_WRITETIME 3600

#define BMRDD_EEPROM_SETUP 300
#define BMRDD_EEPROM_MARKER 0x5afeF00d


#define ENABLE_3G             0
#define ENABLE_ETHERNET       1
#define ENABLE_EEPROM_DOSE    1

#define HEADER_SENSOR  "PNTXS"
#define HEADER  "PNTDD"

   // log file headers
#define LOGFILE_HEADER "# NEW LOG\n# format="


  typedef struct {
    unsigned long total_count;
    unsigned long total_time;
    unsigned long restarts;
    unsigned long fails;
    unsigned long logs;
  } DoseType;

  typedef struct {
  unsigned long marker;     // set at first run
  unsigned int devid;       // did
  unsigned int user_id;     // uid
  unsigned int user_id2;    // uid
  unsigned int devt1;       // device type 1 (see devcies Pointcast_XX) on api  
  unsigned int devt2;       // device type 2(see devcies Pointcast_XX) on api  
  char api_key[24];         // api
  char latitude[16];        // lat
  char longitude[16];       // lon
  byte sensor1_enabled;     // s1e
  float sensor1_cpm_factor; // s1f
  byte sensor2_enabled;     // s2e
  float sensor2_cpm_factor; // s2f
  byte dev;                 // dev
  char gw1[20];             // gw1
  char gw2[20];             // gw1
  char intf[2];             // intf
  unsigned int tws;         // tws
  unsigned int alt;         //alt
  byte autow;               //auto
  int alm;                  //alm
  char tz[3];               // in hours
  char ssid[16];            //SSID of WiFi;
  char pwd[16];             //password of WiFi
  char gwn[16];             //name of gateway
  char s1i[8];              //Sensor 1 Isotope
  char s2i[8];              //Sensor 2 Isotope
  byte aux;                 //auto
  byte trb;                 //troubleshooting
  char apn[3];              //APN name
  char macid[18];           //MAC id for Ethernet card
  int fails;                //fails (not on sdcard)
  unsigned int S1peak;      //S1 peak level
  unsigned int S2peak;      //S2 peak level
  char last_failure[12];    //Last failure message
  char tel[12];             //Phone number for 3G
  char ntp[12];             //NTP hardcoded

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
  //for debugging
  Serial.print(i);
  return i;
}

class PointcastSetup {

public:
  PointcastSetup(SoftwareSerial &openlog, 
    ConfigType &config,
    DoseType &dose,
    char * buffer, size_t buffer_size);
  void initialize();
  void loadFromFile(char * setupFile);

private:
  SoftwareSerial &mOpenlog;
  ConfigType &mConfig;
  DoseType &mDose;

  char * mBuffer;
  size_t mBufferSize;
};

#endif
