/*
    A fixed position Safecast device for Radiation measurement.

   Copyright (c) 2015, Safecast

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


History Versions:

2015-04-05 V2.4.9 delay for switching off LEDs
2015-04-07 V2.6.0 merged code with 3G
2015-04-07 V2.6.1 beeper setup and code cleaning(need jumper from D10 in Arduino shield (is pin D27)to A3)
2015-04-08 V2.6.3 setup for measuring voltage on A13
2015-04-08 V2.6.4 made switch for sending to dev or API
2015-04-14 V2.6.6 added heart beat on green LED and reading setup for files
2015-04-19 V2.6.7 added setup files now 1024 byte possible . added setup variables
2015-04-20 V2.6.8 added device ID, startup screen change, header for file format changed to NGRDD
2015-04-23 V2.6.9 added second line to SDcard logging for added status NNXSTS, added menus
2015-04-23 V2.7.0 renamed Pointcast
2015-04-28 V2.7.1 moved  startup 3G into send string, battery voltage report corrected for Teensy.
2015-04-30 V2.7.2 Added temperature setup for DS18B20 (disabled at the moment) setup screens
2015-05-05 V2.7.3 Added Joy stick setup. Added Height. Fixed lot/lan sending information on Ethernet
2015-05-08 V2.7.4 Added new screens for setup and error reporting
2015-05-09 V2.7.5 Changed startup name
2015-05-10 V2.7.6 Added Red LED warning on SDcard and Sensor fails, renamed SDcard files and updates headers
2015-05-11 V2.7.7 Updates headers. 3G display and header setup same as Ethernet card
2015-05-12 V2.7.8 Moved SDcard screen before time screen
2015-05-16 V2.7.9 Added Joystick interaction. Key down startup 1/5 of normal time. Prepare joy stick enter key (push down) for other functions.
2015-05-17 V2.8.0 Changed startup screen to be faster with 1 second display if joystick in pressed down. Blink the heartbeat LED for 1 sec.
2015-05-18 V2.8.1 Setup NTP for automatic update time on Ethernet.
2015-05-19 V2.8.2 Fixed first display readings of CPM (was too high)
2015-05-19 V2.8.3 Fixed 3G not starting up due to Ethernet settings.
2015-05-22 V2.8.4 Auto gateway setup (two gateways at the moment)
2015-05-22 V2.8.5 Display RSSI level in 3G.
2015-05-23 V2.8.6 SDCard RSSI level saved in 3G.
2015-05-30 V2.8.7 Menu updated.
2015-05-30 V2.8.8 Menu loop remake
2015-05-31 V2.8.9 Logfile name fix
2015-05-31 V2.9.0 Sending data through indextest_extra.php on 107.161.164.166 with no time-stamping and with extra information
2015-06-04 V2.9.1 Setup for fast sens mode selectable from SDcard with the "trb" option
2015-06-08 V2.9.2 Temperature setup truncated API key, battery/power setup changed.
2015-06-10 V2.9.3 Added skip sdcard init on one time pass. uSv/H changed to uSH..menu stat prepared...
2015-06-17 V2.9.4  Fixed second line temperature and batter logging on SDcard
2015-06-17 V2.9.5  3G second line recoding for SDcard fixes
2015-06-19 V2.9.6  Changed file naming SDCard
2015-06-20 V2.9.7  more fixed 3G second line
2015-06-21 V2.9.8  Switch between API and Dev sending through sdcard.
2015-06-22 V2.9.9  DEV status is displayed on LCD
2015-06-22 V3.0.0  Second line for extra information fix (final)
2015-06-25 V3.0.1  Device Type ID on SDCard and the API setup for Device typing LND7317 is 129 and LND712 is 130
2015-06-25 V3.0.2  Added height for dev to measurements.
2015-06-25 V3.0.3  Fixes for SDcard reading and Network startup
2015-06-25 V3.0.4  RTC tests moved after network startup
2015-07-27 V3.0.5  Setup for kcpm
2015-07-30 V3.0.6  Setup for logic for sending data with deviceType ID
2015-08-15 V3.0.7  Fixed CPM2 bug in display
2015-08-16 V3.0.8  3G RTC and display setup changed
2015-08-18 V3.0.9  Voltage display adjusted for volage drop over D103 lower board
2015-08-18 V3.1.0  Switched Teensy to internal ref mode for voltage measurements
2015-08-21 V3.1.1  Added devicetype_id to send sting inside measurement
2015-08-25 V3.1.2  MACid reading from SDcard and programming
2015-08-25 V3.1.3  SDcard fail lcd display
2015-08-25 V3.1.4  Voltage display direct without diode compensation and  3G displays carrier
2015-08-25 V3.1.5  Fixed always two digit display Ethernet Mac
2015-08-25 V3.1.6  Fixed Ethernet hang bug.
2015-08-25 V3.1.7  Fixed RTC bug.
2015-08-26 V3.1.8  Fixed Ethernet display hang on scroll back bug
2015-08-27 V3.1.9  Fixes for sending Ethernet
2015-09-01 V3.2.0  Fixes for variables listing data.
2015-09-04 V3.2.1  Fixes for failing SDcard data is still send
2015-09-05 V3.2.2  Sending DeviceType_ID
2015-09-06 V3.2.3  3G display setting changed
2015-09-06 V3.2.4  Set audio alarms for failing to connect for 3G and Ethernet on sending
2015-09-12 V3.2.5  Setup fails for Ethernet connections FAIL=1 means sensor 1 fails to send .. FAIL=2 means sensor 2 fails to send ....added delay for 3G sensing between sensors
2015-09-12 V3.2.6  Setup S1peak and S2peak and setup for uptime of sensor
2015-09-15 V3.2.7  Terminal displays last failure
2015-09-19 V3.2.8  Single sensor setup displaying no error on sensor test
2015-09-19 V3.2.9  Reset every week on Saturday 9:30am (UTC)
2015-10-03 V3.3.0  Beep 3 times and some display errors fix
2015-10-09 V3.3.1  Added longer delay for 3G sending second sensor
2015-10-09 V3.3.2  Fixed display failed error 3G.
2015-11-14 V3.3.3  Changed the way 3G connects to APN
2015-11-16 V3.3.4  Fixed display failed error 3G.
2015-11-16 V3.3.5  EEprom clearing on enter of joystick at system screen.
2015-11-17 V3.3.6  Switch off fail led by enter joy switch on main screen
2015-11-21 V3.3.7  Fixed display failed error 3G.
2015-11-24 V3.3.8  Fixed display failed error 3G.
2015-12-03 V3.3.9  Added dose storage for EEProm.
2015-12-09 V3.4.0  Stores total counts in EEprom
2015-12-16 V3.4.1  fixes fails display
2015-12-31 V3.4.2  Ethernet fixes fails display
2016-01-05 V3.4.3  Change parameters in 3GIM for longer timeout of network and setup for 1024 body lenght
2016-01-07 V3.4.4  Changed delays in 3GIM for stability
2016-01-08 V3.4.5  Error messages fixed
2016-01-15 V3.4.6  httpPost fixes for 3G
2016-01-15 V3.4.7  serial confirm messages fix for 3G
2016-01-24 V3.4.8  added code for switching temperature sensor
2016-01-26 V3.4.9  changed 3G send way by restarting 3G module every time send.
2016-02-01 V3.5.0  Added Alarm function.
2016-02-15 V3.5.1  3G fast reset
2016-02-28 V3.5.8  reset after 5 times
2016-03-02 V3.5.9  added code for correct data login name on SDcard.
2016-03-03 V3.6.0  Ethernet detect messages on fail
2016-03-05 V3.6.3  redone menus
2016-03-05 V3.6.4  Fixed time issues.
2016-03-06 V3.6.5  Weekly restart redone
2016-03-08 V3.6.6  clean out code
2016-03-09 V3.6.7  RTC menu
2016-03-11 V3.6.8  Ping to api.safecast.org
2016-03-13 V3.7.1  Redone screen flow
2016-03-13 V3.7.2  ping to Google.com/gw02/gw02 setup
2016-03-14 V3.7.3  setup timing for 3G refresh
2016-03-14 V3.7.4  ping http://safecast-production.s3.amazonaws.com
2016-03-14 V3.7.5  moved countdown and delayed displaying countdown
2016-03-16 V3.7.6  added fails, restarts and create file numbers to EEprom. Added restarts and fails to second line in SDCard
2016-03-17 V3.7.7  file creation counts (logs in stats) added.
2016-03-27 V3.7.8  Display API/DEV toggle implemented , logging new lot to sdcard with stats
2016-03-28 V3.7.9  Fixed after EPROM erase not counting error
2016-04-02 V3.8.0  Sliding window cpm calculation
2016-04-02 V3.8.1  Reading sdcard remove "\r" (windows)
2016-04-19 V3.8.2  push down key delay for menu
2016-04-21 V3.8.3  start counting after all screen of startup are done
2016-04-27 V3.8.4  5 minutes count test
2016-04-28 V3.8.5  fix for Reading sdcard remove "\r\n" (windows)
2016-04-29 V3.8.6  added phone number setup from sdcard for 3G, changed display data to 1 minute, changed NX to 12 and delay to 5000 for 5 seconds measuring, KCPM 2 charater after decimal point
2016-04-30 V3.8.7  fixed new line on new log on sdcard
2016-05-05 V3.8.8  5 minutes display
2016-05-15 V3.8.9  pool.ntp.org as time server dns based
2016-05-15 V3.9.0  Xbee BLEbee setup
2016-05-15 V3.9.1  Menu fixes.
2016-06-09 V3.9.2  Boot delay 3 seconds
2016-06-16 V3.9.3  Changed to 5 minutes count
2016-06-17 V3.9.4  Added Ethernet.maintain() to renew lease before restart
2016-07-04 V3.9.5  reset Wiz820IO modified at startup
2016-07-08 V3.9.6  move week hourly reset in main loop and 3g gim restart first shutdown 3gim module.
2016-07-16 V3.9.7  reset counter just before main loop to avoid wrong first count
2016-07-21 V3.9.8  random minute restart weekly restart
2016-07-27 V3.9.9  fix for DS18S20 to report correctly
2016-07-27 V4.0.0  adjusted display for Ethernet and 3G to display count down in 4 digits and get even spaces for the voltage and temperature.
2016-07-29 V4.0.1  3GIM and Wiz820IO modules reset at startup
2016-07-31 V4.0.2  setup weekly restarts
2016-08-01 V4.0.3  improved weekly restart based on rst value on sdcard
2016-08-05 V4.0.4  daily RTC update
2016-08-10 V4.0.5  moved 3Gpower down/up more sooner in startup. to allow 3GIM module to settle down before accessing it
2016-08-30 V4.0.6  fixed bug with pin assignment of Ethernet
2016-09-09 V4.0.7  fixed retries connect Ethernet
2016-09-09 V4.0.8  added delay between retries Ethernet connect
2016-09-14 V4.0.9  fixed in time sync provider
2016-09-17 V4.1.0  fixed weekly restarts (was not correctly working)
2016-09-17 V4.1.1  fixed Openlog startup detection (was broken in 4.0.9)
2016-09-19 V4.1.2  fixed compiler warnings
2016-09-20 V4.1.3  NTP server timeout retries at 5 minutes.
2016-09-21 V4.1.4  fixes freezes on time update
2016-09-22 V4.1.5  Cleaned up formatting
2016-09-23 V4.1.6  setup NTP check for 15 seconds retries and 300 seconds timeout
2016-09-23 V4.1.7  setup to renew a lease for DHCP every 5 minutes for networks that have short DHCP lease setup
2016-09-24 V4.1.8  conditional setup of the Ethernet.maintain() function to restart the Ethernet connection id a new lease or a rebind can not obtained.
2016-10-16 V4.1.9  Added confirmation for measurement ID from API to be sure data is accepted in the Database.
2016-10-28 V4.2.0  Status data to API.


contact rob@yr-design.biz
 */

/**************************************************************************/
// Init
/**************************************************************************/


#define ENABLE_DEBUG

#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <EEPROM.h>
#include <limits.h>
#include <SoftwareSerial.h>
#include <Time.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <i2c_t3.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <limits.h>
#include "a3gim.h"
#include "PointcastSetup.h"
#include "PointcastDebug.h"

//setup LCD I2C
#define I2C_ADDR    0x27  // Define I2C Address where the PCF8574 is for LCD2004 form http://www.sainsmart.com  the PCF8574A is 0x3f
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

int n = 1;
LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);
int green_ledPin = 31;
int red_ledPin = 26;

// 3G signal strengh
int rssi = -125;

//setup Power detection
#define VOLTAGE_PIN A13
#define VOLTAGE_R1 100000
#define VOLTAGE_R2 10000

//sliding windows counting variables
#define NX 60
#define TIME_INTERVAL 5000
#define IS_READY (interruptCounterAvailable())

//setup Onewire for temp sensor
OneWire  ds(32);  // on pin 10 of extra header(a 4.7K resistor is necessary)

//main menu variable
boolean finished_startup = false;
boolean network_startup = false;
boolean sdcard_startup = false;
boolean pingConnect = false;


#define LINE_SZ 128
// SENT_SZ is used for sending data for 3G
#define SENT_SZ 120
//OLINE_SZ is used for OpenLog buffers
#define OLINE_SZ 512
//GATEWAY_sz is array for gateways
#define GATEWAY_SZ 2


static char obuf[OLINE_SZ];
static char buf[LINE_SZ];
static char buf2[LINE_SZ];
static char strbuffer[32];
static char strbuffer1[32];
char body[512];
char body2[512];
char body3[512];
char STATS[255];

// OpenLog Settings --------------------------------------------------------------
SoftwareSerial OpenLog =  SoftwareSerial(0, 1);
SoftwareSerial XbeeSerial =  SoftwareSerial(25, 24);
static const int resetOpenLog = 3;
#define OPENLOG_RETRY 500
bool openlog_ready = false;
char logfile_name[13];  // placeholder for filename
bool logfile_ready = false;

