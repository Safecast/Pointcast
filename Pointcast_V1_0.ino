/*
  Pointcast.ino

2015-04-05 V2.4.9 delay for switching off LEDs
2015-04-07 V2.6.0 merged code with 3G
2015-04-07 V2.6.1 beeper setup and code cleaning　(need jumper from D10 in arduino shield (is pin D27)to A3)
2015-04-08 V2.6.3 setup for measurung voltage on A13
2015-04-08 V2.6.4 made switch for sending to dev or API
2015-04-14 V2.6.6 added heart beat on green LED and reading setup for files
2015-04-19 V2.6.7 added setup files now 1024 byte possible . added setup variables 
2015-04-20 V2.6.8 added device ID, startup screen change, header for file format changed to NGRDD
2015-04-23 V2.6.9 added second line to SDcard logging for added status NNXSTS, added menus 
2015-04-23 V2.7.0 renamed Pointcast
2015-04-28 V2.7.1 moved  startup 3G into send string, battery voltage report corrected for Teensy.
2015-04-30 V2.7.2 Added temperature setup for DS18B20 (disabled at the moment) setup screens
2015-04-05 V2.7.3 Added Joy stick setup. Added Height. Fixed lot/lan sending information on Ethernet
contact rob@yr-design.biz
 */
 
 /**************************************************************************/
// Init
/**************************************************************************/
 
#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EEPROM.h>
#include <limits.h>
#include <SoftwareSerial.h>
#include <Time.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>
#include <i2c_t3.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include "a3gim.h"
#include "PointcastSetup.h"
#include "PointcastDebug.h"

//setup LCD I2C
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
int backlightPin = 2;

//setup Power detection
#define VOLTAGE_PIN A13
#define VOLTAGE_R1 150000
#define VOLTAGE_R2 10000

//setup Onewire for temp sensor
#include <OneWire.h>
OneWire  ds(A12);  // on pin 10 (a 4.7K resistor is necessary)



#define ENABLE_DEBUG 
#define LINE_SZ 128
// SENT_SZ is used for sending data for 3G
#define SENT_SZ 120
//OLINE_SZ is used for OpenLog buffers
#define OLINE_SZ 1024
//GATEWAY_sz is array for gateways
#define GATEWAY_SZ 2

static char obuf[OLINE_SZ];
static char buf[LINE_SZ];
static char buf2[LINE_SZ];
static char lat_buf[16];
static char lon_buf[16];


// OpenLog Settings --------------------------------------------------------------
    SoftwareSerial OpenLog =  SoftwareSerial(0, 1);
    static const int resetOpenLog = 3;
    #define OPENLOG_RETRY 500
    bool openlog_ready = false;
    char logfile_name[13];  // placeholder for filename
    bool logfile_ready = false;

    //static void setupOpenLog();
    static bool loadConfig(char *fileName);
    //static void createFile(char *fileName);

    static ConfigType config;
    PointcastSetup PointcastSetup(OpenLog, config, obuf, OLINE_SZ);


//static
      static char VERSION[] = "V2.7.3";

    #if ENABLE_3G
    static char path[LINE_SZ];
    static char path2[LINE_SZ];
    char datedisplay[8];
    char coordinate[16];
    #endif

    #if ENABLE_ETHERNET
    static char json_buf[SENT_SZ];
    static char json_buf2[SENT_SZ];
    #endif


//Struct setup
typedef struct
{
    unsigned char state;
    unsigned char conn_fail_cnt;
} devctrl_t;
static devctrl_t ctrl;

//const
    const char *server = "107.161.164.163";
    const int port = 80;
    const int interruptMode = FALLING;
    const int updateIntervalInMinutes = 1;

    #if ENABLE_ETHERNET
    //ethernet
    byte macAddress[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xE0, 0x5C };
    EthernetClient client;
    IPAddress localIP (192, 168, 100, 40);	
    IPAddress serverIP(107, 161, 164, 163 ); 
    int resetPin = A1;   //
    int ethernet_powerdonwPin = 7;
    
    #endif

//int
    int MAX_FAILED_CONNS = 3;
    int len;
    int len2;
    int conn_fail_cnt;
    int NORMAL = 0;
    int RESET = 1;
