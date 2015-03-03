/*
  nGeigie.ino

V2.3.6  sending dual ok, pulse count only
V2.3.8  chksum added to sd sensor 2..
V2.3.9  display messages changed 
V2.4.0  fixed cpm display
 */
 
 
#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <limits.h>
#include "board_specific_settings.h"
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "nGeigie3GSetup.h"
#include "nGeigie3GDebug.h"
#include <Time.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27  // Define I2C Address where the PCF8574A is for LCD2004 form http://www.sainsmart.com
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

int n = 1;

LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

#define ENABLE_DEBUG 
#define LINE_SZ 80
#define OLINE_SZ 250


static char json_buf[LINE_SZ];
static char json_buf2[LINE_SZ];
static devctrl_t ctrl;
static char obuf[OLINE_SZ];
static char buf[LINE_SZ];
static char buf2[LINE_SZ];
static char lat_buf[16];
static char lon_buf[16];
static char VERSION[] = "V2.4.0";
const char *server = "107.161.164.163";
const int port = 80;
const int interruptMode = RISING;
const int updateIntervalInMinutes = 1;
//char res[a3gsMAX_RESULT_LENGTH+1];
int MAX_FAILED_CONNS = 3;
int len;
int len2;
unsigned long elapsedTime(unsigned long startTime);
unsigned long int uSv;
char timestamp[19];
char lat[8];
char lon[9];
char lat_lon_nmea[25];
unsigned char state;
int conn_fail_cnt;
int NORMAL = 0;
int RESET = 1;
boolean pluse1_sign;
boolean pluse2_sign;

//WDT etup init

#define RCM_SRS0_WAKEUP                     0x01
#define RCM_SRS0_LVD                        0x02
#define RCM_SRS0_LOC                        0x04
#define RCM_SRS0_LOL                        0x08
#define RCM_SRS0_WDOG                       0x20
#define RCM_SRS0_PIN                        0x40
#define RCM_SRS0_POR                        0x80
#define RCM_SRS1_LOCKUP                     0x02
#define RCM_SRS1_SW                         0x04
#define RCM_SRS1_MDM_AP                     0x08
#define RCM_SRS1_SACKERR                    0x20

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

// OpenLog Settings --------------------------------------------------------------
//Setup sdcard from openlog for serial2
SoftwareSerial OpenLog =  SoftwareSerial(0, 1);
static const int resetOpenLog = 3;
#define OPENLOG_RETRY 500
bool openlog_ready = false;
char logfile_name[13];  // placeholder for filename
bool logfile_ready = false;

//static void setupOpenLog();
static bool loadConfig(char *fileName);
//static void createFile(char *fileName);


// generate checksum for log format
byte len1, chk;
byte len3, chk2;
char checksum(char *s, int N)
{
    int i = 0;
    char chk = s[0];

    for (i=1; i < N; i++)
        chk ^= s[i];

    return chk;
}