static void setupOpenLog();
static void createFile(char *fileName);

static ConfigType config;
static DoseType dose;
PointcastSetup PointcastSetup(OpenLog, config, dose, obuf, OLINE_SZ);


//static
static char VERSION[] = "V4.2.0";

#if ENABLE_3G
static char path[LINE_SZ];
char datedisplay[8];
char coordinate[16];
#endif

#if ENABLE_ETHERNET
static char json_buf[SENT_SZ];
static char json_buf2[SENT_SZ];
bool ethernetfailed;
#endif


//Struct setup
typedef struct
{
  unsigned char state;
  unsigned char conn_fail_cnt;
} devctrl_t;
static devctrl_t ctrl;

//Const
const char *server = "107.161.164.163";
const char serverName[] = "safecast-production.s3.amazonaws.com";
char character;
const int port = 80;
const int interruptMode = FALLING;
int updateIntervalInMinutes = 1;
const char *header = "Content-Type:application/json$r$n";
const boolean useHTTPS = false;  // Use https(true) or http(false)
uint32_t seconds;

#if ENABLE_3G
const int PowerPin = 6;
#endif

#if ENABLE_ETHERNET
//ethernet
byte macAddress[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xE0, 0x5C };
EthernetClient client;
IPAddress localIP (192, 168, 100, 40);
char timeServer[] = "pool.ntp.org";
int resetPin = 9;   //
int ethernet_powerdonwPin = A2;
const int timeZone = 0;
boolean timeset_on = false;
EthernetUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets]
char macstr[19];
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
#endif

//Boolean
boolean getTimeStamp = true;

//Strings

String last_failure = "";
String replysub = "";
String replyserver = "";
String replyserver1 = "";

//int
int MAX_FAILED_CONNS = 5;
int len;
int len2;
int len4;
int conn_fail_cnt;
int NORMAL = 0;
int RESET = 1;
int S1peak;
int S2peak;
int failures;
int left_mins_old = 1;
int ntpcount;
int fail_cnt = 0;



//long
unsigned long elapsedTime(unsigned long startTime);
unsigned long previousMillis = 0;
unsigned long currentmillis = 0;
unsigned long total_time = 0;
long display_days = 0;
long display_hours = 0;
long display_mins = 0;
long display_secs = 0;

//long for sliding window
unsigned long shift_reg[NX] = {0};
unsigned long shift_reg2[NX] = {0};
unsigned long reg_index = 0;
unsigned long reg_index2 = 0;
unsigned long total_count = 0;
unsigned long max_count = 0;
unsigned long uptime = 0;
unsigned long _start_time;
unsigned long _delay = TIME_INTERVAL;
unsigned long _count = 0;
unsigned long cpm1 = 0, cpm2 = 0, cpb1 = 0, cpb2 = 0;

unsigned long cpm_gen1()
{
  unsigned int i;
  unsigned long c_p_m1 = 0;

  // sum up
  for (i = 0 ; i < NX ; i++)
    c_p_m1 += shift_reg[i];
  //deadtime compensation (medcom international)
  c_p_m1 = (unsigned long)((float)c_p_m1 / (1 - (((float)c_p_m1 * 1.8833e-6))) / 5);
  return c_p_m1;
}


unsigned long cpm_gen2()
{
  unsigned int i;
  unsigned long c_p_m2 = 0;

  // sum up
  for (i = 0 ; i < NX ; i++)
    c_p_m2 += shift_reg2[i];

  //deadtime compensation (medcom international)
  c_p_m2 = (unsigned long)((float)c_p_m2 / (1 - (((float)c_p_m2 * 1.8833e-6))) / 5);
  return c_p_m2;
}



// Interval is how long display  wait
int display_interval = 2000;

// Display time on display
bool displayTimeOn;

// Interval is how long LED blinks
int blinkinterval = 50;

// Char
char timestamp[19];
char lat[8];
char lon[9];
char lat_lon_nmea[25];
unsigned char state;

#if ENABLE_3G
char res[a3gsMAX_RESULT_LENGTH + 1];
char res2[a3gsMAX_RESULT_LENGTH + 1];
char res4[a3gsMAX_RESULT_LENGTH + 1];
const int timeZone = 1;
#endif

// Gateway setup
char gateway0[16] ;
char gateway1[16] ;
char *gateway[2];

// Joystick pins setup
const int JOY_A_PIN = 17;
const int JOY_B_PIN = 20;
const int JOY_C_PIN = 21;
const int JOY_D_PIN = 23;
const int JOY_E_PIN = 22;

volatile boolean joyCntA = false;
volatile boolean joyCntB = false;
volatile boolean joyCntC = false;
volatile boolean joyCntD = false;
volatile boolean joyCntE = false;


//WDT timer
IntervalTimer wdTimer;

//reset marco
#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

void onReset()
{
  CPU_RESTART;
}


// generate checksums for log format
byte len1, chk;
byte len3, chk2;
char checksum(char *s, int N)
{
  int i = 0;
  char chk = s[0];

  for (i = 1; i < N; i++)
    chk ^= s[i];

  return chk;
}

char checksum2(char *s, int N)
{
  int i = 0;
  char chk2 = s[0];

  for (i = 1; i < N; i++)
    chk2 ^= s[i];

  return chk2;
}

// Sampling interval (e.g. 60,000ms = 1min)
unsigned long updateIntervalInMillis = 0;

// The next time to feed
unsigned long nextExecuteMillis = 0;

// Event flag signals when a geiger event has occurred
volatile unsigned char eventFlag = 0;       // FIXME: Can we get rid of eventFlag and use counts>0?
unsigned long int counts_per_sample;
unsigned long int counts_per_sample2;

// The last connection time to disconnect from the server
// after uploaded feeds
long lastConnectionTime = 0;

// The conversion coefficient from cpm to µSv/h
float conversionCoefficient = 0;
float conversionCoefficient2 = 0;


//Pulse counters
void onPulse()
{
  counts_per_sample++;
}
void onPulse2()
{
  counts_per_sample2++;
}


//sliding windows setup
void interruptCounterReset()
{
  // set start time
  _start_time = millis();
  // set count to zero (optional)
  counts_per_sample = 0;
  counts_per_sample2 = 0;
}

int interruptCounterAvailable()
{
  // get current time
  unsigned long now = millis();
  // do basic check for millis overflow
  if (now >= _start_time)
    return (now - _start_time >= _delay);
  else
    return (ULONG_MAX + now - _start_time >= _delay);
}

// return current number of counts
unsigned long interruptCounterCount()
{
  return counts_per_sample;
}

// return current number of counts
unsigned long interruptCounterCount2()
{
  return counts_per_sample2;
}



//Joy Switch
void joyA_Callback()
{
  joyCntA = true;
}
void joyB_Callback()
{
  joyCntB = true;
}
void joyC_Callback()
{
  joyCntC = true;
}
void joyD_Callback()
{
  joyCntD = true;
}
void joyE_Callback()
{
  joyCntE = true;
}



/**************************************************************************/
// Setup
/**************************************************************************/

//  freeRam for Teensy 3.0
uint32_t FreeRam() {
  uint32_t stackTop;
  uint32_t heapTop;

  // current position of the stack.
  stackTop = (uint32_t) &stackTop;

  // current position of heap.
  void* hTop = malloc(1);
  heapTop = (uint32_t) hTop;
  free(hTop);

  // The difference is the free, available ram.
  return stackTop - heapTop;
}




void setup() {


//power down and up 3GIM module
#if ENABLE_3G
  delay(1000);
  pinMode(PowerPin, OUTPUT); digitalWrite(PowerPin, HIGH);
#ifdef ENABLE_DEBUG
  Serial.println("3GIM power down");
#endif
  delay(1000);
  digitalWrite(PowerPin, LOW);
#ifdef ENABLE_DEBUG
  Serial.println("3GIM power up");
#endif
  delay(3000);
#endif

  //power down and up Ethernet module
#if ENABLE_ETHERNET
  delay(1000);
  pinMode(ethernet_powerdonwPin, OUTPUT); digitalWrite(ethernet_powerdonwPin, HIGH);
#ifdef ENABLE_DEBUG
  Serial.println("Ethernet power down");
#endif
  delay(1000);
  digitalWrite(ethernet_powerdonwPin, LOW);
#ifdef ENABLE_DEBUG
  Serial.println("Ethernet power up");
#endif
  delay(1000);


  // reset Ethernet wiz820iIO
  pinMode(resetPin, OUTPUT); digitalWrite(resetPin, HIGH);
#ifdef ENABLE_DEBUG
  Serial.println("Ethernet reset");
#endif
  delay(1000);
  digitalWrite(resetPin, LOW);
#endif

  analogReference(INTERNAL);

  //Read dose from EEPROM
  EEPROM_readAnything(BMRDD_EEPROM_DOSE, dose);
  dose.restarts++;
  EEPROM_writeAnything(BMRDD_EEPROM_DOSE, dose);

  //print last reset message and setup the patting of the dog
  delay(100);
  printResetType();

  //start WDT
  wdTimer.begin(KickDog, 10000000); // patt the dog every 10sec

  // Load EEPROM settings
  PointcastSetup.initialize();

  // setup for trouble shooting
  updateIntervalInMinutes = config.trb ? 1 : 5;
  updateIntervalInMillis = updateIntervalInMinutes * 60 * 1000;

  //serial for Xbee
  XbeeSerial.begin(9600);

  //beep for loud piezo twice
  int i = 0;
  while (i < 2) {
    digitalWrite(28, HIGH);
    pinMode(28, OUTPUT);
    delay(250);
    pinMode(28, INPUT);
    delay(250);
    i++;
  }


  //button reset
  pinMode(27, INPUT_PULLUP);
  attachInterrupt(27, onReset, interruptMode);

  // set brightnes
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(125);

  //Joy Switch setup
  pinMode(JOY_A_PIN, INPUT_PULLUP);
  pinMode(JOY_B_PIN, INPUT_PULLUP);
  pinMode(JOY_C_PIN, INPUT_PULLUP);
  pinMode(JOY_D_PIN, INPUT_PULLUP);
  pinMode(JOY_E_PIN, INPUT_PULLUP);

  attachInterrupt(JOY_A_PIN, joyA_Callback, FALLING);
  attachInterrupt(JOY_B_PIN, joyB_Callback, FALLING);
  attachInterrupt(JOY_C_PIN, joyC_Callback, FALLING);
  attachInterrupt(JOY_D_PIN, joyD_Callback, FALLING);
  attachInterrupt(JOY_E_PIN, joyE_Callback, FALLING);



  //set up the LCD's number of columns and rows:
  lcd.begin(20, 4);

  //boot delay
  delay(3000);



  Menu_startup();
}

//End setup

/**************************************************************************/
// Start screen
/**************************************************************************/

void Menu_startup(void) {

  displayTimeOn = false;

  //setup failure message
#ifdef ENABLE_DEBUG
  Serial.print("last_failure =");
  Serial.println(config.last_failure);
  Serial.print("logs =");
  Serial.println(dose.logs);
  Serial.print("fails =");
  Serial.println(dose.fails);
  Serial.print("restarts =");
  Serial.println(dose.restarts);
  Serial.print("free ram = ");
  Serial.println(FreeRam());
#endif


  lcd.clear();
  // LED on delay (start speed display function by pressing down)
  previousMillis = millis();
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; lcd.clear(); display_interval = 3000; Menu_system(); return;
    }
    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; lcd.clear(); lcd.print("Clearing EEPROM"); eepromclear(); return;
    }

    // Print startup message to the LCD.
    lcd.setCursor(0, 0);
    lcd.print("SAFECAST POINTCASTv1");
    lcd.setCursor(0, 1);
    lcd.print("Firmware :");
    lcd.print(VERSION);
    lcd.setCursor(0, 2);
    lcd.print("Device ID:");
    lcd.print(config.devid);
    lcd.setCursor(0, 3);
    lcd.print("http://safecast.org");


    //LED1(green) setup
    pinMode(green_ledPin, OUTPUT);
    digitalWrite(green_ledPin, HIGH);
    digitalWrite(28, LOW);

    //LED2(red) setup
    pinMode(red_ledPin, OUTPUT);
    digitalWrite(red_ledPin, HIGH);

  }

  //LED off
  digitalWrite(red_ledPin, LOW);
  digitalWrite(green_ledPin, LOW);
  // Sound off
  digitalWrite(28, HIGH);

  Menu_system();

}

/**************************************************************************/
// System Screen
/**************************************************************************/


void Menu_system(void) {

  displayTimeOn = false;
  float battery = ((read_voltage(VOLTAGE_PIN)));
  float temperature = getTemp();

  // Print system message to the LCD.
  lcd.clear();
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {
    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; Menu_startup(); return;
    }
    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; lcd.clear(); display_interval = 3000; Menu_sdcard(); return;
    }
    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }


    lcd.setCursor(0, 0);
    lcd.print("System");
    lcd.setCursor(0, 1);
    lcd.print("Power:");
    if (battery < 4.4) {
      lcd.print("BAT");;
    } else {
      lcd.print("EXT");
    }
    lcd.setCursor(15, 1);
    lcd.print(battery);
    lcd.print("V");
    lcd.setCursor(0, 2);
    lcd.print("Bat:");
    int battery1 = ((battery - 2.9) * 100);
    if (battery1 < 0) battery1 = 1;
    if (battery1 > 100) battery1 = 100;
    sprintf_P(strbuffer, PSTR("%02d"), battery1);
    lcd.print(strbuffer);
    lcd.print("%");
    lcd.setCursor(11, 2);
    lcd.print("TNSY:");
    lcd.print("3.3");
    lcd.print("V");
    lcd.setCursor(0, 3);
    lcd.print("Tmp:");
    lcd.print(temperature);
    lcd.print("C");

  }

  Menu_sdcard();
}



/**************************************************************************/
// SDcard Screen
/**************************************************************************/