//long
    unsigned long elapsedTime(unsigned long startTime);

//char
    char timestamp[19];
    char lat[8];
    char lon[9];
    char lat_lon_nmea[25];
    unsigned char state;

    #if ENABLE_3G
    char res[a3gsMAX_RESULT_LENGTH+1];
    #endif


//joystick pins setup
    #define joy_down_pin  19
    #define joy_left_pin  20
    #define joy_up_pin  21
    #define joy_center_pin  22
    #define joy_right_pin  23
    int joy_down = digitalRead(joy_down_pin);
    int joy_left = digitalRead(joy_down_pin);
    int joy_up = digitalRead(joy_down_pin);
    int joy_center = digitalRead(joy_down_pin);
    int joy_right = digitalRead(joy_down_pin);
    
    
    //Joystick setup
        void joy_stick()
        {
            pinMode(joy_down_pin, INPUT_PULLUP);
            pinMode(joy_left_pin, INPUT_PULLUP);
            pinMode(joy_up_pin, INPUT_PULLUP);
            pinMode(joy_center_pin, INPUT_PULLUP);
            pinMode(joy_right_pin, INPUT_PULLUP);
      
        }
    

//WDT setup init

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


// generate checksums for log format
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


//Pulse counters
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

  
   // openlog setup 
          Serial.begin(9600);
          OpenLog.begin(9600);
          
   
   //print last reset message and setup the patting of the dog
         delay(100);
         printResetType();
         
   //start WDT	
         wdTimer.begin(KickDog, 10000000); // patt the dog every 10sec  
         
   // Load EEPROM settings
         PointcastSetup.initialize();

         
   //beep 
       tone(28, 600, 200);
    
    //button reset
          pinMode(27, INPUT_PULLUP);
          attachInterrupt(27, onReset, interruptMode);
  
    // set brightnes
          lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
          lcd.setBacklight(125);
          
    //set up the LCD's number of columns and rows: 
          lcd.begin(20, 4);

	
/**************************************************************************/
// Start screen
/**************************************************************************/


    // Print startup message to the LCD.
	   lcd.clear();
	   lcd.print("Pointcast V1.0");
           lcd.setCursor(0, 1);
           lcd.print("Firmware :");
           lcd.print(VERSION);
           lcd.setCursor(0, 2);
           lcd.print("Device ID:");
           lcd.print(config.devid);
           lcd.setCursor(0, 3);
           lcd.print("http://safecast.org");         


    //LED1(green) setup
      pinMode(31, OUTPUT);
      digitalWrite(31, HIGH);
      
   //LED2(red) setup
     pinMode(26, OUTPUT);
     digitalWrite(26, HIGH);
     
    // LED on delay
      delay (3000); 
     
    //LED off
      digitalWrite(26, LOW);
      digitalWrite(31, LOW); 


/**************************************************************************/
// System Screen
/**************************************************************************/
            
     // read battery     
       float battery =((read_voltage(VOLTAGE_PIN)));
       Serial.println("Battery Voltage =");
       Serial.print(battery);
       Serial.println("V");
       
      //get temperature 
         float temperature = 18.3;
        //float temperature = getTemp();
        Serial.print("Temperature =");
        //Serial.println(temperature);
        Serial.println("Celsius");
       
      
      
    // Print system message to the LCD.
	   lcd.clear();
	   lcd.print("System");
           lcd.setCursor(0, 1);
           lcd.print("Power:");
           if (battery < 4.5) {
                lcd.print("BAT");;
              } else {
                lcd.print("EXT");
              }
                       
           lcd.setCursor(0, 2);
           lcd.print("Bat:");
           lcd.print(battery);
           lcd.print("V");
           lcd.setCursor(0, 3);
           lcd.print("Tmp:"); 
           //lcd.println(temperature);
           lcd.print("C");

           
           

    
    
