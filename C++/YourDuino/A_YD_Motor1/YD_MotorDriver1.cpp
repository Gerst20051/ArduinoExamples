#include "YD_MotorDriver1.h"

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
// Constructors 

YD_MotorDriver1::YD_MotorDriver1()  //Default Constructor
{
  //Pin map
  _INA1 = 2;
  _INB1 = 4;

  _INA2 = 7;
  _INB2 = 8;


}

YD_MotorDriver1::YD_MotorDriver1(unsigned char INA1, unsigned char INB1, unsigned char INA2, unsigned char INB2)
{

  //PWM1 and PWM2 cannot be changed because PWM is on timer1
  _INA1 = INA1;
  _INB1 = INB1;
  _INA2 = INA2;
  _INB2 = INB2;

}

// Public Methods  
/*--------------( init: Set PinModes and set up Timer1  )--------------------------------*/
void YD_MotorDriver1::init()
{

  pinMode(_INA1,OUTPUT);
  pinMode(_INB1,OUTPUT);
  pinMode(_PWM1,OUTPUT);
  pinMode(_INA2,OUTPUT);
  pinMode(_INB2,OUTPUT);
  pinMode(_PWM2,OUTPUT);

  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__)
  // Timer 1 configuration
  // prescaler: clockI/O / 1
  // outputs enabled
  // phase-correct PWM
  // top of 400
  //
  // PWM frequency calculation
  // 16MHz / 1 (prescaler) / 2 (phase-correct) / 400 (top) = 20kHz
  TCCR1A = 0b10100000;
  TCCR1B = 0b00010001;
  ICR1 = 400;
  #endif
}
/*--------------( Motor 1 Speed )--------------------------------*/

void YD_MotorDriver1::Motor1Speed(int speed)
{
  unsigned char reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed;  // Make speed a positive quantity
    reverse = 1;  // Preserve the direction
  }
  if (speed > 400)  // Max PWM dutycycle
    speed = 400;
  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__)
  OCR1A = speed;
  #else
  analogWrite(_PWM1,speed * 51 / 80); // default to using analogWrite, mapping 400 to 255
  #endif
  if (reverse)
  {
    digitalWrite(_INA1,LOW);
    digitalWrite(_INB1,HIGH);
  }
  else
  {
    digitalWrite(_INA1,HIGH);
    digitalWrite(_INB1,LOW);
  }
}

/*--------------( Motor 2 Speed )--------------------------------*/
void YD_MotorDriver1::Motor2Speed(int speed)
{
  unsigned char reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed;  // make speed a positive quantity
    reverse = 1;  // preserve the direction
  }
  if (speed > 400)  // Max 
    speed = 400;
  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__)
  OCR1B = speed;
  #else
  analogWrite(_PWM2,speed * 51 / 80); // default to using analogWrite, mapping 400 to 255
  #endif 
  if (reverse)
  {
    digitalWrite(_INA2,LOW);
    digitalWrite(_INB2,HIGH);
  }
  else
  {
    digitalWrite(_INA2,HIGH);
    digitalWrite(_INB2,LOW);
  }
}


/*--------------( Motor 1 Brake )--------------------------------*/
void YD_MotorDriver1::Motor1Brake(int brake)
{
  // normalize brake
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 400)  // Max brake
    brake = 400;
  digitalWrite(_INA1, LOW);
  digitalWrite(_INB1, LOW);
  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__)
  OCR1A = brake;
  #else
  analogWrite(_PWM1,brake * 51 / 80); // default to using analogWrite, mapping 400 to 255
  #endif
}

/*--------------( Motor 2 Brake )--------------------------------*/
void YD_MotorDriver1::Motor2Brake(int brake)
{
  // normalize brake
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 400)  // Max brake
    brake = 400;
  digitalWrite(_INA2, LOW);
  digitalWrite(_INB2, LOW);
  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__)
  OCR1B = brake;
  #else
  analogWrite(_PWM2,brake * 51 / 80); // default to using analogWrite, mapping 400 to 255
  #endif
}