void Menu_sdcard(void) {

  displayTimeOn = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SDCARD ");

  //Openlog setup
  if (!sdcard_startup) {
    OpenLog.begin(9600);
    setupOpenLog();
  }
  if (!openlog_ready) {
    lcd.setCursor(8, 0);
    lcd.print("FAIL");
    // failure message to EEPROM
    String last_failure = "FAILsdcard";
    last_failure.toCharArray(config.last_failure, 12);
    EEPROM_writeAnything(BMRDD_EEPROM_SETUP, config);
    //end test
#ifdef ENABLE_DEBUG
    Serial.print(config.last_failure);
#endif
    delay(2000);
    //Red LED on
    digitalWrite(26, HIGH);
#ifdef ENABLE_DEBUG
    Serial.println();
    Serial.println("No SD card.. ");
#endif
  }
  if (openlog_ready) {
    previousMillis = millis() ;
    while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

      if (joyCntB) {
#ifdef ENABLE_DEBUG
        Serial.println ("Up");
#endif
        joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; Menu_system(); return;
      }

      if (joyCntA) {
#ifdef ENABLE_DEBUG
        Serial.println ("Down");
#endif
        joyCntA = !joyCntA; joyCntB = false; lcd.clear(); display_interval = 3000; Menu_network(); return;
      }

      if (joyCntE) {
#ifdef ENABLE_DEBUG
        Serial.println ("Enter");
#endif
        joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
      }
      if (!sdcard_startup) {
        lcd.setCursor(8, 0);
        lcd.print(" PASS");
        lcd.setCursor(0, 1);
        lcd.print("PNTCAST:");
#ifdef ENABLE_DEBUG
        Serial.println();
        Serial.println("loading setup");
#endif
        PointcastSetup.loadFromFile("PNTCAST.TXT");
        lcd.print(" PASS");
        lcd.setCursor(0, 2);
        lcd.print("SENSORS:");
#ifdef ENABLE_DEBUG
        Serial.println();
        Serial.println("loading sensors setup");
#endif
        PointcastSetup.loadFromFile("SENSORS.TXT");
        lcd.print(" PASS");
        lcd.setCursor(0, 3);
        lcd.print("NETWORK:");
#ifdef ENABLE_DEBUG
        Serial.println();
        Serial.println("loading Network setup");
#endif
        PointcastSetup.loadFromFile("NETWORKS.TXT");
        lcd.print(" PASS");

        sdcard_startup = true;

      }
      // sdcard display status
      lcd.setCursor(8, 0);
      lcd.print(" PASS");
      lcd.setCursor(0, 1);
      lcd.print("PNTCAST:");
      lcd.print(" PASS");
      lcd.setCursor(0, 2);
      lcd.print("SENSORS:");
      lcd.print(" PASS");
      lcd.setCursor(0, 3);
      lcd.print("NETWORK:");
      lcd.print(" PASS");
      delay(3000);

    }
  }
  // SENSOR 1 setup
  if (config.sensor1_enabled) {
    conversionCoefficient = 1 / config.sensor1_cpm_factor;
    pinMode(14, INPUT_PULLUP);
    attachInterrupt(14, onPulse, interruptMode);
  }

  // SENSOR 2 setup
  if (config.sensor2_enabled) {
    conversionCoefficient2 = 1 / config.sensor2_cpm_factor;
    pinMode(15, INPUT_PULLUP);
    attachInterrupt(15, onPulse2, interruptMode);
  }

  Menu_network();
}


/**************************************************************************/
// Network Screen
/**************************************************************************/
void Menu_network(void) {

  displayTimeOn = false;

#if ENABLE_ETHERNET
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NETWORK ETHER (DHCP)");
  //setup time
  if (!network_startup) {
    lcd.setCursor(0, 1);
    lcd.print("setting up Ethernet.");
#ifdef ENABLE_DEBUG
    Serial.print("setting up Ethernet");
#endif
    sscanf(config.macid, "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx", &macAddress[0], &macAddress[1], &macAddress[2], &macAddress[3], &macAddress[4], &macAddress[5]);
#ifdef ENABLE_DEBUG
    Serial.print("ID");
    for (int i = 0; i < 6; ++i)
    {
      Serial.print(":");
      Serial.print(macAddress[i], HEX);
    }

    Serial.println();
#endif

    //setup time

    if (Ethernet.begin(macAddress) == 0) {
      {
        Ethernet.begin(macAddress, localIP);
      }
    }
    Udp.begin(localPort);
    setSyncProvider(getNtpTime);
    Teensy3Clock.set(now());

    // weekly restarts based on days
    if (config.rst > 0) {
      unsigned long pointcast_reboot = config.rst * 60 * 60 * 24;
      Alarm.alarmRepeat(pointcast_reboot, WeeklyRestart);
#ifdef ENABLE_DEBUG
      Serial.print("Reboots every ");
      Serial.print(config.rst);
      Serial.println(" days");
#endif
    } else {
      //weekly restarts
      Alarm.alarmRepeat(604800L, WeeklyRestart);
#ifdef ENABLE_DEBUG
      Serial.print("Reboots every ");
      Serial.print("7");
      Serial.println(" days");
#endif
    }
    // Daily RTC 60*60*24 seconds
    setSyncInterval(86400L);
  }


#endif


#if ENABLE_ETHERNET

  // random gateway array setup
  if (!network_startup) {
    gateway[0] = config.gw1;
    gateway[1] = config.gw2;
    randomSeed(analogRead(0));
    int gatewaynumber = random(GATEWAY_SZ);
    server = gateway[gatewaynumber];
#ifdef ENABLE_DEBUG
    Serial.print("Random gateway setup for ");
    sprintf_P(strbuffer, PSTR("%s"), server);
    Serial.println(strbuffer);
    Serial.print("LocalIP =");
    Serial.println(Ethernet.localIP());
#endif
  }

  // Initiate a DHCP session
  if (!network_startup) {

    if (Ethernet.begin(macAddress) == 0)
    {
#ifdef ENABLE_DEBUG
      Serial.println("Failed DHCP");
#endif
      ethernetfailed = true;
      Ethernet.begin(macAddress, localIP);

      //alarm peep
      digitalWrite(28, HIGH);
      pinMode(28, OUTPUT);
      delay(250);
      pinMode(28, INPUT);
      //switch on fail LED
      digitalWrite(26, HIGH);
    }


    // ping https://safecast-production.s3.amazonaws.com/uploads/bgeigie_import/source

    if (client.connect(serverName, 80)) {
      Serial.println("connected");
      client.println("GET  https://safecast-production.s3.amazonaws.com/uploads/bgeigie_import/source");
      client.println();
    }
    else {
      Serial.println("connection failed");
    }

    while (client.connected() && !client.available()) delay(10); //waits for data
    while (client.connected() || client.available()) { //connected or data available
      character = client.read();
      replyserver.concat(character);
    }

    if (replyserver.indexOf("HTTP/1.1 505 HTTP Version not supported") == 0) //checks for xml
    {
      Serial.println("S3 amazon connected comfirmed");
      pingConnect = true;

    } else {
      Serial.println("NC");
      pingConnect = false;
      digitalWrite(26, HIGH);
    }
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
    }

    client.stop();

  }

  //end ping S3

  network_startup = true;
  previousMillis = millis() ;
  lcd.clear();
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; Menu_sdcard(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; lcd.clear(); display_interval = 3000; Menu_network_test(); return;
    }

    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }

    lcd.setCursor(0, 0);
    lcd.print("NETWORK ETHER (DHCP)");
    lcd.setCursor(0, 1);
    if (!ethernetfailed) {
      lcd.print("IP:");
      lcd.print(Ethernet.localIP());
      lcd.setCursor(0, 2);
      lcd.print("GW:");
      lcd.print(server);
      lcd.setCursor(0, 3);
      lcd.print("ID:");
      snprintf(macstr, 18, "%02x:%02x:%02x:%02x:%02x:%02x", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
      lcd.print(macstr);
    } else {
      lcd.print("No LAN");
    }
  }
  if (!network_startup) {
#ifdef ENABLE_DEBUG
    Serial.print("MACid stored:");
    Serial.println(macstr);
#endif
  }
  // getTimeStamp=false;
#ifdef ENABLE_DEBUG
  Serial.print("RTC timestamp flag is set to ");
  Serial.println(getTimeStamp);
#endif


#endif // endif of Ethernet setup


#if ENABLE_3G

  //Get RSSI level
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NETWORK 3G startup");
  lcd.setCursor(0, 1);
  lcd.print("Connecting.....");

  if (!network_startup) {
    if (a3gs.start() == 0 && a3gs.begin() == 0) {
      a3gs.getRSSI(rssi);
#ifdef ENABLE_DEBUG
      Serial.print("RSSI = ");
      Serial.print(rssi);
      Serial.println(" dBm");
#endif
    } else {
      lcd.setCursor(0, 1);
      lcd.print("FAIL                ");
      String last_failure = "3G no start";
      // failure message to EEPROM
      last_failure.toCharArray(config.last_failure, 12);
      EEPROM_writeAnything(BMRDD_EEPROM_SETUP, config);
      //end message
      //alarm peep
      digitalWrite(28, HIGH);
      pinMode(28, OUTPUT);
      delay(250);
      pinMode(28, INPUT);
      //switch on fail LED
      digitalWrite(26, HIGH);
    }

    if (a3gs.getTime2(seconds) == 0) {
      setTime(seconds);
      adjustTime(-32400);
      Teensy3Clock.set(now());

      // weekly restarts based on days
      if (config.rst > 0) {
        unsigned long pointcast_reboot = config.rst * 60 * 60 * 24;
        Alarm.alarmRepeat(pointcast_reboot, WeeklyRestart);
#ifdef ENABLE_DEBUG
        Serial.print("Reboots every ");
        Serial.print(config.rst);
        Serial.println(" days");
#endif
      } else {
        //weekly restarts
        Alarm.alarmRepeat(604800L, WeeklyRestart);
#ifdef ENABLE_DEBUG
        Serial.print("Reboots every ");
        Serial.print("7");
        Serial.println(" days");
#endif
      }
      // Daily RTC
      Alarm.alarmRepeat(12, 30, 0, DailyRTC);
#ifdef ENABLE_DEBUG
      Serial.println("Daily RTC setup");
#endif

#ifdef ENABLE_DEBUG
      Serial.print(seconds);
      Serial.println(" Sec.");
#endif
    }
    else {
#ifdef ENABLE_DEBUG
      Serial.println("Can't get seconds.");
      String last_failure = "no EPOCH";
      // failure message to EEPROM
      last_failure.toCharArray(config.last_failure, 12);
      EEPROM_writeAnything(BMRDD_EEPROM_SETUP, config);
      //end message
#endif
    }


    // random gateway array setup
    gateway[0] = config.gw1;
    gateway[1] = config.gw2;
    randomSeed(analogRead(0));
    int gatewaynumber = random(GATEWAY_SZ);
    server = gateway[gatewaynumber];
#ifdef ENABLE_DEBUG
    Serial.print("Random gateway setup for ");
    sprintf_P(strbuffer, PSTR("%s"), server);
    Serial.println(strbuffer);
#endif
  }

  lcd.clear();
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; Menu_sdcard(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; lcd.clear(); display_interval = 3000; Menu_network_test(); return;
    }

    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }

    lcd.setCursor(0, 0);
    lcd.print("NETWORK 3G");
    lcd.setCursor(0, 1);
    lcd.print("Signal:");
    lcd.print(rssi);
    lcd.print(" dBm");
    lcd.setCursor(0, 2);
    lcd.print("Carrier:");
    lcd.print(config.apn);
    lcd.setCursor(0, 3);
    lcd.print("Phone: ");
    lcd.print(config.tel);
  }
  network_startup = true;
#endif

  Menu_network_test();
}

/**************************************************************************/
// Network Screen Test
/**************************************************************************/
void Menu_network_test(void) {

  displayTimeOn = false;
#if ENABLE_ETHERNET
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NETWORK TEST ETHER");


  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; joyCntD = false; lcd.clear(); display_interval = 3000; Menu_network(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; joyCntD = false; lcd.clear(); display_interval = 3000; Menu_Ping(); return;
    }

    if (joyCntD) {
#ifdef ENABLE_DEBUG
      Serial.println ("Right");
#endif
      joyCntD = !joyCntD; joyCntA = false; joyCntB = false; lcd.clear(); display_interval = 5000; Menu_Ping(); return;
    }


    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }

    lcd.setCursor(0, 0);
    lcd.print("NETWORK TEST ETHER");
    lcd.setCursor(0, 1);
    lcd.print("DEVICE:");
    sprintf_P(strbuffer, PSTR("%s"), (ethernetfailed ? "FAILED" : "PASS"));
    lcd.print(strbuffer);
    lcd.setCursor(0, 2);
    lcd.print("GW:");
    lcd.print(server);
    lcd.setCursor(0, 3);
    lcd.print("PING:");
    sprintf_P(strbuffer, PSTR("%s"), (pingConnect ? "PASS                " : "FAILED              " ));
    lcd.print(strbuffer);
  }
  if (!network_startup) {
    Serial.print("MACid stored:");
    Serial.println(macstr);
  }
#endif


