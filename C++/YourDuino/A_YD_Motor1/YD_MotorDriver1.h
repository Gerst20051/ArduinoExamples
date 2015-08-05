#ifndef YD_MotorDriver1_h
#define YD_MotorDriver1_h
/*--------( Include the correct Arduino definitions file, for Arduino1.0 or earlier versions)----*/
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class YD_MotorDriver1
{
  public:  
    // CONSTRUCTORS
    YD_MotorDriver1(); // Default pins
    YD_MotorDriver1(unsigned char INA1, unsigned char INB1, unsigned char INA2, unsigned char INB2); // Pin Options 
    
    // PUBLIC METHODS
    void init(); // Set up TIMER 1, set the PWM to 20kHZ
    void Motor1Speed(int speed); // Set speed for Motor1
    void Motor2Speed(int speed); 
    void Motor1Brake(int brake); // Brake Motor1
    void Motor2Brake(int brake); 
    
  private:
    unsigned char _INA1;
    unsigned char _INB1;
    static const unsigned char _PWM1 = 9;  // Can not be changed by user
    unsigned char _INA2;
    unsigned char _INB2;
    static const unsigned char _PWM2 = 10;
    
}; // End Class

#endif