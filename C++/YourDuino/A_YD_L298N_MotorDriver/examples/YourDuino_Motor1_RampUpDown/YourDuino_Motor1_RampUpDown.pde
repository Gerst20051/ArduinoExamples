/* YourDuino.com Example Software Sketch
 YD_MotorDriver1 Library Test
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <YD_MotorDriver1.h>

/*-----( Declare Constants )-----*/
// NOTE: Pins 9 (Motor1) and 10 (Motor2) are predefined, unchangeable
#define  Motor1A  2   // Motor pins to Driver board
#define  Motor1B  4
#define  Motor2A  7
#define  Motor2B  8

#define  RampDelay  10

/*-----( Declare objects )-----*/
YD_MotorDriver1 RobotDriver(Motor1A,Motor1B,Motor2A,Motor2B);

/*-----( Declare Variables )-----*/


void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);
  Serial.println("YourDuino MotorDriver1 Ramp Up / Down Test");
  RobotDriver.init();

}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  
/*----( Ramp Motor1 Up and Down both directions )-------*/  
  for (int i = 0; i <= 400; i++)
  {
    RobotDriver.Motor1Speed(i);
    delay(RampDelay);
  }  
  
  for (int i = 400; i >= -400; i--)
  {
    RobotDriver.Motor1Speed(i);
    delay(RampDelay);
  } 

  for (int i = -400; i <= 0; i++)
  {
    RobotDriver.Motor1Speed(i);
    delay(RampDelay);
  } 


  
/*----( Ramp Motor2 Up and Down both directions )-------*/  
  for (int i = 0; i <= 400; i++)
  {
    RobotDriver.Motor2Speed(i);
    delay(RampDelay);
  }  
  
  for (int i = 400; i >= -400; i--)
  {
    RobotDriver.Motor2Speed(i);
    delay(RampDelay);
  } 

  for (int i = -400; i <= 0; i++)
  {
    RobotDriver.Motor2Speed(i);
    delay(RampDelay);
  } 

  delay(5000);  
}/* --(end main loop )-- */

/*-----( Declare User-written Functions )-----*/


/* ( THE END ) */