#if ENABLE_3G
  lcd.setCursor(0, 0);
  lcd.print("NETWORK 3G TEST");
  lcd.setCursor(0, 1);
  lcd.print("Signal:");
  lcd.print(rssi);
  lcd.print(" dBm");
  lcd.setCursor(0, 2);
  lcd.print("Carrier:");
  lcd.print(config.apn);
  lcd.setCursor(0, 3);
  lcd.print("Ping :");
  lcd.print("pinging.....");

  len = sizeof(res);
  sprintf_P(strbuffer, PSTR("%s"), serverName);
  Serial.println(strbuffer);
  if (a3gs.httpGET(strbuffer, port, path, res, len) == 0) {
    replyserver.concat(res);

    if (replyserver.indexOf("<?xml version") == 0) //checks for gateway for html message of gateway
    {
#ifdef ENABLE_DEBUG
      Serial.println("S3 amazon connect confirmed");
#endif
      pingConnect = true;
    } else {
#ifdef ENABLE_DEBUG
      Serial.println("NC");
#endif
      pingConnect = false;

      //switch on fail LED
      digitalWrite(26, HIGH);
    }
  }
  else {
#ifdef ENABLE_DEBUG
    Serial.print("Can't get HTTP response from ");
    Serial.println(server);
#endif
    //switch on fail LED
    digitalWrite(26, HIGH);
  }

  lcd.clear();
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; joyCntD = false; lcd.clear(); display_interval = 3000; Menu_network(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; joyCntD = false; lcd.clear(); display_interval = 3000; Menu_time(); return;
    }

    if (joyCntD) {
#ifdef ENABLE_DEBUG
      Serial.println ("Right");
#endif
      joyCntD = !joyCntD; joyCntA = false; joyCntB = false; lcd.clear(); display_interval = 5000; Menu_Ping(); return;
    }


    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }

    lcd.setCursor(0, 0);
    lcd.print("NETWORK 3G TEST");
    lcd.setCursor(0, 1);
    lcd.print("Signal:");
    lcd.print(rssi);
    lcd.print(" dBm");
    lcd.setCursor(0, 2);
    lcd.print("Carrier:");
    lcd.print(config.apn);
    lcd.setCursor(0, 3);
    lcd.print("Ping: ");
    sprintf_P(strbuffer, PSTR("%s"), (pingConnect ? "PASS                " : "FAILED              " ));
    lcd.print(strbuffer);

  }
  network_startup = true;
#endif


  Menu_time();
}




/**************************************************************************/
// Ping Screen
/**************************************************************************/

void Menu_Ping(void) {
  displayTimeOn = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ping Test");
  lcd.setCursor(0, 1);
  lcd.print("Pinging api....");

#if ENABLE_3G
  len = sizeof(res);
  sprintf_P(strbuffer, PSTR("%s"), serverName);
  Serial.println(strbuffer);
  if (a3gs.httpGET(strbuffer, port, path, res, len) == 0) {
    replyserver.concat(res);

    if (replyserver.indexOf("<?xml version") == 0) //checks for gateway for html message of gateway
    {
#ifdef ENABLE_DEBUG
      Serial.println("S3 amazonaws connect confirmed");
#endif
      pingConnect = true;
    } else {
#ifdef ENABLE_DEBUG
      Serial.println("NC");
#endif
      pingConnect = false;
      //switch on fail LED
      digitalWrite(26, HIGH);

    }
  }
  else {
#ifdef ENABLE_DEBUG
    Serial.print("Can't get HTTP response from ");
    Serial.println(server);
#endif
    //switch on fail LED
    digitalWrite(26, HIGH);
  }
#endif //ENABLE_3G


#if ENABLE_ETHERNET
  if (client.connected())
  {
#ifdef ENABLE_DEBUG
    Serial.println("Disconnecting");
#endif
    client.stop();
  }

  // ping https://safecast-production.s3.amazonaws.com/uploads/bgeigie_import/source

  if (client.connect(serverName, 80)) {
    Serial.println("connected");
    client.println("GET https://safecast-production.s3.amazonaws.com/uploads/bgeigie_import/source");
    client.println();
  }
  else {
    Serial.println("connection failed");
  }

  while (client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available()) { //connected or data available
    character = client.read();
    replyserver.concat(character);
  }

  if (replyserver.indexOf("HTTP/1.1 505 HTTP Version not supported") == 0) //checks for http 301
  {
    Serial.println("S3 amazon connected comfirmed");
    pingConnect = true;

  } else {
    Serial.println("NC");
    pingConnect = false;
    digitalWrite(26, HIGH);
  }
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
  }

  client.stop();

  //end ping api
#endif



  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; joyCntD = false; lcd.clear(); display_interval = 3000; Menu_network_test(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; joyCntD = false; lcd.clear(); display_interval = 3000; Menu_time(); return;
    }


    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }

    lcd.setCursor(0, 0);
    lcd.print("Ping Test ");
    lcd.setCursor(0, 2);
    sprintf_P(strbuffer, PSTR("%s"), (pingConnect ? "PASS                " : "FAILED              " ));
    lcd.print(strbuffer);
  }

  Menu_network_test();

}
/**************************************************************************/
// Time Screen
/**************************************************************************/
void Menu_time(void) {

  displayTimeOn = false;
  setSyncProvider(getTeensy3Time);


  if (timeStatus() != timeSet) {
#ifdef ENABLE_DEBUG
    Serial.println("Unable to sync with the RTC");
#endif
    sprintf_P(logfile_name, PSTR("%04d1234.TXT"), config.user_id % 10000);
  } else {
#ifdef ENABLE_DEBUG
    Serial.println("RTC has set the system time for UTC");
#endif
    sprintf_P(logfile_name, PSTR("%04d%02d%02d.TXT"), year(), month(), day());
  }



  //display time

  lcd.clear();
  previousMillis = millis();
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; Menu_network_test(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; lcd.clear(); display_interval = 3000; Menu_RTC(); return;
    }

    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }

    lcd.setCursor(0, 0);
    lcd.print("TIME (UTC)");
    lcd.setCursor(0, 1);
    lcd.print("Date:");
    lcd.print(month());
    lcd.print("-");
    lcd.print(day());
    lcd.setCursor(0, 2);
    lcd.print("Time:");
    printDigits(hour());
    lcd.print(":");
    printDigits(minute());
    lcd.setCursor(0, 3);
    lcd.print("Zone:");
    lcd.print(config.tz);
  }

  //dev
  if (config.dev) {
#ifdef ENABLE_DEBUG
    Serial.println ("setup for sending to dev.safecast.org");
#endif
  }
  sdcard_startup = true;


  // create SDcard filename
  if (!finished_startup) {
    if (openlog_ready) {
      logfile_ready = true;
      createFile(logfile_name);
      sprintf_P(strbuffer, PSTR(LOGFILE_HEADER));
      OpenLog.print(strbuffer);
      DEBUG_PRINT(strbuffer);
      sprintf_P(strbuffer, PSTR(VERSION));
      DEBUG_PRINTLN(strbuffer);
      OpenLog.print(strbuffer);
      //added extra info
      sprintf_P(strbuffer, PSTR("# Logs=%ld Fails=%ld Restarts=%ld"),  \
                dose.logs, \
                dose.fails, \
                dose.restarts);
      OpenLog.print(strbuffer);
      DEBUG_PRINTLN(strbuffer);
      sprintf_P(strbuffer, PSTR("# Last Failure=%s \n"), \
                config.last_failure);
      DEBUG_PRINTLN(strbuffer);
      OpenLog.print(strbuffer);

    }
  }



  Menu_RTC();
}


/**************************************************************************/
// RTC Screen
/**************************************************************************/
void Menu_RTC(void) {
  displayTimeOn = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RTC checking ...");
  lcd.setCursor(0, 1);
  lcd.print("please wait");
  delay(1000);


  //display time

  lcd.clear();
  previousMillis = millis();
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; joyCntD = false; lcd.clear(); display_interval = 3000; Menu_time(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; joyCntD = false; lcd.clear(); display_interval = 3000; Menu_pointcast1(); return;
    }

    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }


    if (!timeStatus() != timeSet) {
      lcd.setCursor(0, 0);
      lcd.print("RTC ");
      lcd.setCursor(0, 1);
      lcd.print("NTP server");
      lcd.setCursor(0, 2);
#if ENABLE_ETHERNET
      lcd.print(timeServer);
#endif
#if ENABLE_3G
      lcd.print("3G module");
#endif
      lcd.setCursor(0, 3);
      lcd.print("Time:");
      printDigits(hour());
      lcd.print(":");
      printDigits(minute());
    } else {
      lcd.setCursor(0, 0);
      lcd.print("RTC ");
      lcd.setCursor(0, 1);
      lcd.print("Not set");
    }

  }


  Menu_pointcast1();
}



/**************************************************************************/
// POINTCAST1 Screen
/**************************************************************************/
void Menu_pointcast1(void) {

  //display information
  displayTimeOn = false;
  lcd.clear();
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; Menu_RTC(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; lcd.clear(); display_interval = 3000; Menu_pointcast2(); return;
    }

    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }

    lcd.setCursor(0, 0);
    lcd.print("POINTCAST SETUP");
    lcd.setCursor(0, 1);
    lcd.print("DeviceID:");
    lcd.print(config.devid);
    lcd.setCursor(0, 2);
    lcd.print("TIMEZONE:");
    lcd.print(config.tz);
    lcd.setCursor(0, 3);
    lcd.print("ALARM-S1:");
    lcd.print(config.alm);
    lcd.print("CPM");
  }


  Menu_pointcast2();
}


/**************************************************************************/
// POINTCAST2 Screen
/**************************************************************************/
void Menu_pointcast2(void) {
  displayTimeOn = false;
  lcd.clear();
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; Menu_pointcast1(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; lcd.clear(); display_interval = 3000; Menu_gps(); return;
    }

    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }

    lcd.setCursor(0, 0);
    lcd.print("UPLOAD MODE");
    lcd.setCursor(0, 1);
    lcd.print("Adaptive: OFF");
    lcd.setCursor(0, 2);
    lcd.print("Integr Win: 300sec");
    lcd.setCursor(0, 3);
    lcd.print("Upload Win: 300sec");
  }

  Menu_gps();
}

/**************************************************************************/
// GPS Screen
/**************************************************************************/
void Menu_gps(void) {
  displayTimeOn = false;
  lcd.clear();
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; Menu_pointcast2(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; lcd.clear(); display_interval = 3000; Menu_sensors(); return;
    }

    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }

    lcd.setCursor(0, 0);
    lcd.print("GPS LOCATION");
    lcd.setCursor(0, 1);
    lcd.print("Lon:");
    lcd.print(config.longitude);
    lcd.setCursor(0, 2);
    lcd.print("Lat:");
    lcd.print(config.latitude);
    lcd.setCursor(0, 3);
    lcd.print("Alt:");
    lcd.print(config.alt);
    lcd.print("m");
  }

  //serial print
#ifdef ENABLE_DEBUG
  Serial.print("Lon:");
  Serial.println(config.longitude);
  Serial.print("Lat:");
  Serial.println(config.latitude);
  Serial.print("Alt:");
  Serial.println(config.alt);
#endif
  Menu_sensors();
}

/**************************************************************************/
// Sensors Screen
/**************************************************************************/
void Menu_sensors(void) {
  displayTimeOn = false;
  lcd.clear();
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; Menu_gps(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; lcd.clear(); display_interval = 3000; Menu_sensors_tests(); return;
    }

    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }

    lcd.setCursor(0, 0);
    lcd.print("SENSORS ");
    if (config.sensor1_enabled & config.sensor2_enabled ) {
      lcd.print("DUAL");;
    } else {
      lcd.print("SINGLE");
    }
    lcd.setCursor(0, 1);
    lcd.print("S1=");
    lcd.print(int(config.sensor1_cpm_factor));
    lcd.print(" CPM/uSv ");
    lcd.print(config.s1i);
    lcd.setCursor(0, 2);
    lcd.print("S2=");
    lcd.print(int(config.sensor2_cpm_factor));
    lcd.print(" CPM/uSv ");
    lcd.print(config.s2i);
    lcd.setCursor(0, 3);
    lcd.print("AUX=NC");
  }

  Menu_sensors_tests();
}

/**************************************************************************/
// Sensors Test Screen
/**************************************************************************/
void Menu_sensors_tests(void) {
  displayTimeOn = false;
  lcd.clear();
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; Menu_sensors(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; lcd.clear(); display_interval = 3000; Menu_api(); return;
    }


    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }

    lcd.setCursor(0, 0);
    lcd.print("SENSOR TEST");
    lcd.setCursor(0, 1);
    if (config.sensor1_enabled ) {
      if (counts_per_sample < 1 ) {
        lcd.print("S1=FAIL");
        //Red LED on
        digitalWrite(26, HIGH);
      } else {
        lcd.print("S1=PASS");
      }
    } else {
      lcd.print("S1=NC");
    }
    lcd.setCursor(0, 2);
    if (config.sensor1_enabled ) {
      if (counts_per_sample2 < 1 ) {
        lcd.print("S2=FAIL");
        //Red LED on
        digitalWrite(26, HIGH);
      } else {
        lcd.print("S2=PASS");
      }
    } else {
      lcd.print("S2=NC");
    }
    lcd.setCursor(0, 3);
    lcd.print("AUX=NC");
  }

  Menu_api();
}

/**************************************************************************/
// API Screen
/**************************************************************************/

void Menu_api(void) {
  displayTimeOn = false;
  lcd.clear();
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; Menu_sensors_tests(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; lcd.clear(); display_interval = 3000; Menu_datalogger(); return;
    }

    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }

    lcd.setCursor(0, 0);
    lcd.print("API");
    lcd.setCursor(5, 0);
    if (config.dev) {
      lcd.print("DEVELOPMENT");
    } else {
      lcd.print("PRODUCTION");
    }
    lcd.setCursor(0, 1);
    lcd.print("S1-ID=");
    lcd.print(config.user_id);
    lcd.setCursor(0, 2);
    lcd.print("S2-ID=");
    lcd.print(config.user_id2);
    lcd.setCursor(0, 3);
    //lcd.print("AK=");
//              strncpy( strbuffer, config.api_key, 12);
    lcd.print(config.api_key);
  }

  Menu_datalogger();
}

/**************************************************************************/
// Datalogger Screen
/**************************************************************************/

void Menu_datalogger(void) {
  displayTimeOn = false;
  lcd.clear();
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; network_startup = true; Menu_api(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; lcd.clear(); display_interval = 3000; Menu_counting(); return;
    }

    if (joyCntE) {
#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntB = false; joyCntA = false; display_interval = 100000;
    }

    lcd.setCursor(0, 0);
    lcd.print("DATA LOGGER");
    lcd.setCursor(0, 1);
    lcd.print(logfile_name);
    lcd.print(":");
    lcd.setCursor(0, 2);
    lcd.print("XXXX MB Free");
  }

  Menu_counting();
}