char checksum2(char *s, int N)
{
    int i = 0;
    char chk2 = s[0];

    for (i=1; i < N; i++)
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

// nGeigie Settings --------------------------------------------------------------
static ConfigType config;
nGeigieSetup ngeigieSetup(OpenLog, config, obuf, OLINE_SZ);

void onPulse()
{
    counts_per_sample++; 

}
void onPulse2()
{
    counts_per_sample2++;

}

/**************************************************************************/
// Setup
/**************************************************************************/

void setup() {  
	
  //print last reset message and setup the patting of the dog
   // delay(100);
   // printResetType();
   wdTimer.begin(KickDog, 10000000); // patt the dog every 10sec  
    
    //button reset

        pinMode(20, INPUT_PULLUP);
        attachInterrupt(20, onReset, interruptMode);
       
    // openlog setup 
    Serial.begin(9600);
    OpenLog.begin(9600);

	// init command line parser
      Serial.begin(9600);

  
        // set brightnes
              lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
              lcd.setBacklight(125);
	//set up the LCD's number of columns and rows: 
		  lcd.begin(20, 4);
	
	// Print a message to the LCD.
		  lcd.clear();
		  lcd.print(F("nGeigie"));
		  delay(3000);
		  lcd.setCursor(0, 1);
		  lcd.print(VERSION);


    // Load EEPROM settings
    ngeigieSetup.initialize();

   
  //Openlog setup
    OpenLog.begin(9600);
    setupOpenLog();
    if (openlog_ready) {
        ngeigieSetup.loadFromFile("NGEIGIE.TXT");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print ("loading setup");
        Serial.println();
        Serial.println("loading setup");
    }
    if (!openlog_ready) {
      lcd.setCursor(0, 3);
      lcd.print("No SD card.. ");
      Serial.println();
      Serial.println("No SD card.. ");
    }
    
    
  //Check if Time is setup
    setSyncProvider(getTeensy3Time);
    if (timeStatus()!= timeSet) {
        Serial.println("Unable to sync with the RTC");
        lcd.setCursor(0, 3);
        lcd.print ("Please setup time");
        sprintf_P(logfile_name, PSTR("%04d1234.log"),config.user_id);
        if (openlog_ready) {
            lcd.clear();
            lcd.setCursor(0, 1);
  	    lcd.print("Log=");
            lcd.println(logfile_name);
  	    lcd.println("Local logging only");
        }
      } else {
          Serial.println("RTC has set the system time for GMT");
	       sprintf_P(logfile_name, PSTR("%04d%02d%02d.log"),config.user_id, month(), day());
  		if (openlog_ready) {
                    lcd.print("Log=");
      		    lcd.println(logfile_name);
          }
      }  
      
      //display time
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Time (GMT):");
          printDigits(hour());
          lcd.print(":");
       	  printDigits(minute());
          lcd.setCursor(0, 1);
          lcd.print("Date:");
          lcd.print(month());
          lcd.print("-");
          lcd.print(day());
          
          Serial.print("Time (GMT):");
          printDigitsSerial(hour());
          Serial.print(F(":"));
       	  printDigitsSerial(minute());
          Serial.println("");
          Serial.print("Date:");
          Serial.print(month());
          Serial.print("-");
          Serial.println(day());
                
   
    //Gateways setup to be done
    //read for SDcard gateways 
    //store value in array
    //select randomly for total sserver
       delay(5000);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("G1=");
       lcd.print(config.gw1);
       lcd.setCursor(0, 1);
       lcd.print("G2=");
       lcd.print(config.gw2);
       Serial.print("Gateway1=");
       Serial.println(config.gw1);
       Serial.print("Gateway2=");
       Serial.println(config.gw2);
       Serial.print("APIkey=");
       Serial.println(config.api_key);    
    
    //
    // SENSOR 1 setup
    if (config.sensor1_enabled) {
        //LND_712 conversionCoefficient = 0.0083;
        //LND 7317 conversionCoefficient = 0.0029;
        conversionCoefficient = 1/config.sensor1_cpm_factor; // 0.0029;
        //Pulse1 comes in at D4
        lcd.clear();
        lcd.setCursor(0, 0);
        pinMode(14, INPUT);
        attachInterrupt(14, onPulse, interruptMode);
        lcd.print("CMPF1=");
        lcd.print(config.sensor1_cpm_factor);
        Serial.print("CMPF1=");
        Serial.println(config.sensor1_cpm_factor); 
    }
    
    // SENSOR 2 setup
    
     if (config.sensor2_enabled) {
        // LND_712 conversionCoefficient = 0.0083;
        // LND 7317 conversionCoefficient = 0.0029;
        conversionCoefficient2 = 1/config.sensor2_cpm_factor; // 0.0029;
        //Pulse2 comes in at D15
        pinMode(15, INPUT);
        attachInterrupt(15, onPulse2, interruptMode);
        lcd.setCursor(0, 1);
        lcd.print("CMPF2=");
        lcd.print(config.sensor2_cpm_factor);
        Serial.print("CMPF2=");
        Serial.println(config.sensor2_cpm_factor);
        
    }
    


    //Display User IDs
        delay(5000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ID:");
        lcd.print(config.user_id);
        lcd.print(" ID2:");
        lcd.print(config.user_id2);
        lcd.setCursor(0, 1);
        Serial.print("ID:");
        Serial.print(config.user_id);
        Serial.print(" ID2:");
        Serial.println(config.user_id2);
        
     //Display Lon/Lat
        lcd.print("LAT:");
        lcd.print(config.latitude);
        lcd.setCursor(0, 2);
        lcd.print("LON:");
        lcd.print(config.longitude);
        Serial.print("LAT:");
        Serial.print(config.latitude);
        lcd.setCursor(0, 3);
        Serial.print("LON:");
        Serial.println(config.longitude);
           

    updateIntervalInMillis = updateIntervalInMinutes * 300000;                  // update time in ms
    //updateIntervalInMillis = updateIntervalInMinutes * 6000;                  // update time in ms
    unsigned long now1 = millis();
    nextExecuteMillis = now1 + updateIntervalInMillis;

    //get time
  
    if (openlog_ready) {
        logfile_ready = true;
        createFile(logfile_name);
    }



/**************************************************************************/
// Print out the current device ID
/**************************************************************************/

	// Initiate a DHCP session
	//Serial.println(F("Getting an IP address..."));
        if (Ethernet.begin(macAddress) == 0)
	{
       		Serial.println(F("Failed DHCP"));
  // DHCP failed, so use a fixed IP address:
        	Ethernet.begin(macAddress, localIP);
	}
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("local_IP:"));
	lcd.setCursor(0, 1);
	lcd.print(Ethernet.localIP());
        Serial.print("Local IP:");
        Serial.println(Ethernet.localIP());	
        
	delay(5000);
	Serial.println(F("setup OK."));	
	lcd.setCursor(0, 2);
	lcd.print("setup finished");
	lcd.setCursor(0, 3);
	lcd.print("no errors");
	delay(5000);
	
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
  double lat_min = (lat_f - lat_d)*60.;
  
  char lat_min_str[9];
  dtostrf(lat_min, 2, 4, lat_min_str);
  
  char EW = (lon_f >= 0) ? 'E' : 'W';
  lon_f = (lon_f >= 0) ? lon_f : -lon_f;
  int lon_d = (int)fabs(lon_f);
  double lon_min = (lon_f - lon_d)*60.;
  
  char lon_min_str[9];
  dtostrf(lon_min, 2, 4, lon_min_str);
  
  snprintf(lat_lon_nmea, 25, "%02d%s,%c,%03d%s,%c", lat_d, lat_min_str, NS, lon_d, lon_min_str, EW);

}
/**************************************************************************/
// display digits
/**************************************************************************/

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  if(digits < 10)
    lcd.print('0');
    lcd.print(digits);
}

