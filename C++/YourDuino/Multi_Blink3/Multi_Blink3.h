// Multi_Blink3.h
//
// Blink lots of LEDs at different frequencies simultaneously
//
// Header file is required to be able to define the structured types
//
#include <Arduino.h>

#ifndef  MULTIBLINKH
#define  MULTIBLINKH

// State values for FSM
#define  MB_NULL     0
#define  MB_ON       1
#define  MB_OFF      2
#define  MB_FADE_ON  3
#define  MB_FADE_OFF 4    // stateDef.data is the start and end PWM Setting - use FADE_* macros
#define  MB_LOOP     5    // stateDef.data is the next state position and counter setpoint, use LOOP_* macros

#define  CTR_UNDEF  255

#define  FADE_PARAM(s, e)    (((s) << 8) | (e))
#define  FADE_START_GET(n)   highByte(n)
#define  FADE_END_GET(n)     lowByte(n)

#define  LOOP_PARAM(s, c)    (((s) << 8) | (c))
#define  LOOP_STATE_GET(n)   highByte(n)
#define  LOOP_SP_GET(n)      lowByte(n)

typedef struct
{
  uint8_t  stateID;       // state value for this state to be active (MB_* defines)
  uint16_t activeTime;    // time to stay active in this state in milliseconds
  uint16_t data;          // data store for state context information/parameters (depends on MB_*)
  uint8_t  counter;       // generic counter for the state context information. CTR_UNDEF is special value.
} stateDef;

typedef struct
{
  uint8_t  ledPin;         // Arduino I/O pin number
  uint8_t  currentState;   // current active state
  uint32_t nextWakeup;     // the 'time' for the next wakeup - saves the millis() value
  stateDef state[5];       // the MB_* state definitions. Add more states if required
} ledTable;

#endif