/**************************************************************************/
// Counting Screen
/**************************************************************************/
void Menu_counting(void) {

  //start at 0
  if (!finished_startup) {
    lastConnectionTime = millis();
  }

  //reset counter
  interruptCounterReset();

  // read temperature
  float temperature = getTemp();
  char temperature_string[5];
  dtostrf(temperature, 0, 1, temperature_string);

  // read battery
  float battery = ((read_voltage(VOLTAGE_PIN)));
  char battery_string[5];
  dtostrf(battery, 0, 2, battery_string);

  displayTimeOn = true;

  lcd.clear();
  previousMillis = millis() ;

  display_interval = 1000;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {
    unsigned long now1 = millis();
    nextExecuteMillis = now1 + updateIntervalInMillis;

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; Menu_datalogger(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; display_interval = 500; loop(); return;
    }
    if (joyCntD) {
#ifdef ENABLE_DEBUG
      Serial.println ("Right");
#endif
      joyCntD = !joyCntD; joyCntA = false; joyCntB = false; lcd.clear(); display_interval = 3000; Menu_term(); return;
    }

    lcd.setCursor(0, 0);
    lcd.print("S1:");
    lcd.print("-");
    lcd.print(" CPM ");
    lcd.print("-");
    lcd.print("uSh");
    lcd.setCursor(0, 1);
    lcd.print("S2:");
    lcd.print("-");
    lcd.print(" CPM ");
    lcd.print("-");
    lcd.print("uSh");
    lcd.setCursor(0, 2);
    lcd.print(config.dev ? "DEV:" : "API:");
    lcd.setCursor(4, 2);
    printDigits(hour());
    lcd.print(":");
    printDigits(minute());
    lcd.print("UTC");
    lcd.setCursor(13, 2);
    lcd.print("STARTUP");
    lcd.setCursor(0, 3);
    lcd.print("STS:");
    lcd.setCursor(4, 3);
    lcd.print(temperature_string);
    lcd.print("C");
    lcd.setCursor(10, 3);
    lcd.print(battery_string);
    lcd.print("V");

  }

}

/**************************************************************************/
// Degrees to NMEA format
/**************************************************************************/

void deg2nmae(char *lat, char *lon, char *lat_lon_nmea)
{

  double lat_f = strtod(lat, NULL);
  double lon_f = strtod(lon, NULL);

  char NS = (lat_f >= 0) ? 'N' : 'S';
  lat_f = (lat_f >= 0) ? lat_f : -lat_f;
  int lat_d = (int)fabs(lat_f);
  double lat_min = (lat_f - lat_d) * 60.;

  char lat_min_str[9];
  dtostrf(lat_min, 2, 4, lat_min_str);

  char EW = (lon_f >= 0) ? 'E' : 'W';
  lon_f = (lon_f >= 0) ? lon_f : -lon_f;
  int lon_d = (int)fabs(lon_f);
  double lon_min = (lon_f - lon_d) * 60.;

  char lon_min_str[9];
  dtostrf(lon_min, 2, 4, lon_min_str);

  snprintf(lat_lon_nmea, 25, "%02d%s,%c,%03d%s,%c", lat_d, lat_min_str, NS, lon_d, lon_min_str, EW);

}
/**************************************************************************/
// display digits
/**************************************************************************/

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  if (digits < 10)
    lcd.print('0');
  lcd.print(digits);
}
void printDigitsSingle(int digits) {
  // utility function for digital clock display: prints preceding colon and leading space
  if (digits < 10)
    lcd.print(' ');
  lcd.print(digits);
}


void printDigitsSerial(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  if (digits < 10)
#ifdef ENABLE_DEBUG
    Serial.print('0');
  Serial.print(digits);
#endif
  ;
}