void printDigitsSerial(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  if(digits < 10)
    Serial.print('0');
    Serial.print(digits);
}	
//**************************************************************************/
/*!
//  On each falling edge of the Geiger counter's output,
//  increment the counter and signal an event. The event
//  can be used to do things like pulse a buzzer or flash an LED
*/
/**************************************************************************/


void SendDataToServer(float CPM,float CPM2){ 

// Convert from cpm to µSv/h with the pre-defined coefficient

    float uSv = CPM * conversionCoefficient;                   // convert CPM to Micro Sieverts Per Hour
    char CPM_string[16];
    dtostrf(CPM, 0, 0, CPM_string);
     Serial.println(CPM_string);
    float uSv2 = CPM2 * conversionCoefficient2;                   // convert CPM to Micro Sieverts Per Hour
    char CPM2_string[16];
    dtostrf(CPM2, 0, 0, CPM2_string);

    //display geiger info
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("1:");
      lcd.print(uSv);
      lcd.print("uSv/h");     
      lcd.print(" CPM");
      lcd.print(CPM_string);      
      lcd.setCursor(0,1);
      lcd.print("2:");
      lcd.print(uSv2);
      lcd.print("uSv/h");
      lcd.print(" CPM");
      lcd.print(CPM2_string); 
  
	if (client.connected())
	{
		Serial.println(F("Disconnecting"));
		client.stop();
	}

	// Try to connect to the server
	Serial.println(F("Connecting"));
	if (client.connect(serverIP, 80))
	{
		Serial.println(F("Connected"));
		lastConnectionTime = millis();

		// clear the connection fail count if we have at least one successful connection
		//ctrl.conn_fail_cnt = 0;
	}
	else
	{
     	   ctrl.conn_fail_cnt++;
           lcd.setCursor(0,3);
           lcd.print("no connect retry=");
           lcd.print(ctrl.conn_fail_cnt);
		if (ctrl.conn_fail_cnt >= MAX_FAILED_CONNS)
		{
                CPU_RESTART;
		}
		lastConnectionTime = millis();
		return;
	}


    // prepare the log entry

//	memset(json_buf, 0, LINE_SZ);
	sprintf_P(json_buf, PSTR("{\"longitude\":\"%s\",\"latitude\":\"%s\",\"device_id\":\"%d\",\"value\":\"%s\",\"unit\":\"cpm\"}"),  \
	              config.latitude, \
	              config.longitude, \
	              config.user_id,  \
	              CPM_string);

	int len = strlen(json_buf);
	json_buf[len] = '\0';
	Serial.println(json_buf);

	client.print("POST /scripts/indextest.php?api_key=");
	client.print(config.api_key);
	client.println(F(" HTTP/1.1"));
	client.println(F("Accept: application/json"));
	client.println(F("Host: 107.161.164.163"));
	client.print(F("Content-Length: "));
	client.println(strlen(json_buf));
	client.println(F("Content-Type: application/json"));
	client.println();
	client.println(json_buf);
	Serial.println(F("Disconnecting"));
client.stop();

//send second sensor
      Serial.println(F("Connecting"));
	if (client.connect(serverIP, 80))
	{
		Serial.println("connected");
		lastConnectionTime = millis();

		// clear the connection fail count if we have at least one successful connection
		//ctrl.conn_fail_cnt = 0;
	}
	else
	{
     	   ctrl.conn_fail_cnt++;
           lcd.setCursor(0,3);
           lcd.print("no connect retry=");
           lcd.print(ctrl.conn_fail_cnt);
		if (ctrl.conn_fail_cnt >= MAX_FAILED_CONNS)
		{
                CPU_RESTART;
		}
		lastConnectionTime = millis();
		return;
	}

    // prepare the log entry for sensor 2

	memset(json_buf2, 0, LINE_SZ);
	sprintf_P(json_buf2, PSTR("{\"longitude\":\"%s\",\"latitude\":\"%s\",\"device_id\":\"%d\",\"value\":\"%s\",\"unit\":\"cpm\"}"),  \
	              config.longitude, \
	              config.latitude, \
	              config.user_id2,  \
	              CPM2_string);

	int len2 = strlen(json_buf2);
	json_buf2[len2] = '\0';
	Serial.println(json_buf2);

	client.print("POST /scripts/indextest.php?api_key=");
	client.print(config.api_key);
	client.println(F(" HTTP/1.1"));
	client.println(F("Accept: application/json"));
	client.println(F("Host: 107.161.164.163"));
	client.print(F("Content-Length: "));
	client.println(strlen(json_buf2));
	client.println(F("Content-Type: application/json"));
	client.println();
	client.println(json_buf2);
	Serial.println(F("Disconnecting"));
client.stop();
	
//convert time in correct format
      //memset(timestamp, 0, LINE_SZ);
        sprintf_P(timestamp, PSTR("%02d-%02d-%02dT%02d:%02d:%02dZ"),  \
					year(), month(), day(),  \
                    hour(), minute(), second());


// convert degree to NMAE
		deg2nmae (config.latitude,config.longitude, lat_lon_nmea);

     //sensor 1 sd card string setup
          memset(buf, 0, LINE_SZ);
          sprintf_P(buf, PSTR("$BMRDD,%d,%s,,,%s,A,%s,1,A,,"),  \
                    config.user_id, \
                    timestamp, \
                    CPM_string, \
                    lat_lon_nmea);
  
          len = strlen(buf);
          buf[len] = '\0';
        
        // generate checksum
          chk = checksum(buf+1, len);
        
        // add checksum to end of line before sending
          if (chk < 16)
              sprintf_P(buf + len, PSTR("*0%X"), (int)chk);
          else
              sprintf_P(buf + len, PSTR("*%X"), (int)chk);
          Serial.println(buf);
       
     //sensor 2 sd card string setup
          memset(buf2, 0, LINE_SZ);     
          sprintf_P(buf2, PSTR("$BMRDD,%d,%s,,,%s,A,%s,1,A,,"),  \
                    config.user_id2, \
                    timestamp, \
                    CPM2_string, \
                    lat_lon_nmea);
  
          len2 = strlen(buf2);
          buf2[len2] = '\0';

        // generate checksum
          chk2 = checksum(buf2+1, len2);
        
        
        // add checksum to end of line before sending
          if (chk2 < 16)
              sprintf_P(buf2 + len2, PSTR("*0%X"), (int)chk2);
          else
              sprintf_P(buf2 + len2, PSTR("*%X"), (int)chk2);
              
          Serial.println(buf2);    

        //write to sd card sensor 1 info
          OpenLog.println(buf);
        //write to sd card sensor 2 info
          OpenLog.println(buf2);

       
    
    // report to LCD 
    
    lcd.setCursor(0, 2);
          lcd.print(month());
          lcd.print("-");
          lcd.print(day());
          lcd.print("   ");
          printDigits(hour());
          lcd.print(":");
       	  printDigits(minute());

          
          
          lcd.setCursor(0, 3);
	  //lcd.print(F("Sent OK"));
	  client.stop();

      lastConnectionTime = millis();
}