/**************************************************************************/
// Time Screen
/**************************************************************************/  

    
  //Check if Time is setup
    setSyncProvider(getTeensy3Time);
    if (timeStatus()!= timeSet) {
        Serial.println("Unable to sync with the RTC");
        sprintf_P(logfile_name, PSTR("%04d1234.log"),config.user_id);

      } else {
        Serial.println("RTC has set the system time for GMT");
	sprintf_P(logfile_name, PSTR("%04d%02d%02d.log"),config.user_id, month(), day());
      }  
      
      //display time
          delay (3000); 
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("TIME (PRESS TO SET)");
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


          //serial info print
              Serial.print("Time (GMT):");
              printDigitsSerial(hour());
              Serial.print(F(":"));
              printDigitsSerial(minute());
              Serial.println("");
              Serial.print("Date:");
              Serial.print(month());
              Serial.print("-");
              Serial.println(day());
          
          
/**************************************************************************/
// SDcard Screen
/**************************************************************************/  
        
    //Openlog setup
        OpenLog.begin(9600);
        setupOpenLog();
          if (openlog_ready) {
              Serial.println();
              Serial.println("loading setup");
              PointcastSetup.loadFromFile("PCAST.TXT");
              Serial.println();
              Serial.println("loading Network setup");
              PointcastSetup.loadFromFile("NETWORKS.TXT");
              Serial.println();
              Serial.println("loading sensors setup");
              PointcastSetup.loadFromFile("SENSORS.TXT");
          }
          if (!openlog_ready) {
              lcd.setCursor(0, 3);
              lcd.print("No SD card.. ");
              Serial.println();
              Serial.println("No SD card.. ");
          }
      
      
     // printout selected interface
        Serial.print("Device ID =");
        Serial.println(config.devid);
        Serial.print("Interface =");
        Serial.println(config.intf);
        Serial.print("dev =");
        Serial.println(config.dev);
        Serial.print("tws =");
        Serial.println(config.tws);
        Serial.print("alt =");
        Serial.println(config.alt);
        Serial.print("autow =");
        Serial.println(config.autow);
        Serial.print("alm =");
        Serial.println(config.alm);
        Serial.print("ssid =");
        Serial.println(config.ssid);
        Serial.print("tz =");
        Serial.println(config.tz);
        Serial.print("pwd =");
        Serial.println(config.pwd);
        Serial.print("gwn =");
        Serial.println(config.gwn);
        Serial.print("s1i =");
        Serial.println(config.s1i);
        Serial.print("s2i =");
        Serial.println(config.s2i);
        Serial.print("aux =");
        Serial.println(config.aux);

        //display information
          delay (3000); 
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("SDCARD");
          lcd.setCursor(0, 1);
          lcd.print("POINTCAST:");
          lcd.setCursor(0, 2);
          lcd.print("SENSORS:");
          lcd.setCursor(0, 3);
          lcd.print("NETWORK:");
         

/**************************************************************************/
// POINTCAST Screen
/**************************************************************************/  
        //display information
          delay (3000); 
          lcd.clear();
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

          delay (3000); 
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("UPLOAD MODE");
          lcd.setCursor(0, 1);
          lcd.print("Adaptive: OFF");
          lcd.setCursor(0, 2);
          lcd.print("Integr Win: 300sec");         
          lcd.setCursor(0, 3);
          lcd.print("Upload Win: 300sec");   
  
/**************************************************************************/
// GPS Screen
/**************************************************************************/  
  
        delay (3000); 
        lcd.clear();
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
     
     //serial print 
        Serial.print("Lon:");
        Serial.println(config.longitude);   
        Serial.print("Lat:");
        Serial.println(config.latitude);
        Serial.print("Alt:");
        Serial.println(config.alt);  


/**************************************************************************/
// Sensors Screen
/**************************************************************************/    
        delay (3000); 
        lcd.clear();
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
        
/**************************************************************************/
// Sensors Test Screen
/**************************************************************************/  
        delay (3000); 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SENSOR TEST");
        lcd.setCursor(0, 1);
        lcd.print("S1=");
        lcd.setCursor(0, 2);        
        lcd.print("S2=");
        lcd.setCursor(0, 3);
        lcd.print("AUX=NC");

/**************************************************************************/
// API Screen
/**************************************************************************/  
        delay (3000); 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("API");
        lcd.setCursor(0, 1);
        lcd.print("S1-ID=");
        lcd.print(config.user_id); 
        lcd.setCursor(0, 2);        
        lcd.print("S1-ID=");
        lcd.print(config.user_id2);
        lcd.setCursor(0, 3);
        lcd.print("API-KEY=");