/**************************************************************************/
//Sending to server
/**************************************************************************/
void SendDataToServer(float CPM, float CPM2) {

//setup ID for temperature
  int tempID = config.user_id + 8;

//setup alarm

  if (CPM > config.alm) {
    //beep for loud piezo 10 times
    int i = 0;
    while (i < 10) {
      digitalWrite(28, HIGH);
      pinMode(28, OUTPUT);
      delay(250);
      pinMode(28, INPUT);
      delay(250);
      i++;
    }
  }

  // display data
  displayTimeOn = true;


#if ENABLE_ETHERNET


// Convert from cpm to µSv/h with the pre-defined coefficient

  conversionCoefficient = 1 / config.sensor1_cpm_factor;
  float uSv = CPM * conversionCoefficient;                   // convert CPM to Micro Sievers Per Hour
  char CPM_string[16];
  dtostrf(CPM, 0, 0, CPM_string);
  if (CPM > (int)(config.S1peak)) {
    config.S1peak = CPM;
#ifdef ENABLE_DEBUG
    Serial.println ("Updating S1peak");
#endif
  }
#ifdef ENABLE_DEBUG
  Serial.print ("S1peak =");
  Serial.println (config.S1peak);
#endif
  conversionCoefficient2 = 1 / config.sensor2_cpm_factor;
  float uSv2 = CPM2 * conversionCoefficient2;                // convert CPM to Micro Sievers Per Hour
  char CPM2_string[16];
  dtostrf(CPM2, 0, 0, CPM2_string);
  if (CPM2 > (int)(config.S2peak)) {
    config.S2peak = CPM2;
#ifdef ENABLE_DEBUG
    Serial.println ("Updating S2peak");
#endif
  }
#ifdef ENABLE_DEBUG
  Serial.print ("S2peak =");
  Serial.println (config.S2peak);
#endif


  //Get temp and Battery
  float temperature = getTemp();
  char temperature_string[5];
  dtostrf(temperature, 0, 1, temperature_string);

  float battery = ((read_voltage(VOLTAGE_PIN)));
  char battery_string[5];
  dtostrf(battery, 0, 2, battery_string);

  //display geiger info
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S1:");
  if (CPM >= 1000) {
    dtostrf((float)(CPM / 1000.0), 4, 3, strbuffer);
    strncpy (strbuffer1, strbuffer, 4);
    if (strbuffer1[strlen(strbuffer1) - 1] == '.') {
      strbuffer1[strlen(strbuffer1) - 1] = 0;
    }
    lcd.print(strbuffer1);
    sprintf_P(strbuffer, PSTR("kCPM "));
    lcd.print(strbuffer);
  } else {
    dtostrf((float)CPM, 0, 0, strbuffer);
    lcd.print(strbuffer);
    sprintf_P(strbuffer, PSTR(" CPM "));
    lcd.print(strbuffer);
  }
  lcd.print(uSv);
  lcd.print("uSh");
  lcd.setCursor(0, 1);
  lcd.print("S2:");
  if (CPM2 >= 1000) {
    dtostrf((float)(CPM2 / 1000.0), 4, 3, strbuffer);
    strncpy (strbuffer1, strbuffer, 4);
    if (strbuffer1[strlen(strbuffer1) - 1] == '.') {
      strbuffer1[strlen(strbuffer1) - 1] = 0;
    }
    lcd.print(strbuffer1);
    sprintf_P(strbuffer, PSTR("kCPM "));
    lcd.print(strbuffer);
  } else {
    dtostrf((float)CPM2, 0, 0, strbuffer);
    lcd.print(strbuffer);
    sprintf_P(strbuffer, PSTR(" CPM "));
    lcd.print(strbuffer);
  }
  lcd.print(uSv2);
  lcd.print("uSh");
  lcd.setCursor(0, 2);
  lcd.print(config.dev ? "DEV:" : "API:");


// convert degree to NMAE
  deg2nmae (config.latitude, config.longitude, lat_lon_nmea);


//convert time in correct format
  sprintf_P(timestamp, PSTR("%02d-%02d-%02dT%02d:%02d:%02dZ"),  \
            year(), month(), day(),  \
            hour(), minute(), second());



//sensor 1 sd card string setup
  sprintf_P(buf, PSTR("$%s,%d,%s,,,%s,A,%s,%d,A,,"),  \
            HEADER, \
            config.user_id, \
            timestamp, \
            CPM_string, \
            lat_lon_nmea, \
            config.alt);

  len = strlen(buf);
  buf[len] = '\0';

  // generate checksum
  chk = checksum(buf + 1, len);

  // add checksum to end of line before sending
  if (chk < 16)
    sprintf_P(buf + len, PSTR("*0%X"), (int)chk);
  else
    sprintf_P(buf + len, PSTR("*%X"), (int)chk);
#ifdef ENABLE_DEBUG
  Serial.println(buf);
#endif



  //sensor 2 sd card string setup
  sprintf_P(buf2, PSTR("$%s,%d,%s,,,%s,A,%s,%d,A,,"),  \
            HEADER, \
            config.user_id2, \
            timestamp, \
            CPM2_string, \
            lat_lon_nmea, \
            config.alt);

  len2 = strlen(buf2);
  buf2[len2] = '\0';

  // generate checksum
  chk2 = checksum(buf2 + 1, len2);


  // add checksum to end of line before sending
  if (chk2 < 16)
    sprintf_P(buf2 + len2, PSTR("*0%X"), (int)chk2);
  else
    sprintf_P(buf2 + len2, PSTR("*%X"), (int)chk2);


  //add second line for additional info
  sprintf_P(buf2 + len2, PSTR("*%X%s$%s,%d,%s,%s,%ld,%ld,%ld,%s,%d"),
            (int)chk, \
            "\n", \
            HEADER_SENSOR,  \
            config.devid, \
            temperature_string, \
            battery_string, \
            dose.fails, \
            dose.restarts, \
            FreeRam(), \
            config.last_failure, \
            ntpcount);
#ifdef ENABLE_DEBUG
  Serial.println(buf2);
#endif

  if (openlog_ready) {
    //write to sd card sensor 1 info
    OpenLog.println(buf);
    //write to Xbee info
    XbeeSerial.print(buf);
    //write to sd card sensor 2 info
    OpenLog.println(buf2);
    //write to Xbee info
    XbeeSerial.print(buf2);
  } else {
    lcd.setCursor(13, 2);
    lcd.print("SD FAIL");
    String last_failure = "SD FAIL";
#ifdef ENABLE_DEBUG
    Serial.print (last_failure);
#endif
    //alarm peep
    digitalWrite(28, HIGH);
    pinMode(28, OUTPUT);
    delay(250);
    pinMode(28, INPUT);
    //switch on fail LED
    digitalWrite(26, HIGH);

  }

sendEN:
//maintain DHCP assigned IP for networks that have short DHCP lease setup and restart Ethernet if renew or rebind is failed

  if ( Ethernet.maintain() == 1 || Ethernet.maintain() == 3) {
    Ethernet.begin(macAddress, localIP);
  }

//send  sensor  1

  if (client.connected())
  {
#ifdef ENABLE_DEBUG
    Serial.println("Disconnecting");
#endif
    client.stop();
  }

  // Try to connect to the server
#ifdef ENABLE_DEBUG
  Serial.println("Connecting");
#endif
  if (client.connect(server, 80))
  {
#ifdef ENABLE_DEBUG
    Serial.println("Connected");
#endif


    // clear the connection fail count if we have at least one successful connection
    ctrl.conn_fail_cnt = 0;
  }
  else
  {
    ctrl.conn_fail_cnt++;
    if (ctrl.conn_fail_cnt >= MAX_FAILED_CONNS)
    {
      //restart
      delay (3000);
      CPU_RESTART;
    }

    lcd.setCursor(13, 2);
    lcd.print(" FAIL");
    //add fail to EEProm
    dose.fails++;
    Serial.println("dos.fail++");
    EEPROM_writeAnything(BMRDD_EEPROM_DOSE, dose);
    lcd.print(" ");
#ifdef ENABLE_DEBUG
    Serial.print(last_failure);
#endif
    //alarm peep
    digitalWrite(28, HIGH);
    pinMode(28, OUTPUT);
    delay(250);
    pinMode(28, INPUT);
    //switch on fail LED
    digitalWrite(26, HIGH);
    lcd.print(ctrl.conn_fail_cnt);
    lastConnectionTime = millis();
    delay (3000);
    goto sendEN;
    return;
  }

// display sending for sensor 1
  lcd.setCursor(0, 2);
  lcd.print(config.dev ? "DEV:" : "API:");
  lcd.print("sending S1    ");

  // prepare the log entry for sensor 1
  sprintf_P(json_buf, PSTR("{\"longitude\":\"%s\",\"latitude\":\"%s\",\"device_id\":\"%d\",\"value\":\"%s\",\"unit\":\"cpm\",\"height\":\"%d\",\"devicetype_id\":\"Pointcast V1\"}"),  \
            config.longitude, \
            config.latitude, \
            config.user_id,  \
            CPM_string, \
            config.alt);

  int len = strlen(json_buf);
  json_buf[len] = '\0';
#ifdef ENABLE_DEBUG
  Serial.println(json_buf);
#endif

  if (config.dev) {
    client.print("POST /scripts/indextest.php?api_key=");
#ifdef ENABLE_DEBUG
    Serial.println ("sending to dev.safecast.org");
#endif
  } else {
    client.print("POST /scripts/index.php?api_key=");
#ifdef ENABLE_DEBUG
    Serial.println ("sending to api.safecast.org");
#endif
  }
  client.print(config.api_key);
  client.println(" HTTP/1.1");
  client.println("Accept: application/json");
  client.print("Host:");
  client.println(server);
  client.print("Content-Length: ");
  client.println(strlen(json_buf));
  client.println("Content-Type: application/json");
  client.println();
  client.println(json_buf);

// get confirmation from API
  int GetmeasurementReplyReturn = GetMeasurementReply(15000);

  if (!GetmeasurementReplyReturn) {
#ifdef ENABLE_DEBUG
    Serial.println("Timed out");
#endif
    fail_cnt++;
#ifdef ENABLE_DEBUG
    Serial.print("Connections Failed = ");
    Serial.println(fail_cnt);
#endif
    if (fail_cnt >= MAX_FAILED_CONNS)
    {
      //restart
      delay (2000);
      CPU_RESTART;
    }
  } else {
#ifdef ENABLE_DEBUG
    Serial.print("Measurement ID sensor 1= ");
    Serial.println(GetmeasurementReplyReturn);
#endif
    fail_cnt = 0;
  }

#ifdef ENABLE_DEBUG
  Serial.println("Disconnecting");
#endif



  //send sensor 2

  if (client.connected())
  {
#ifdef ENABLE_DEBUG
    Serial.println("Disconnecting");
#endif
    client.stop();
  }

  // Try to connect to the server
  Serial.println("Connecting");
  if (client.connect(server, 80))
  {
#ifdef ENABLE_DEBUG
    Serial.println("Connected");
#endif


    // clear the connection fail count if we have at least one successful connection
    ctrl.conn_fail_cnt = 0;
  }
  else
  {
    ctrl.conn_fail_cnt++;
    if (ctrl.conn_fail_cnt >= MAX_FAILED_CONNS)
    {
      //restart
      delay (3000);
      CPU_RESTART;
    }

    lcd.setCursor(13, 2);
    lcd.print(" FAIL");
    String last_failure = "NC S2";
    lcd.print(" ");
#ifdef ENABLE_DEBUG
    Serial.print(last_failure);
#endif
    //alarm peep
    digitalWrite(28, HIGH);
    pinMode(28, OUTPUT);
    delay(250);
    pinMode(28, INPUT);
    //switch on fail LED
    EEPROM_writeAnything(BMRDD_EEPROM_DOSE, dose);
    digitalWrite(26, HIGH);

    lcd.print(ctrl.conn_fail_cnt);
    //restart
    Ethernet.maintain();
    delay (3000);
    CPU_RESTART;
    lastConnectionTime = millis();
    return;
  }

// display sending for sensor 1
  lcd.setCursor(0, 2);
  lcd.print(config.dev ? "DEV:" : "API:");
  lcd.print("sending S2    ");

  // prepare the log entry for sensor 2
  sprintf_P(json_buf2, PSTR("{\"longitude\":\"%s\",\"latitude\":\"%s\",\"device_id\":\"%d\",\"value\":\"%s\",\"unit\":\"cpm\",\"height\":\"%d\",\"devicetype_id\":\"Pointcast V1\"}"),  \
            config.longitude, \
            config.latitude, \
            config.user_id2,  \
            CPM2_string, \
            config.alt);

  int len2 = strlen(json_buf2);
  json_buf2[len2] = '\0';
#ifdef ENABLE_DEBUG
  Serial.println(json_buf2);
#endif


  if (config.dev) {
    client.print("POST /scripts/indextest.php?api_key=");
#ifdef ENABLE_DEBUG
    Serial.println ("sending to dev.safecast.org");
#endif
  } else {
    client.print("POST /scripts/index.php?api_key=");
#ifdef ENABLE_DEBUG
    Serial.println ("sending to api.safecast.org");
#endif
  }
  client.print(config.api_key);
  client.println(" HTTP/1.1");
  client.println("Accept: application/json");
  client.print("Host:");
  client.println(server);
  client.print("Content-Length: ");
  client.println(strlen(json_buf2));
  client.println("Content-Type: application/json");
  client.println();
  client.println(json_buf2);

// get confirmation from API
  unsigned long GetmeasurementReplyReturn1 = GetMeasurementReply(15000);

  if (!GetmeasurementReplyReturn1) {
#ifdef ENABLE_DEBUG
    Serial.println("Timed out");
#endif
    fail_cnt++;
#ifdef ENABLE_DEBUG
    Serial.print("Connections Failed = ");
    Serial.println(fail_cnt);
#endif
    if (fail_cnt >= MAX_FAILED_CONNS)
    {
      //restart
      delay (2000);
      CPU_RESTART;
    }
  } else {
#ifdef ENABLE_DEBUG
    Serial.print("Measurement ID sensor 2= ");
    Serial.println(GetmeasurementReplyReturn1);
#endif
    fail_cnt = 0;
  }


  lcd.setCursor(11, 2);
  lcd.print("  PASS   ");
#ifdef ENABLE_DEBUG
  Serial.println("Disconnecting");
#endif
  //switch off fail LED
  digitalWrite(26, LOW);
  //client.stop();

  //send temperature-----------------------------

  if (client.connected())
  {
    Serial.println("Disconnecting");
    client.stop();
  }

  // Try to connect to the server
  Serial.println("Connecting");
  if (client.connect(server, 80))
  {
    Serial.println("Connected");
    lastConnectionTime = millis();

    // clear the connection fail count if we have at least one successful connection
    ctrl.conn_fail_cnt = 0;
  }
  else
  {
    ctrl.conn_fail_cnt++;
    lcd.setCursor(13, 2);
    lcd.print(" FAIL");
    //add fail to EEProm
    dose.fails++;
    Serial.println("dos.fail++");
    EEPROM_writeAnything(BMRDD_EEPROM_DOSE, dose);
    lcd.print(" ");
    digitalWrite(28, HIGH);
    pinMode(28, OUTPUT);
    delay(250);
    pinMode(28, INPUT);
    //switch on fail LED
    digitalWrite(26, HIGH);

    lcd.print(ctrl.conn_fail_cnt);
    //restart
    Ethernet.maintain();
    delay (3000);
    CPU_RESTART;
    return;
  }

// display sending for sensor 1
  lcd.setCursor(0, 2);
  lcd.print(config.dev ? "DEV:" : "API:");
  lcd.print("sending Status ");

  //put Statics of sensor into STATS
  sprintf_P(STATS, PSTR("DeviceID:%d,Temperature:%s,Battery Voltage:%s,Fails:%ld,Restarts:%ld,FreeRam:%ld,Last failure:%s,NTP count:%d"),
            config.devid, \
            temperature_string, \
            battery_string, \
            dose.fails, \
            dose.restarts, \
            FreeRam(), \
            config.last_failure, \
            ntpcount);

#ifdef ENABLE_DEBUG
  Serial.print("STATS = ");
  Serial.println(STATS);
#endif

  // prepare the log entry for temperature
  memset(json_buf2, 0, SENT_SZ);
  sprintf_P(json_buf2, PSTR("{\"longitude\":\"%s\",\"latitude\":\"%s\",\"device_id\":\"%d\",\"value\":\"%s\",\"unit\":\"status\",\"height\":\"%d\",\"devicetype_id\":\"%s\"}"),  \
            config.longitude, \
            config.latitude, \
            tempID,  \
            temperature_string, \
            config.alt,
            STATS);

  int len4 = strlen(json_buf2);
  json_buf2[len4] = '\0';
  Serial.println(json_buf2);

  if (config.dev) {
    client.print("POST /scripts/indextest.php?api_key=");
    Serial.println ("sending to dev.safecast.org");
  } else {

    client.print("POST /scripts/index.php?api_key=");
    Serial.println ("sending to api.safecast.org");
  }
  client.print(config.api_key);
  client.println(" HTTP/1.1");
  client.println("Accept: application/json");
  client.print("Host:");
  client.println(server);
  client.print("Content-Length: ");
  client.println(strlen(json_buf2));
  client.println("Content-Type: application/json");
  client.println();
  client.println(json_buf2);

// get confirmation from API
  unsigned long GetmeasurementReplyReturn2 = GetMeasurementReply(15000);

  if (!GetmeasurementReplyReturn2) {
#ifdef ENABLE_DEBUG
    Serial.println("Timed out");
#endif
    fail_cnt++;
#ifdef ENABLE_DEBUG
    Serial.print("Connections Failed = ");
    Serial.println(fail_cnt);
#endif
    if (fail_cnt >= MAX_FAILED_CONNS)
    {
      //restart
      delay (2000);
      CPU_RESTART;
    }
  } else {
#ifdef ENABLE_DEBUG
    Serial.print("Measurement Temperature= ");
    Serial.println(GetmeasurementReplyReturn2);
#endif
    fail_cnt = 0;
  }

  lcd.setCursor(11, 2);
  lcd.print("  PASS   ");
  Serial.println("Disconnecting");



  //display battery
  lcd.setCursor(0, 3);
  lcd.print("STS:");
  lcd.setCursor(4, 3);
  lcd.print(temperature_string);
  lcd.print("C");
  lcd.setCursor(10, 3);
  lcd.print(battery_string);
  lcd.print("V");

  // report to LCD
  lcd.setCursor(4, 2);
  printDigits(hour());
  lcd.print(":");
  printDigits(minute());
  lcd.print("UTC");

  lastConnectionTime = millis();
  return;

#endif


  /**************************************************************************/
// 3G card sending setup
  /**************************************************************************/


#if ENABLE_3G


//Get temp and Battery
  float temperature = getTemp();
  char temperature_string[5];
  dtostrf(temperature, 0, 1, temperature_string);

  float battery = ((read_voltage(VOLTAGE_PIN)));
  char battery_string[5];
  dtostrf(battery, 0, 2, battery_string);


// Convert from cpm to µSv/h with the pre-defined coefficient
  if (a3gs.begin() == 0)
#ifdef ENABLE_DEBUG
    Serial.println("Succeeded.");
#endif
  else {
#ifdef ENABLE_DEBUG
    Serial.println("Failed.");
#endif
  }


  conversionCoefficient = 1 / config.sensor1_cpm_factor;
  float uSv = CPM * conversionCoefficient;                   // convert CPM to Micro Sievers Per Hour
  char CPM_string[16];
  dtostrf(CPM, 0, 0, CPM_string);
  if (CPM > (int)(config.S1peak)) {
    config.S1peak = CPM;
#ifdef ENABLE_DEBUG
    Serial.print ("Updating S1peak");
#endif
  }
#ifdef ENABLE_DEBUG
  Serial.print ("S1peak =");
  Serial.println (config.S1peak);
#endif
  conversionCoefficient2 = 1 / config.sensor2_cpm_factor;
  float uSv2 = CPM2 * conversionCoefficient2;                // convert CPM to Micro Sievers Per Hour
  char CPM2_string[16];
  dtostrf(CPM2, 0, 0, CPM2_string);
  if (CPM2 > (int)(config.S2peak)) {
    config.S2peak = CPM2;
#ifdef ENABLE_DEBUG
    Serial.print ("Updating S2peak");
#endif
  }
#ifdef ENABLE_DEBUG
  Serial.print ("S2peak =");
  Serial.println (config.S2peak);
#endif



  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S1:");
  if (CPM >= 1000) {
    dtostrf((float)(CPM / 1000.0), 4, 3, strbuffer);
    strncpy (strbuffer1, strbuffer, 4);
    if (strbuffer1[strlen(strbuffer1) - 1] == '.') {
      strbuffer1[strlen(strbuffer1) - 1] = 0;
    }
    lcd.print(strbuffer1);
    sprintf_P(strbuffer, PSTR("kCPM "));
    lcd.print(strbuffer);
  } else {
    dtostrf((float)CPM, 0, 0, strbuffer);
    lcd.print(strbuffer);
    sprintf_P(strbuffer, PSTR(" CPM "));
    lcd.print(strbuffer);
  }
  lcd.print(uSv);
  lcd.print("uSh");
  lcd.setCursor(0, 1);
  lcd.print("S2:");
  if (CPM2 >= 1000) {
    dtostrf((float)(CPM2 / 1000.0), 4, 3, strbuffer);
    strncpy (strbuffer1, strbuffer, 4);
    if (strbuffer1[strlen(strbuffer1) - 1] == '.') {
      strbuffer1[strlen(strbuffer1) - 1] = 0;
    }
    lcd.print(strbuffer1);
    sprintf_P(strbuffer, PSTR("kCPM "));
    lcd.print(strbuffer);
  } else {
    dtostrf((float)CPM2, 0, 0, strbuffer);
    lcd.print(strbuffer);
    sprintf_P(strbuffer, PSTR(" CPM "));
    lcd.print(strbuffer);
  }
  lcd.print(uSv2);
  lcd.print("uSh");
  lcd.setCursor(0, 2);
  lcd.print(config.dev ? "DEV:" : "API:");
  lcd.print("sending ");


//check level
  if (a3gs.getRSSI(rssi) == 0) {
#ifdef ENABLE_DEBUG
    Serial.print("RSSI = ");
    Serial.print(rssi);
    Serial.println("dBm");
    lcd.setCursor(13, 3);
    lcd.print(rssi);
    lcd.print("dBm");
#endif
  }


//convert time in correct format
  memset(timestamp, 0, LINE_SZ);
  sprintf_P(timestamp, PSTR("%02d-%02d-%02dT%02d:%02d:%02dZ"),  \
            year(), month(), day(),  \
            hour(), minute(), second());


// convert degree to NMAE
  deg2nmae (config.latitude, config.longitude, lat_lon_nmea);


//sensor 1 sd card string setup
  memset(buf, 0, LINE_SZ);
  sprintf_P(buf, PSTR("$%s,%d,%s,,,%s,A,%s,1,A,,"),  \
            HEADER, \
            config.user_id, \
            timestamp, \
            CPM_string, \
            lat_lon_nmea);

  len = strlen(buf);
  buf[len] = '\0';

// generate checksum
  chk = checksum(buf + 1, len);

// add checksum to end of line before sending
  if (chk < 16)
    sprintf_P(buf + len, PSTR("*0%X"), (int)chk);
  else
    sprintf_P(buf + len, PSTR("*%X"), (int)chk);

#ifdef ENABLE_DEBUG
  Serial.println(buf);
#endif

//sensor 2 sd card string setup
  memset(buf2, 0, LINE_SZ);
  sprintf_P(buf2, PSTR("$%s,%d,%s,,,%s,A,%s,1,A,,"),  \
            HEADER, \
            config.user_id2, \
            timestamp, \
            CPM2_string, \
            lat_lon_nmea);

  len2 = strlen(buf2);
  buf2[len2] = '\0';


// generate checksum
  chk2 = checksum(buf2 + 1, len2);


// add checksum to end of line before sending
  if (chk2 < 16)
    sprintf_P(buf2 + len2, PSTR("*0%X"), (int)chk2);
  else
    sprintf_P(buf2 + len2, PSTR("*%X"), (int)chk2);


  //add second line for additional info
  sprintf_P(buf2 + len2, PSTR("*%X%s$%s,%d,%s,%s,%d,%d,%d,%s,%d"),
            (int)chk, \
            "\n", \
            HEADER_SENSOR,  \
            config.devid, \
            temperature_string, \
            battery_string, \
            dose.fails, \
            dose.restarts, \
            FreeRam(), \
            config.last_failure, \
            ntpcount);

#ifdef ENABLE_DEBUG
  Serial.println(buf2);
#endif

  if (openlog_ready) {
    //write to sd card sensor 1 info
    OpenLog.println(buf);
    XbeeSerial.println(buf);
    //write to sd card sensor 2 info
    OpenLog.println(buf2);
    XbeeSerial.println(buf2);
  } else {
    lcd.setCursor(13, 2);
    lcd.print("SD FAIL");
    //alarm peep
    digitalWrite(28, HIGH);
    pinMode(28, OUTPUT);
    delay(250);
    pinMode(28, INPUT);
    //switch on fail LED
    digitalWrite(26, HIGH);
  }


//send to server for httpPOST
// Create path string for sensor 1 and 2
  len = sizeof(res);
  len2 = sizeof(res2);
  len4 = sizeof(res4);
  if (config.dev) {
    sprintf_P(path, PSTR("scripts/indextest.php?api_key=%s"), \
              config.api_key);

  } else {
    sprintf_P(path, PSTR("scripts/index.php?api_key=%s"), \
              config.api_key);
  }




//create body, body2, body3  ....\$\"value\$\" : \$\"60\$\"
  sprintf_P(body, PSTR("{\$\"longitude\$\":\$\"%s\$\",\$\"latitude\$\":\$\"%s\$\",\$\"device_id\$\":\$\"%d\$\",\$\"value\$\":\$\"%s\$\",\$\"unit\$\":\$\"cpm\$\",\$\"height\$\":\$\"%d\$\"}"),  \
            config.longitude, \
            config.latitude, \
            config.user_id, \
            CPM_string, \
            config.alt);

  sprintf_P(body2, PSTR("{\$\"longitude\$\":\$\"%s\$\",\$\"latitude\$\":\$\"%s\$\",\$\"device_id\$\":\$\"%d\$\",\$\"value\$\":\$\"%s\$\",\$\"unit\$\":\$\"cpm\$\",\$\"height\$\":\$\"%d\$\"}"),  \
            config.longitude, \
            config.latitude, \
            config.user_id2,  \
            CPM2_string, \
            config.alt);

//put Statics of sensor into STATS
  sprintf_P(STATS, PSTR("DeviceID:%d,Temperature:%s,Battery Voltage:%s,Fails%ld,Restarts:%ld,FreeRam:%ld,Last failure:%s,NTP count:%d"),
            config.devid, \
            temperature_string, \
            battery_string, \
            dose.fails, \
            dose.restarts, \
            FreeRam(), \
            config.last_failure, \
            ntpcount);

#ifdef ENABLE_DEBUG
  Serial.print("STATS = ");
  Serial.println(STATS);
#endif

  sprintf_P(body3, PSTR("{\$\"longitude\$\":\$\"%s\$\",\$\"latitude\$\":\$\"%s\$\",\$\"device_id\$\":\$\"%d\$\",\$\"value\$\":\$\"%s\$\",\$\"unit\$\":\$\"status\$\",\$\"height\$\":\$\"%d\$\",\$\"devicetype_id\$\":\$\"%s\$\"}"),  \
            config.longitude, \
            config.latitude, \
            tempID,  \
            temperature_string, \
            config.alt, \
            STATS );


send3G:
  sprintf_P(strbuffer, PSTR("%s"), server);
  Serial.println(strbuffer);

  if (a3gs.httpPOST(strbuffer, port, path, header, body, res, &len, useHTTPS) == 0  ) {

    String res_string(res);
    int startChar = res_string.indexOf("\"id\":");
    res_string.remove(0, startChar + 5);
    int endComma = res_string.indexOf(",");
    replysub = res_string.substring(0, endComma);
    res_string="";

    if (replysub.toInt() == 0) {
#ifdef ENABLE_DEBUG
      Serial.println("Timed out");
#endif
      fail_cnt++;
      lcd.setCursor(11, 2);
      lcd.print("  Fail   ");
#ifdef ENABLE_DEBUG
      Serial.print("Connections Failed = ");
      Serial.println(fail_cnt);
#endif
      if (fail_cnt >= MAX_FAILED_CONNS)
      {
        //restart
        delay (2000);
        CPU_RESTART;
      }
    } else {
#ifdef ENABLE_DEBUG
      Serial.print("Measurement S1=");
      Serial.println(replysub);
#endif
      fail_cnt = 0;
      lcd.setCursor(11, 2);
      lcd.print("  PASS   ");
      replysub ="";
#ifdef ENABLE_DEBUG
      Serial.println("Disconnecting");
#endif
    }


#ifdef ENABLE_DEBUG
    Serial.println("Sent sensor 1 info to server OK!");
    Serial.print(">Response=[");
    Serial.print(res);
    Serial.println("]");
#endif

    delay (5000);


    a3gs.httpPOST(strbuffer, port, path, header, body2, res2, &len2, useHTTPS);

    String res_string2(res2);
    startChar = res_string2.indexOf("\"id\":");
    res_string2.remove(0, startChar + 5);
    endComma = res_string2.indexOf(",");
    replysub = res_string2.substring(0, endComma);
    res_string2="";
    if (replysub.toInt() == 0) {
#ifdef ENABLE_DEBUG
      Serial.println("Timed out");
#endif
      fail_cnt++;
      lcd.setCursor(11, 2);
      lcd.print("  Fail   ");
#ifdef ENABLE_DEBUG
      Serial.print("Connections Failed = ");
      Serial.println(fail_cnt);
#endif
      if (fail_cnt >= MAX_FAILED_CONNS)
      {
        //restart
        delay (2000);
        CPU_RESTART;
      }
    } else {
#ifdef ENABLE_DEBUG
      Serial.print("Measurement S2=");
      Serial.println(replysub);
#endif
      fail_cnt = 0;
      lcd.setCursor(11, 2);
      lcd.print("  PASS   ");
      replysub="";

#ifdef ENABLE_DEBUG
      Serial.println("Disconnecting");
#endif
    }


#ifdef ENABLE_DEBUG
    Serial.println("Sent sensor 2 info to server OK!");
    Serial.print(">Response=[");
    Serial.print(res2);
    Serial.println("]");
#endif

    delay (3000);



    a3gs.httpPOST(strbuffer, port, path, header, body3, res4, &len4, useHTTPS);

    String res_string4(res4);
    startChar = res_string4.indexOf("\"id\":");
    res_string4.remove(0, startChar + 5);
    endComma = res_string4.indexOf(",");
    replysub = res_string4.substring(0, endComma);
    res_string4 = "";

    if (replysub.toInt() == 0) {
#ifdef ENABLE_DEBUG
      Serial.println("Timed out");
#endif
      fail_cnt++;
      lcd.setCursor(11, 2);
      lcd.print("  Fail   ");
#ifdef ENABLE_DEBUG
      Serial.print("Connections Failed = ");
      Serial.println(fail_cnt);
#endif
      if (fail_cnt >= MAX_FAILED_CONNS)
      {
        //restart
        delay (2000);
        CPU_RESTART;
      }
    } else {
#ifdef ENABLE_DEBUG
      Serial.print("Measurement Status = ");
      Serial.println(replysub);
#endif
      fail_cnt = 0;
      lcd.setCursor(11, 2);
      lcd.print("  PASS   ");
      replysub ="";

#ifdef ENABLE_DEBUG
      Serial.println("Disconnecting");
#endif
    }


#ifdef ENABLE_DEBUG
    Serial.println("Sent Status info to server OK!");
    Serial.print(">Response=[");
    Serial.print(res4);
    Serial.println("]");
#endif

    //switch on fail LED off
    digitalWrite(26, LOW);

    //Display information
    lcd.setCursor(13, 2);
    lcd.print("PASS   ");
    lcd.setCursor(0, 3);
    lcd.setCursor(0, 3);
    lcd.print("STS:");
    lcd.setCursor(4, 3);
    lcd.print(temperature_string);
    lcd.print("C");
    lcd.setCursor(10, 3);
    lcd.print(battery);
    lcd.print("V");

  }
  else {

    lcd.setCursor(13, 2);
#ifdef ENABLE_DEBUG
    Serial.println("No connection to API!");
    Serial.println("saving to SDcard only");
#endif
    ctrl.conn_fail_cnt++;
    if (ctrl.conn_fail_cnt >= MAX_FAILED_CONNS)
    {
      //first shut down 3G before reset
      a3gs.end();
      a3gs.shutdown();
      CPU_RESTART;
    }

    lcd.print(" FAIL");
    //add fail to EEProm
    dose.fails++;
    Serial.println("dos.fail++");
    EEPROM_writeAnything(BMRDD_EEPROM_DOSE, dose);
    failures++;
    //alarm peep
    digitalWrite(28, HIGH);
    pinMode(28, OUTPUT);
    delay(250);
    pinMode(28, INPUT);
    //switch on fail LED
    digitalWrite(26, HIGH);
    //display fails
    lcd.print(ctrl.conn_fail_cnt);
    lcd.print(" ");
#ifdef ENABLE_DEBUG
    Serial.print("NC. Retries left:");
    Serial.println(MAX_FAILED_CONNS - ctrl.conn_fail_cnt);
#endif
    lastConnectionTime = millis();
    delay (1000);
    goto send3G;
  }


#endif
//   end of 3G send

//for all modules
  lastConnectionTime = millis();


// report to LCD
  lcd.setCursor(4, 2);
  printDigits(hour());
  lcd.print(":");
  printDigits(minute());
  lcd.print("UTC");

}
//end SendDataToServer(


