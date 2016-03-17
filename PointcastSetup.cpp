/*
 Simple library to handle ngeigie configuration from file and EEPROM
*/

#include "PointcastSetup.h"
#include "PointcastDebug.h"

 PointcastSetup::PointcastSetup(SoftwareSerial &openlog, ConfigType &config,DoseType &dose,
  char * buffer, size_t buffer_size):
 mOpenlog(openlog), mConfig(config), mDose(dose), mBuffer(buffer), mBufferSize(buffer_size) {
 }

 void PointcastSetup::initialize() {


// Configuration 
  DEBUG_PRINTLN("Loading EEPROM configuration");
  memset(&mConfig, 0, sizeof(mConfig));
  EEPROM_readAnything(BMRDD_EEPROM_SETUP, mConfig);

  DEBUG_PRINTLN("Loading EEPROM configuration");

  if (mConfig.marker != BMRDD_EEPROM_MARKER) {
    DEBUG_PRINTLN("  - First time setup");
  // First run, time to set default values
    memset(&mConfig, 0, sizeof(mConfig));
    mConfig.marker = BMRDD_EEPROM_MARKER;
    EEPROM_writeAnything(BMRDD_EEPROM_SETUP, mConfig);

//#if ENABLE_EEPROM_DOSE
//    memset(&mDose, 0, sizeof(mDose));
//    EEPROM_writeAnything(BMRDD_EEPROM_DOSE, mDose);
//#endif
  }
}