//        lcd.print(config.api_key);        



    // SENSOR 1 setup
    if (config.sensor1_enabled) {
        conversionCoefficient = 1/config.sensor1_cpm_factor; // 0.0029;
        pinMode(14, INPUT_PULLUP);
        attachInterrupt(14, onPulse, interruptMode);
        Serial.print("CMPF1=");
        Serial.println(config.sensor1_cpm_factor); 
    }
    
    // SENSOR 2 setup
    
     if (config.sensor2_enabled) {
        conversionCoefficient2 = 1/config.sensor2_cpm_factor; // 0.0029;
        pinMode(15,INPUT_PULLUP);
        attachInterrupt(15, onPulse2, interruptMode);
        Serial.print("CMPF2=");
        Serial.println(config.sensor2_cpm_factor);
        
    }
    

/**************************************************************************/
// Network Screen
/**************************************************************************/   
#if ENABLE_ETHERNET
	// Initiate a DHCP session
        if (Ethernet.begin(macAddress) == 0)
        
	{
       		Serial.println(F("Failed DHCP"));
  // DHCP failed, so use a fixed IP address:
        	Ethernet.begin(macAddress, localIP);
	}

        Serial.print("Local IP:");
        Serial.println(Ethernet.localIP());	       
	Serial.println(F("setup OK."));	

//          if (config.intf == "EN") {
                  delay (3000); 
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("NETWORK ETHER (DHCP)");
                  lcd.setCursor(0, 1);
                  lcd.print("IP:");
                  lcd.print(Ethernet.localIP()); 
                  lcd.setCursor(0, 2);        
                  lcd.print("GW:");
                  lcd.print(config.gw1);
                  lcd.setCursor(0, 3);
                  lcd.print("ID");
                      for (int i=0; i<6; ++i)
                          {
                        lcd.print(":");
                        lcd.print(macAddress[i],HEX);
                        
                        }
                  
            
//          }
#endif

#if ENABLE_3G
                  delay (3000); 
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("NETWORK 3G");
                  lcd.setCursor(0, 1);
                  lcd.print("Signal:");
                  lcd.print("[000000]"); 
                  lcd.setCursor(0, 2);        
                  lcd.print("Carrier:");
                  lcd.print("NTT Docomo");
                  lcd.setCursor(0, 3);
                  lcd.print("Phone: ");
                  lcd.print("080XXXXYYYY");
  #endif          


   
    //Gateways setup to be done
    //read for SDcard gateways 
    //store value in array
    
       //gatewaynumber=random(2);
       //Serial.print(gatewaynumber);
    
    //select randomly for total sserver