/**************************************************************************/
// Main Loop sliding windows
/**************************************************************************/
void loop() {

  // Main Loop
  finished_startup = true;

  // check for weekly alarm
  Alarm.delay(0);


  //display timer setup

  int leftMillis = (updateIntervalInMillis - elapsedTime(lastConnectionTime));
  int left_secs = leftMillis / 1000; //convert milliseconds to seconds
  int left_mins = left_secs / 60; //convert seconds to minutes
  left_secs = left_secs - (left_mins * 60); //subtract the converted seconds to minutes in order to display 59 secs max

  //display current countdown time
  if (displayTimeOn) {
    lcd.setCursor(15, 3);
    printDigitsSingle(left_mins);
    lcd.print(":");
    printDigits(left_secs);
  }



  unsigned long cpb1 = 0, cpb2 = 0;

  if (elapsedTime(lastConnectionTime) < updateIntervalInMillis)
  {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; joyCntD = false; joyCntE = false; lcd.clear(); display_interval = 1000; Menu_datalogger(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntD = false; joyCntB = false; joyCntE = false; lcd.clear(); display_interval = 1000; Menu_stat(); return;
    }
    if (joyCntE) {

#ifdef ENABLE_DEBUG
      Serial.println ("Enter");
#endif
      joyCntE = !joyCntE; joyCntD = false; joyCntB = false; joyCntA = false; digitalWrite(26, LOW); return;
    }
    if (joyCntD) {

#ifdef ENABLE_DEBUG
      Serial.println ("Right");
#endif
      joyCntD = !joyCntD; joyCntA = false; joyCntB = false; lcd.clear(); joyCntE = false; display_interval = 1000; Menu_term(); return;
    }


    //sliding window setup

    if IS_READY {

    cpb1 = interruptCounterCount();
      cpb2 = interruptCounterCount2();

      interruptCounterReset();

      // insert count in sliding window and compute CPM
      shift_reg[reg_index] = cpb1;     // put the count in the correct bin
      reg_index = (reg_index + 1) % NX; // increment register index
      cpm1 = cpm_gen1();               // compute sum over all bins

      // insert count in sliding window and compute CPM2
      shift_reg2[reg_index2] = cpb2;    // put the count in the correct bin
      reg_index2 = (reg_index2 + 1) % NX; // increment register index
      cpm2 = cpm_gen2();  // compute sum over all bins
    }

    Alarm.delay(0);
    return;

  }

#ifdef ENABLE_DEBUG
  Serial.print("FreeRam = ");
  Serial.println(FreeRam());
#endif

  SendDataToServer(cpm1, cpm2);
}


///**************************************************************************/
// Menus for stat etc..
//**************************************************************************/

void Menu_stat() {
  displayTimeOn = false;
  lcd.clear();
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; displayTimeOn = true; Menu_counting(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; display_interval = 500; Menu_stat2(); return;
    }

    lcd.setCursor(0, 0);
    lcd.print("STATS");
    lcd.setCursor(0, 1);
    lcd.print("S1peak=");
    lcd.print(config.S1peak);
    lcd.setCursor(0, 2);
    lcd.print("S2peak=");
    lcd.print(config.S2peak);
    lcd.setCursor(0, 3);
    lcd.print("FreeRam=");
    lcd.print(FreeRam());

  }
}

//**************************************************************************/
// Menus for stat2
//**************************************************************************/