/**************************************************************************/
// Main Loop
/**************************************************************************/
void loop() {

    // Main Loop
      if (elapsedTime(lastConnectionTime) < updateIntervalInMillis)
      {
          return;
      }
  
      float CPM = (float)counts_per_sample / (float)updateIntervalInMinutes/5;
      counts_per_sample = 0;
      float CPM2 = (float)counts_per_sample2 / (float)updateIntervalInMinutes/5;
      counts_per_sample2 = 0;
      
      SendDataToServer(CPM,CPM2);
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


void GetFirmwareVersion()
{
	printf_P(PSTR("Firmware_ver:\t%s\n"), VERSION);
}

/**************************************************************************/
// OpenLog code
/**************************************************************************/

/* wait for openlog prompt */
bool waitOpenLog(bool commandMode) {
    int safeguard = 0;
    bool result = false;

    while(safeguard < OPENLOG_RETRY) {
        safeguard++;
        if(OpenLog.available())
            if(OpenLog.read() == (commandMode ? '>' : '<')) break;
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
    int safeguard = 0;

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
//RTC setup
/**************************************************************************/

    time_t getTeensy3Time()
    {
      return Teensy3Clock.get();
    }

/**************************************************************************/
//WDT setup
/**************************************************************************/

    void KickDog() {
      Serial.println("Patting the dog!");
      //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      noInterrupts();
      WDOG_REFRESH = 0xA602;
      WDOG_REFRESH = 0xB480;
      interrupts();
    }

//WDT error messages
    void printResetType() {
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
    }

//WDT hook setup
      #ifdef __cplusplus
      extern "C" {
      #endif
        void startup_early_hook() {
          WDOG_TOVALL = (10000); // The next 2 lines sets the time-out value. This is the value that the watchdog timer compare itself to.
          WDOG_TOVALH = 0;
          WDOG_STCTRLH = (WDOG_STCTRLH_ALLOWUPDATE | WDOG_STCTRLH_WDOGEN | WDOG_STCTRLH_WAITEN | WDOG_STCTRLH_STOPEN); // Enable WDG
          //WDOG_PRESC = 0; // prescaler 
        }
      #ifdef __cplusplus
      }
      #endif

 