//       delay(3000);
//       lcd.clear();
//       lcd.setCursor(0, 0);
//       lcd.print("G1=");
//       lcd.print(config.gw1);
//       lcd.setCursor(0, 1);
//       lcd.print("G2=");
//       lcd.print(config.gw2);
//       Serial.print("Gateway1=");
//       Serial.println(config.gw1);
//       Serial.print("Gateway2=");
//       Serial.println(config.gw2);
//       Serial.print("APIkey=");
//       Serial.println(config.api_key);    
//        

    
    
    //setup update time in msec
        updateIntervalInMillis = updateIntervalInMinutes * 300000;                  // update time in ms
        //updateIntervalInMillis = updateIntervalInMinutes * 6000;                  // update time in ms
        unsigned long now1 = millis();
        nextExecuteMillis = now1 + updateIntervalInMillis;
     
    // create logfile name 
    if (openlog_ready) {
        logfile_ready = true;
        createFile(logfile_name);
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

  #if ENABLE_ETHERNET


// Convert from cpm to µSv/h with the pre-defined coefficient

    float uSv = CPM * conversionCoefficient;                   // convert CPM to Micro Sieverts Per Hour
    char CPM_string[16];
    dtostrf(CPM, 0, 0, CPM_string);
    float uSv2 = CPM2 * conversionCoefficient2;                   // convert CPM to Micro Sieverts Per Hour
     char CPM2_string[16];
    dtostrf(CPM2, 0, 0, CPM2_string);

    //display geiger info
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("1:");
      lcd.print(uSv);
      lcd.print("uSv/h"); 
      lcd.setCursor(0,1);    
      lcd.print(CPM_string); 
      lcd.print(" CPM");     
      lcd.setCursor(0,2);
      lcd.print("2:");
      lcd.print(uSv2);
      lcd.print("uSv/h");
      lcd.setCursor(0,3);
      lcd.print(CPM2_string); 
      lcd.print(" CPM");
      
      
	
 //send first sensor  
	if (client.connected())
	{
		Serial.println("Disconnecting");
		client.stop();
	}

	// Try to connect to the server
	Serial.println("Connecting");
	if (client.connect(serverIP, 80))
	{
		Serial.println("Connected");
		lastConnectionTime = millis();

		// clear the connection fail count if we have at least one successful connection
		ctrl.conn_fail_cnt = 0;
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

	memset(json_buf, 0, SENT_SZ);
	sprintf_P(json_buf, PSTR("{\"longitude\":\"%s\",\"latitude\":\"%s\",\"device_id\":\"%d\",\"value\":\"%s\",\"unit\":\"cpm\"}"),  \
	              config.longitude, \
	              config.latitude, \
	              config.user_id,  \
	              CPM_string);

	int len = strlen(json_buf);
	json_buf[len] = '\0';
	Serial.println(json_buf);

        #if ENABLE_DEV
        	client.print("POST /scripts/indextest.php?api_key=");
        #endif
        
        #if ENABLE_API
                client.print("POST /scripts/index.php?api_key=");
        #endif        
	client.print(config.api_key);
	client.println(" HTTP/1.1");
	client.println("Accept: application/json");
	client.print("Host:");
        client.println(serverIP);
	client.print("Content-Length: ");
	client.println(strlen(json_buf));
	client.println("Content-Type: application/json");
	client.println();
	client.println(json_buf);
	Serial.println("Disconnecting");
        client.stop();
   

      
  //send second sensor  
	if (client.connected())
	{
		Serial.println("Disconnecting");
		client.stop();
	}

	// Try to connect to the server
	Serial.println("Connecting");
	if (client.connect(serverIP, 80))
	{
		Serial.println("Connected");
		lastConnectionTime = millis();

		// clear the connection fail count if we have at least one successful connection
		ctrl.conn_fail_cnt = 0;
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
	memset(json_buf2, 0, SENT_SZ);
	sprintf_P(json_buf2, PSTR("{\"longitude\":\"%s\",\"latitude\":\"%s\",\"device_id\":\"%d\",\"value\":\"%s\",\"unit\":\"cpm\"}"),  \
	              config.longitude, \
	              config.latitude, \
	              config.user_id2,  \
	              CPM2_string);

	int len2 = strlen(json_buf2);
	json_buf2[len2] = '\0';
	Serial.println(json_buf2);


        #if ENABLE_DEV
        	client.print("POST /scripts/indextest.php?api_key=");
        #endif
        
        #if ENABLE_API
                client.print("POST /scripts/index.php?api_key=");
        #endif 
	client.print(config.api_key);
	client.println(" HTTP/1.1");
	client.println("Accept: application/json");
	client.print("Host:");
        client.println(serverIP);
	client.print("Content-Length: ");
	client.println(strlen(json_buf2));
	client.println("Content-Type: application/json");
	client.println();
	client.println(json_buf2);
	Serial.println("Disconnecting");
        client.stop();



      //convert time in correct format
        memset(timestamp, 0, LINE_SZ);
        sprintf_P(timestamp, PSTR("%02d-%02d-%02dT%02d:%02d:%02dZ"),  \
					year(), month(), day(),  \
                    hour(), minute(), second());


      // convert degree to NMAE
		deg2nmae (config.latitude,config.longitude, lat_lon_nmea);

     //sensor 1 sd card string setup
          memset(buf, 0, LINE_SZ);
          sprintf_P(buf, PSTR("$NGRDD,%d,%s,,,%s,A,%s,%d,A,,"),  \
                    config.user_id, \
                    timestamp, \
                    CPM_string, \
                    lat_lon_nmea, \
                    config.alt);
  
          len = strlen(buf);
          buf[len] = '\0';
        
        // generate checksum
          chk = checksum(buf+1, len);
        
        // add checksum to end of line before sending
          if (chk < 16)
              sprintf_P(buf + len, PSTR("*0%X"), (int)chk);
          else
              sprintf_P(buf + len, PSTR("*%X"), (int)chk);

 
 
      float battery =((read_voltage(VOLTAGE_PIN)));
      //test data temperature
      float temperature= 18.6;
 
     //add second line for addtional info
       sprintf_P(buf + len, PSTR("*%X%s$%s,%d,%d"), 
              (int)chk, \
              "\n", \
              HEADER_SENSOR,  \
              temperature, \
              battery);
 
      Serial.println(buf);
 
       
     //sensor 2 sd card string setup
          memset(buf2, 0, LINE_SZ);     
          sprintf_P(buf2, PSTR("$NGRDD,%d,%s,,,%s,A,%s,%d,A,,"),  \
                    config.user_id2, \
                    timestamp, \
                    CPM2_string, \
                    lat_lon_nmea, \
                    config.alt);
  
          len2 = strlen(buf2);
          buf2[len2] = '\0';

        // generate checksum
          chk2 = checksum(buf2+1, len2);
        
        
        // add checksum to end of line before sending
          if (chk2 < 16)
              sprintf_P(buf2 + len2, PSTR("*0%X"), (int)chk2);
          else
              sprintf_P(buf2 + len2, PSTR("*%X"), (int)chk2);
              
  
         
        //add second line for addtional info
           sprintf_P(buf + len, PSTR("*%X%s$%s,%d,%d"), 
              (int)chk, \
              "\n", \
              HEADER_SENSOR,  \
              temperature, \
              battery);
              
              
         Serial.println(buf2); 

        //write to sd card sensor 1 info
          OpenLog.println(buf);
        //write to sd card sensor 2 info
          OpenLog.println(buf2);

       
    
    // report to LCD 
    
    lcd.setCursor(15, 4);
          printDigits(hour());
          lcd.print(":");
       	  printDigits(minute());
#endif


#if ENABLE_3G
// Convert from cpm to µSv/h with the pre-defined coefficient

    float uSv = CPM * conversionCoefficient;                   // convert CPM to Micro Sieverts Per Hour
    char CPM_string[16];
    dtostrf(CPM, 0, 0, CPM_string);
    float uSv2 = CPM2 * conversionCoefficient2;                   // convert CPM to Micro Sieverts Per Hour
    char CPM2_string[16];
    dtostrf(CPM2, 0, 0, CPM2_string);

    //display geiger info
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("S1:");
      lcd.print(uSv);
      lcd.print("uSv/h");
      lcd.setCursor(0, 1);
      lcd.print("S2:");
      lcd.print(uSv2);
      lcd.print("uSv/h");

   // connect 3G
//    lcd.clear();
//    lcd.print("Starting up 3Gshield");
//    lcd.setCursor(0, 1);
//    lcd.print("counting pulses..");
    if (a3gs.start() == 0 && a3gs.begin() == 0)
           {
         }else {
           //a3gs.restart();
           lcd.setCursor(0, 0);
           lcd.print("no 3G connection ..");
       }
       

        // Create data string for sensor 1
        len = sizeof(res);
		lastConnectionTime = millis();
                  #if ENABLE_DEV
                          sprintf_P(path, PSTR("/scripts/shorttest.php?api_key=%s&lat=%s&lon=%s&cpm=%s&id=%d"),
                  #endif
                  
                  #if ENABLE_API
                          sprintf_P(path, PSTR("/scripts/short.php?api_key=%s&lat=%s&lon=%s&cpm=%s&id=%d"),
                  #endif 

                  config.api_key, \
                  config.latitude, \
                  config.longitude, \
                  CPM_string, \
                  config.user_id);
                  
                  
       // Create data string for sensor 2
                  #if ENABLE_DEV
                          sprintf_P(path2, PSTR("/scripts/shorttest.php?api_key=%s&lat=%s&lon=%s&cpm=%s&id=%d"),
                  #endif
                  
                  #if ENABLE_API
                          sprintf_P(path2, PSTR("/scripts/short.php?api_key=%s&lat=%s&lon=%s&cpm=%s&id=%d"),
                  #endif 
                  config.api_key, \
                  config.latitude, \
                  config.longitude, \
                  CPM2_string, \
                  config.user_id2);          
                  

        //convert time in correct format
        memset(timestamp, 0, LINE_SZ);
        sprintf_P(timestamp, PSTR("%02d-%02d-%02dT%02d:%02d:%02dZ"),  \
					year(), month(), day(),  \
                    hour(), minute(), second());
                    
                    
		// convert degree to NMAE
		deg2nmae (config.latitude,config.longitude, lat_lon_nmea);
     
     
       //sensor 1 sd card string setup
        memset(buf, 0, LINE_SZ);
        sprintf_P(buf, PSTR("$NGRDD,%d,%s,,,%s,A,%s,1,A,,"),  \
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
        sprintf_P(buf2, PSTR("$NGRDD,%d,%s,,,%s,A,%s,1,A,,"),  \
                  config.user_id2, \
                  timestamp, \
                  CPM2_string, \
                  lat_lon_nmea);

        len2 = strlen(buf2);
        buf2[len2] = '\0';
        //check if timestamp works
       

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

        //send to server
        if (a3gs.httpGET(server, port, path, res, len) == 0) {
	           Serial.println(F("Sent sensor 1 info to server OK!"));
            a3gs.httpGET(server, port, path2, res, len);
                   Serial.println(F("Sent sensor 2 info to server OK!"));
		  conn_fail_cnt = 0;
      
             //Display infomation 
                lcd.setCursor(0, 2);
            	lcd.print("Sent (GMT):");
                printDigits(hour());
                lcd.print(F(":"));
             	printDigits(minute());
                lcd.setCursor(0, 3);
                //lcd.print(     );
                lcd.print("CPM1:");
                lcd.print(CPM_string);
                lcd.print("  CPM2:");
                lcd.print(CPM2_string);
                         
            lastConnectionTime = millis();
        }
        else {
            
            lcd.setCursor(0,2);
            lcd.print("NC API! SDcard only");
            lastConnectionTime = millis();
            Serial.println("No connection to API!");
            Serial.println("saving to SDcard only");
            
            conn_fail_cnt++;
		if (conn_fail_cnt >= MAX_FAILED_CONNS)
		{
                      //first shut down 3G before reset
                      a3gs.end();
                      a3gs.shutdown();
		      
                      CPU_RESTART;
		}
                  lcd.setCursor(0, 2);
                  lcd.print("Retries left:");
                  lcd.print(MAX_FAILED_CONNS - conn_fail_cnt);
                  Serial.print("NC. Retries left:");
                  Serial.println(MAX_FAILED_CONNS - conn_fail_cnt);
		lastConnectionTime = millis();
		return;
        }
    //}
    //clean out the buffers
    memset(buf, 0, sizeof(buf));
    memset(path, 0, sizeof(path));
    lastConnectionTime = millis();


#endif
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
      pinMode(31, OUTPUT);
      digitalWrite(31, !digitalRead(31));
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


// retrieve battery voltage 
    float read_voltage(int pin)
    {
      static float voltage_divider = (float)VOLTAGE_R2 / (VOLTAGE_R1 + VOLTAGE_R2);
      float result = (float)analogRead(pin)/4096*10  / voltage_divider;
      return result;
    }

// retrieve temperature
float getTemp(){

      byte data[12];
      byte addr[8];
      
      if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
      }
      
      
      if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
      }
      
      ds.reset();
      ds.select(addr);
      ds.write(0x44,1); // start conversion, with parasite power on at the end
      
      byte present = ds.reset();
      ds.select(addr);
      ds.write(0xBE); // Read Scratchpad
      
      for (int i = 0; i < 9; i++) { // we need 9 bytes
      data[i] = ds.read();
      }
      
      ds.reset_search();
      
      byte MSB = data[1];
      byte LSB = data[0];
      
      float tempRead = ((MSB << 8) | LSB); //using two’s compliment
      float temperature = tempRead / 16;
      delay(1000);
      return temperature;

}