void Menu_stat2() {

  displayTimeOn = false;
  // check timeup
  currentmillis = millis(); // get the  current milliseconds from arduino
#ifdef ENABLE_DEBUG
  Serial.print("Total milliseconds running: ");
#endif
  display_secs = currentmillis / 1000; //convect milliseconds to seconds
  display_mins = display_secs / 60; //convert seconds to minutes
  display_hours = display_mins / 60; //convert minutes to hours
  display_days = display_hours / 24; //convert hours to days
  display_secs = display_secs - (display_mins * 60); //subtract the converted seconds to minutes in order to display 59 secs max
  display_mins = display_mins - (display_hours * 60); //subtract the converted minutes to hours in order to display 59 minutes max
  display_hours = display_hours - (display_days * 24); //sub
#ifdef ENABLE_DEBUG
  Serial.printf("%d days, %d hours, %d minutes \n", display_days, display_hours, display_mins);
#endif

  lcd.clear();
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {

    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; Menu_stat(); return;
    }

    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Down");
#endif
      joyCntA = !joyCntA; joyCntB = false; lcd.clear(); display_interval = 3000; displayTimeOn = true; Menu_counting(); return;
    }

    lcd.setCursor(0, 0);
    lcd.print("up=");
    lcd.print(display_days);
    lcd.print("d ");
    lcd.print(display_hours);
    lcd.print("h ");
    lcd.print(display_mins);
    lcd.print("m ");
    lcd.setCursor(0, 1);
    lcd.print("#logs=");
    lcd.print(dose.logs);
    lcd.setCursor(0, 2);
    lcd.print("#fail=");
    lcd.print(dose.fails);
    lcd.setCursor(0, 3);
    lcd.print("#restarts=");
    lcd.print(dose.restarts);
  }
  return;
}

//**************************************************************************/
// Menus for terminal
//**************************************************************************/

void Menu_term() {
  displayTimeOn = false;
  lcd.clear();
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)display_interval) {
    if (joyCntB) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntB = !joyCntB; joyCntA = false; lcd.clear(); display_interval = 3000; displayTimeOn = true; Menu_counting(); return;
    }
    if (joyCntA) {
#ifdef ENABLE_DEBUG
      Serial.println ("Up");
#endif
      joyCntA = !joyCntA; joyCntB = false; display_interval = 500; displayTimeOn = true; Menu_counting(); return;
    }

#if ENABLE_3G
    String res_string(res);
    int startChar = res_string.indexOf("\"id\":");
    res_string.remove(0, startChar + 5);
    int endComma = res_string.indexOf(",");

    String res2_string(res2);
    int startChar2 = res2_string.indexOf("\"id\":");
    res2_string.remove(0, startChar2 + 5);
    int endComma2 = res2_string.indexOf(",");

    String res4_string(res4);
    int startChar4 = res4_string.indexOf("\"id\":");
    res4_string.remove(0, startChar4 + 5);
    int endComma4 = res4_string.indexOf(",");
#endif

#if ENABLE_ETHERNET


#endif

    lcd.setCursor(0, 0);
    lcd.print("Terminal: API ID");
    lcd.setCursor(0, 1);
    lcd.print("S1=");
#if ENABLE_3G
    if (startChar != -1) {
      lcd.print(res_string.substring(0, endComma));
    }
#endif
    lcd.setCursor(0, 2);
    lcd.print("S2=");
#if ENABLE_3G
    if (startChar2 != -1) {
      lcd.print(res2_string.substring(0, endComma2));
    }
#endif
    lcd.setCursor(0, 4);
    lcd.print("TMP=");
#if ENABLE_3G
    if (startChar4 != -1) {
      lcd.print(res4_string.substring(0, endComma4));
    }
#endif

  }
  Menu_counting();
}


/**************************************************************************/
// calculate elapsed time, taking into account rollovers
/**************************************************************************/
unsigned long elapsedTime(unsigned long startTime)
{
  unsigned long stopTime = millis();

  if (startTime >= stopTime)
  {
    return startTime - stopTime;
  }
  else
  {
    return (ULONG_MAX - (startTime - stopTime));
  }
}



/**************************************************************************/
// OpenLog code
/**************************************************************************/

/* wait for openlog prompt */
bool waitOpenLog(bool commandMode) {
  int safeguard = 0;
  bool result = false;

  while (safeguard < OPENLOG_RETRY) {
    safeguard++;
    if (OpenLog.available())
      if (OpenLog.read() == (commandMode ? '>' : '<')) break;
    delay(10);
  }

  if (safeguard >= OPENLOG_RETRY) {
  } else {
    result = true;
  }

  return result;
}

/* setups up the software serial, resets OpenLog */
void setupOpenLog() {
  pinMode(resetOpenLog, OUTPUT);
  OpenLog.listen();

  // reset OpenLog
  digitalWrite(resetOpenLog, LOW);
  delay(100);
  digitalWrite(resetOpenLog, HIGH);

  if (!waitOpenLog(true)) {
    logfile_ready = true;
  } else {
    openlog_ready = true;
  }
}

/* create a new file */
void createFile(char *fileName) {
  int result = 0;
  dose.logs++;
  EEPROM_writeAnything(BMRDD_EEPROM_DOSE, dose);
  OpenLog.listen();

  do {
    result = 0;

    do {
      OpenLog.print("append ");
      OpenLog.print(fileName);
      OpenLog.write(13); //This is \r

      if (!waitOpenLog(false)) {
        break;
      }
      result = 1;
    } while (0);

    if (0 == result) {
      // reset OpenLog
      digitalWrite(resetOpenLog, LOW);
      delay(100);
      digitalWrite(resetOpenLog, HIGH);

      // Wait for OpenLog to return to waiting for a command
      waitOpenLog(true);
    }
  } while (0 == result);

  //OpenLog is now waiting for characters and will record them to the new file
}


/**************************************************************************/
//WDT setup
/**************************************************************************/

void KickDog() {
  green_led_blink();
  noInterrupts();
  WDOG_REFRESH = 0xA602;
  WDOG_REFRESH = 0xB480;
  interrupts();
}

//WDT error messages
void printResetType() {
#ifdef ENABLE_DEBUG
  if (RCM_SRS1 & RCM_SRS1_SACKERR)   Serial.println("[RCM_SRS1] - Stop Mode Acknowledge Error Reset");
  if (RCM_SRS1 & RCM_SRS1_MDM_AP)    Serial.println("[RCM_SRS1] - MDM-AP Reset");
  if (RCM_SRS1 & RCM_SRS1_SW)        Serial.println("[RCM_SRS1] - Software Reset");
  if (RCM_SRS1 & RCM_SRS1_LOCKUP)    Serial.println("[RCM_SRS1] - Core Lockup Event Reset");
  if (RCM_SRS0 & RCM_SRS0_POR)       Serial.println("[RCM_SRS0] - Power-on Reset");
  if (RCM_SRS0 & RCM_SRS0_PIN)       Serial.println("[RCM_SRS0] - External Pin Reset");
  if (RCM_SRS0 & RCM_SRS0_WDOG)      Serial.println("[RCM_SRS0] - Watchdog(COP) Reset");
  if (RCM_SRS0 & RCM_SRS0_LOC)       Serial.println("[RCM_SRS0] - Loss of External Clock Reset");
  if (RCM_SRS0 & RCM_SRS0_LOL)       Serial.println("[RCM_SRS0] - Loss of Lock in PLL Reset");
  if (RCM_SRS0 & RCM_SRS0_LVD)       Serial.println("[RCM_SRS0] - Low-voltage Detect Reset");
#endif
}


// retrieve battery voltage
float read_voltage(int pin)
{
  static float voltage_divider = (float)VOLTAGE_R2 / (VOLTAGE_R1 + VOLTAGE_R2);
  float result = ((float)analogRead(pin) / 1023 * 1.2 / voltage_divider);
  return result;
}

// retrieve temperature
float getTemp() {
  byte i;
  byte data[12];
  byte addr[8];
  byte type_s;
  float celsius, fahrenheit;

  ds.reset();
  ds.select(addr);
  ds.reset_search();

  if ( !ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
#ifdef ENABLE_DEBUG
    Serial.println("no more sensors on chain");  // or old DS1820
#endif
    return -1000;
  }


  if ( addr[0] != 0x10 && addr[0] != 0x28) {
#ifdef ENABLE_DEBUG
    Serial.print("Device is not recognized");
#endif
    return -1000;
  }
  switch (addr[0]) {
  case 0x10:
#ifdef ENABLE_DEBUG
    Serial.println("  Chip = DS18S20");  // or old DS1820
#endif
    type_s = 1;
    break;
  case 0x28:
#ifdef ENABLE_DEBUG
    Serial.println("  Chip = DS18B20");
#endif
    type_s = 0;
    break;
  case 0x22:
#ifdef ENABLE_DEBUG
    Serial.println("  Chip = DS1822");
#endif
    type_s = 0;
    break;
  default:
#ifdef ENABLE_DEBUG
    Serial.println("Device is not a DS18x20 family device.");
#endif
    return -1000;
  }


  ds.reset();
  ds.select(addr);
  ds.write(0x44, 0); // start conversion, with parasite power on at the end

  delay(750);

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
      celsius = (float)raw / 16.0;
      fahrenheit = celsius * 1.8 + 32.0;
#ifdef ENABLE_DEBUG
      Serial.print("  Temperature = ");
      Serial.print(celsius);
      Serial.print(" Celsius, ");
      Serial.print(fahrenheit);
      Serial.println(" Fahrenheit");
#endif
      float temperature = celsius;
      return temperature;
    }
  } else {
    ds.reset_search();

    byte MSB = data[1];
    byte LSB = data[0];

    float tempRead = ((MSB << 8) | LSB); //using two's compliment
    float temperature = tempRead / 16;
#ifdef ENABLE_DEBUG
    Serial.print("  Temperature = ");
    Serial.println(temperature);
#endif
    return temperature;
  }
  return 0;
}



// Red blink routine
void red_led_blink() {
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)blinkinterval) {
    digitalWrite(red_ledPin, HIGH);
  }
  digitalWrite(red_ledPin, LOW);
}


// Green blink routine
void green_led_blink() {
  previousMillis = millis() ;
  while ((unsigned long)(millis() - (unsigned long)previousMillis) <= (unsigned long)blinkinterval) {
    digitalWrite(green_ledPin, HIGH);
  }
  digitalWrite(green_ledPin, LOW);
}




void printDouble( double val, unsigned int precision) {
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

#ifdef ENABLE_DEBUG
  Serial.print (int(val));  //prints the int part
  Serial.print("."); // print the decimal point
#endif
  unsigned int frac;
  if (val >= 0)
    frac = (val - int(val)) * precision;
  else
    frac = (int(val) - val ) * precision;
  int frac1 = frac;
  while ( frac1 /= 10 )
    precision /= 10;
  precision /= 10;
  while (  precision /= 10)
#ifdef ENABLE_DEBUG
    Serial.print("0");
  Serial.println(frac, DEC) ;
#endif
}


void WeeklyRestart() {
  Serial.println("Alarm: - Weekly restart");
  //alarm peep 3 times before shut down
  int i = 0;
  while (i < 3) {
    digitalWrite(28, HIGH);
    pinMode(28, OUTPUT);
    delay(250);
    pinMode(28, INPUT);
    delay(250);
    i++;
  }
  delay(3000);

#if ENABLE_3G
  a3gs.end();
  a3gs.shutdown();
#endif
  CPU_RESTART;
}

#if ENABLE_3G
void DailyRTC() {
  Serial.println("Alarm: - Daily RTC");
  //alarm peep 2 times
  int i = 0;
  while (i < 3) {
    digitalWrite(28, HIGH);
    pinMode(28, OUTPUT);
    delay(250);
    pinMode(28, INPUT);
    delay(250);
    i++;
  }
  if (a3gs.getTime2(seconds) == 0) {
    setTime(seconds);
    adjustTime(-32400);
    Teensy3Clock.set(now());
  }
}
#endif



void eepromclear() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
#ifdef ENABLE_DEBUG
  Serial.println ("Finished clearing eeprom");
#endif
  lcd.setCursor(0, 1);
  lcd.print("Finished clearing");
  delay(1000);
  CPU_RESTART;
}



/**************************************************************************/
//RTC setup
/**************************************************************************/

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

/*-------- NTP code ----------*/
#if ENABLE_ETHERNET


time_t getNtpTime()
{
  ntpcount++;
  if (getTimeStamp) {
    while (Udp.parsePacket() > 0) ; // discard any previously received packets
#ifdef ENABLE_DEBUG
    Serial.println("Transmit NTP Request");
    Serial.print("timeServer =");
    Serial.println(timeServer);
#endif

    uint32_t beginWait = millis();
    while (millis() - beginWait < 300000) {
      sendNTPpacket(timeServer);
      while (millis() - beginWait < 15000) {
        int size = Udp.parsePacket();
        if (size >= NTP_PACKET_SIZE) {
#ifdef ENABLE_DEBUG
          Serial.println("Receive NTP Response");
#endif
          Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
          unsigned long secsSince1900;
          // convert four bytes starting at location 40 to a long integer
          secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
          secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
          secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
          secsSince1900 |= (unsigned long)packetBuffer[43];
          return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
      }
    }
#ifdef ENABLE_DEBUG
    Serial.println("No NTP Response :-(");
#endif
    lcd.setCursor(0, 3);
    lcd.print("Timeserver no reply");
    return 0; // return 0 if unable to get the time
  }
  return 0;
}

// send an NTP request to the time server at the given address

unsigned long sendNTPpacket(char* address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
  return (0);
}
#endif

#if ENABLE_ETHERNET
unsigned long GetMeasurementReply(unsigned long Timeout) {
  replysub = "";
  replyserver1 = "";
  unsigned long lastTime = millis();
  while (client.available() == 0 && millis() - lastTime < Timeout) delay(10);
  while (client.available() && millis() - lastTime < Timeout) {
    character = client.read();
    replyserver1.concat(character);
    lastTime = millis();
  }
  if (replyserver1.indexOf("\"id\":") != -1) {
    int val = replyserver1.indexOf("\"id\":");
    int valend = val + 13;
    replysub = replyserver1.substring((val + 5), valend);
    return replysub.toInt();
  } else {
    return (0);
  }
}
#endif