void PointcastSetup::loadFromFile(char * setupFile) {
  bool config_changed = false;
  char *config_buffer, *key, *value;
  unsigned int pos, line_lenght;
  byte i, buffer_lenght;

  mOpenlog.listen();

// Send read command to OpenLog
  DEBUG_PRINT(" - read ");
  DEBUG_PRINTLN(setupFile);

  sprintf_P(mBuffer, PSTR("read %s 0 %d"), setupFile, mBufferSize);
  mOpenlog.print(mBuffer);
mOpenlog.write(13); //This is \r

while(1) {
  if(mOpenlog.available())
    if(mOpenlog.read() == '\r') break;
}

// Read config file in memory
pos = 0;
memset(mBuffer, 0, mBufferSize);
for(int timeOut = 0 ; timeOut < 1000 ; timeOut++) {
  if(mOpenlog.available()) {
    mBuffer[pos++] = mOpenlog.read();
    timeOut = 0;
  }
  delay(1);

  if(pos == mBufferSize) {
    break;
  }
}

line_lenght = pos;
pos = 0;

// Process each config file lines
while(pos < line_lenght){

  // Get a complete line
  i = 0;
  config_buffer = mBuffer + pos;
  while(mBuffer[pos++] != '\n') {
    i++;
    if(pos == mBufferSize) {
      break;
    }
  }
  buffer_lenght = i++;
  config_buffer[--i] = '\0';

  // Skip empty lines
  if(config_buffer[0] == '\0' || config_buffer[0] == '#' || buffer_lenght < 3) continue;

  // Search for keys
  i = 0;
  while(config_buffer[i] == ' ' || config_buffer[i] == '\t') {
    if(++i == buffer_lenght) break; // skip white spaces
  }
  if(i == buffer_lenght) continue;
  key = &config_buffer[i];

  // Search for '=' ignoring white spaces
  while(config_buffer[i] != '=') {
    if(config_buffer[i] == ' ' || config_buffer[i] == '\t') config_buffer[i] = '\0';
    if(++i == buffer_lenght) {
      break;
    }
  }
  if(i == buffer_lenght) continue;
  config_buffer[i++] = '\0';

  // Search for value ignoring white spaces
  while(config_buffer[i] == ' ' || config_buffer[i] == '\t') {
    if(++i == buffer_lenght) {
      break;
    }
  }
  if(i == buffer_lenght) continue;
  value = &config_buffer[i];
  
  //
  // Process matching keys
  //
  if(strcmp(key, "s1f") == 0) {
    float factor = atoi(value);
    if (mConfig.sensor1_cpm_factor != factor) {
      mConfig.sensor1_cpm_factor = factor;
      config_changed = true;
      DEBUG_PRINTLN("   - Update sensor1_cpm_factor");
    }
  }
  else if(strcmp(key, "s2f") == 0) {
    float factor = atoi(value);
    if (mConfig.sensor2_cpm_factor != factor) {
      mConfig.sensor2_cpm_factor = factor;
      config_changed = true;
      DEBUG_PRINTLN("   - Update sensor2_cpm_factor");
    }
  }
  else if(strcmp(key, "devid") == 0) {
    if (mConfig.devid != (unsigned int)atoi(value)) {
      mConfig.devid = atoi(value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update dev_id");
    }
  }
  else if(strcmp(key, "devt1") == 0) {
    if (mConfig.devt1 != (unsigned int)atoi(value)) {
      mConfig.devt1 = atoi(value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update Device Type 1");
    }
  }
  else if(strcmp(key, "devt2") == 0) {
    if (mConfig.devt2 != (unsigned int)atoi(value)) {
      mConfig.devt2 = atoi(value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update Device Type 2");
    }
  }
  else if(strcmp(key, "uid1") == 0) {
    if (mConfig.user_id != (unsigned int)atoi(value)) {
      mConfig.user_id = atoi(value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update user_id");
    }
  }
  else if(strcmp(key, "uid2") == 0) {
    if (mConfig.user_id2 != (unsigned int)atoi(value)) {
      mConfig.user_id2 = atoi(value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update user_id2");
    }
  }
  else if(strcmp(key, "api") == 0) {
    if (strcmp(mConfig.api_key, value) != 0 ) {
      strcpy(mConfig.api_key, value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update api_key");
    }
  }
  else if(strcmp(key, "s1e") == 0) {
    if (mConfig.sensor1_enabled != atoi(value)) {
      mConfig.sensor1_enabled = atoi(value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update sensor1_enabled flag");
    }
  }
  else if(strcmp(key, "s2e") == 0) {
    if (mConfig.sensor2_enabled != atoi(value)) {
      mConfig.sensor2_enabled = atoi(value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update sensor2_enabled flag");
    }
  }
  else if(strcmp(key, "lat") == 0) {
    if (strcmp(mConfig.latitude, value) != 0 ) {
      strcpy(mConfig.latitude, value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update latitude");
    }
  }
  else if(strcmp(key, "lon") == 0) {
    if (strcmp(mConfig.longitude, value) != 0 ) {
      strcpy(mConfig.longitude, value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update longitude");
    }
  }
  else if(strcmp(key, "dev") == 0) {
    if (mConfig.dev != atoi(value)) {
      mConfig.dev = atoi(value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update API for development");
    }
  }
  else if(strcmp(key, "gw1") == 0) {
    if (strcmp(mConfig.gw1, value) != 0 ) {
      strcpy(mConfig.gw1, value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update Gateway1");
    }
  }
  else if(strcmp(key, "gw2") == 0) {
    if (strcmp(mConfig.gw2, value) != 0 ) {
      strcpy(mConfig.gw2, value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update Gateway2");
    }
  }
  else if(strcmp(key, "intf") == 0) {
    if (strcmp(mConfig.intf, value) != 0 ) {
      strcpy(mConfig.intf, value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update Interface");
    }
  }
  else if(strcmp(key, "tws") == 0) {
    if (mConfig.tws != (unsigned int)atoi(value)) {
      mConfig.tws = atoi(value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update Time measuring window");
    }
  }
  else if(strcmp(key, "alt") == 0) {
    if (mConfig.alt != (unsigned int)atoi(value)) {
      mConfig.alt = atoi(value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update altitude");
    }
  }
  else if(strcmp(key, "autow") == 0) {
    if (mConfig.autow != atoi(value)) {
      mConfig.autow = atoi(value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update auto adaptive window");
    }
  }
  else if(strcmp(key, "alm") == 0) {
    if (mConfig.alm != atoi(value)) {
      mConfig.alm = atoi(value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update Alarm");
    }
  }  
  else if(strcmp(key, "tz") == 0) {
    if (strcmp(mConfig.tz, value) != 0 ) {
      strcpy(mConfig.tz, value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update timezone in EEPROM");
    }
  } 
  else if(strcmp(key, "ssid") == 0) {
    if (strcmp(mConfig.ssid, value) != 0 ) {
      strcpy(mConfig.ssid, value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update SSID for WiFi");
    }
  }
  else if(strcmp(key, "pwd") == 0) {
    if (strcmp(mConfig.pwd, value) != 0 ) {
      strcpy(mConfig.pwd, value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update password for WiFi");
    }
  }
  else if(strcmp(key, "gwn") == 0) {
    if (strcmp(mConfig.gwn, value) != 0 ) {
      strcpy(mConfig.gwn, value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update name for GateWay");
    }
  }
  else if(strcmp(key, "s1i") == 0) {
    if (strcmp(mConfig.s1i, value) != 0 ) {
      strcpy(mConfig.s1i, value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update Sensor 1 Isotope");
    }
  }
  else if(strcmp(key, "s2i") == 0) {
    if (strcmp(mConfig.s2i, value) != 0 ) {
      strcpy(mConfig.s2i, value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update Sensor 2 Isotope");
    }
  }
  else if(strcmp(key, "aux") == 0) {
    if (mConfig.aux != atoi(value)) {
      mConfig.aux = atoi(value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update aux input is active");
    }
  }
  else if(strcmp(key, "trb") == 0) {
    if (mConfig.trb != atoi(value)) {
      mConfig.trb = atoi(value);
      config_changed = true;
      DEBUG_PRINTLN("   - Update trouble shooting active");
    }
  }
  else if(strcmp(key, "apn") == 0) {
   if (strcmp(mConfig.apn, value) != 0 ) {
    strcpy(mConfig.apn, value);
    config_changed = true;
    DEBUG_PRINTLN("   - Update apn");
    }
  }
else if(strcmp(key, "macid") == 0) {
 if (strcmp(mConfig.macid, value) != 0 ) {
  strcpy(mConfig.macid, value);
  config_changed = true;
  DEBUG_PRINTLN("   - Update macid");
}
}
#if ENABLE_EEPROM_DOSE
else if(strcmp(key, "dose") == 0) {
     // Reset total dose in EEPROM
memset(&mDose, 0, sizeof(mDose));
EEPROM_writeAnything(BMRDD_EEPROM_DOSE, mDose);
DEBUG_PRINTLN("   - Reset total dose in EEPROM");
}
#endif

}
DEBUG_PRINTLN("   - Done.");

if (config_changed) {
  // Configuration is changed
DEBUG_PRINTLN("Update configuration in EEPROM");
EEPROM_writeAnything(BMRDD_EEPROM_SETUP, mConfig);
}
}
