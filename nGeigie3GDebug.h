#ifndef _NGEIGIE_DEBUG_H_
#define _NGEIGIE_DEBUG_H_

#define ENABLE_DEBUG 

// Debug definitions ----------------------------------------------------------
#ifdef ENABLE_DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

#endif
