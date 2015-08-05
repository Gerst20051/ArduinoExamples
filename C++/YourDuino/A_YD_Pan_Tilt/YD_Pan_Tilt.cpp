/* 	YD_Pan_Tilt: Arduino Library for Dagu Pan-Tilt Kit
	(This is the Implementation / Source File part of the Library)
	Run YD_Pan_Tilt_Example1 in "examples" folder here. There should be no compile errors.
	
	V1.1 16Mar2012 Terry King  terry@yourduino.com
	
*/
/*--------( Include the Declaration/Header file)----*/
#include "YD_Pan_Tilt.h"
//???
//#include "C:\Documents and Settings\Administrator\Desktop\arduino-0023\libraries\Servo\Servo.h"
#include "..\Servo\Servo.h"


// Constructors 

YD_Pan_Tilt::YD_Pan_Tilt()  //Default Constructor
{
  //Allow default values of Variables
  _SERVO_PIN1 = 3;
  _SERVO_PIN2 = 5;
}
/*--------------( Normal Constructor  )--------------------------------*/
YD_Pan_Tilt::YD_Pan_Tilt(unsigned char SERVO_PIN1, unsigned char SERVO_PIN2) //Same as in .h !
{
  //Copy the passed variable values to the private variables
  _SERVO_PIN1 = SERVO_PIN1;
  _SERVO_PIN2 = SERVO_PIN2;  

}

// Public Methods  
/*--------------( init: Whatever init time actions you need  )--------------------------------*/
void YD_Pan_Tilt::init()
{

  pinMode(13,OUTPUT);  // Show we are alive: blink differently than "BLINK"
  for (int i = 0; i < 8; i++)
  {
  //???
    Servo  MyServo;
	MyServo.attach(_SERVO_PIN1);
	MyServo.write(90); 
	
    digitalWrite(13, HIGH);
	delay(100);
    digitalWrite(13, LOW);	
	delay(500);
  };


}
/*--------------( YD_Pan_Tilt_Method1 )--------------------------------*/

void YD_Pan_Tilt::panPosition(int var1) // Blink LONG number of times in variable
{
  for (int i = 0; i < var1; i++)
  {
  //???
  //  MyServo.attach(9);
//  	MyServo.write(90); 
    digitalWrite(13, HIGH);
	delay(1000);
    digitalWrite(13, LOW);	
	delay(100);  
  }
}

/*--------------( YD_Pan_Tilt_Method2 )--------------------------------*/
void YD_Pan_Tilt::tiltPosition(int var1) // Blink SHORT number of times in variable
{
  for (int i = 0; i < var1; i++)
  {
    digitalWrite(13, HIGH);
	delay(50);
    digitalWrite(13, LOW);	
	delay(500);  
   }
}
/*--------------( YD_Pan_Tilt_Method3 )--------------------------------*/

void YD_Pan_Tilt::panSpeed(int var1) // Blink LONG number of times in variable
{
  for (int i = 0; i < var1; i++)
  {
    digitalWrite(13, HIGH);
	delay(1000);
    digitalWrite(13, LOW);	
	delay(100);  
  }
}

/*--------------( YD_Pan_Tilt_Method4 )--------------------------------*/

void YD_Pan_Tilt::tiltSpeed(int var1) // Blink SHORT number of times in variable
{
  for (int i = 0; i < var1; i++)
  {
    digitalWrite(13, HIGH);
	delay(50);
    digitalWrite(13, LOW);	
	delay(1000);  
  }
}




