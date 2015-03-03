// NOTE:
// Before uploading to your Arduino Ethernet board,
// please replace with your own settings



// Your API key (a public secure key is recommended) IWAKI HAWK 002
//const char *apiKey = "q1LKu7RQ8s5pmyxunnDW";
//const char *lat = "34.4825" ;
//const char *lon = "136.1633" ;
//const char *ID = "95" ;


// REPLACE WITH A PROPER MAC ADDRESS
byte macAddress[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xE0, 0x5C };

// Enter an IP address for your controller below.
// The IP address will be dependent on your local network:
EthernetClient client;
IPAddress localIP (192, 168, 100, 40);		// fallback local IP address if no DHCP
IPAddress serverIP(107, 161, 164, 163 );    // Safecast's proxy server with timestamp


// This is specific to the Uno Ethernet board
int pinSpkr = 1;	// pin number of piezo speaker
int pinLED = 0;		// pin number of event LED
int resetPin = A1;   //
int ethernet_powerdonwPin = 7;

// Update interval in minutes
//const int updateIntervalInMinutes = 1;

typedef struct
{
  unsigned short addr;
  char lat[10];
  char lon[10];
  char ID[3];
  char firmware_version[8];
} device_t;

typedef struct
{
    unsigned char state;
    unsigned char conn_fail_cnt;
} devctrl_t;



// Interrupt mode:
// * For most geiger counter modules: FALLING
// * Geiger Counter Twig by Seeed Studio: RISING
//const int interruptMode = RISING;


