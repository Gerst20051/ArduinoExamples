/* YourDuino.com Example Software Sketch
 YD_MotorDriver1 Library Test
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <YD_MotorDriver1.h>

/*-----( Declare Constants )-----*/
// NOTE: PWM Motor Speed Pins 9 (Motor1) and 10 (Motor2) are predefined
// They are specific to Timer1 and are unchangeable
#define  Motor1A  2   // Motor pins to Driver board
#define  Motor1B  4
#define  Motor2A  7
#define  Motor2B  8

/*-----( Declare objects )-----*/
YD_MotorDriver1 RobotDriver(Motor1A,Motor1B,Motor2A,Motor2B);

/*-----( Declare Variables )-----*/


void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);
  RobotDriver.init();

}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  RobotDriver.Motor1Speed(200);
  delay(2000);
  RobotDriver.Motor1Speed(-100);
  delay(1000);
  RobotDriver.Motor1Brake(50);  
  delay(2000);
  
  RobotDriver.Motor2Speed(200);
  delay(2000);
  RobotDriver.Motor2Speed(-100);
  delay(1000);
  RobotDriver.Motor2Brake(50);  
  delay(5000);  
}/* --(end main loop )-- */

/*-----( Declare User-written Functions )-----*/


/* ( THE END ) */
