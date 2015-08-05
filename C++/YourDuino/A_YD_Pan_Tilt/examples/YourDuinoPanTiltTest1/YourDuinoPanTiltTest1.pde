/* YourDuino Pan-Tilt test 1
 Dagu Pan-Tilt Kit Test
 2 mini servos
 terry@yourduino.com */
/*-----( Import needed libraries )-----*/
#include <Servo.h>
/*-----( Declare objects )-----*/
Servo PanServo;  // create servo object to control a servo 
Servo TiltServo; 

/*-----( Declare Constants )-----*/
#define PAN_PIN   3
#define TILT_PIN  5
#define PAN_MAX 170 //Change if your servos have full range
#define PAN_MIN 10
#define TILT_MAX 170
#define TILT_MIN 10

/*-----( Declare Variables )-----*/
int  PanPosition = 90;  
int  TiltPosition = 90;  

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  PanServo.attach(PAN_PIN);  // attaches the servo to the servo object   
  TiltServo.attach(TILT_PIN);
  
  PanServo.write(PanPosition);              // tell servo to go to position in variable 'pos'  
  TiltServo.write(TiltPosition);
  
  Serial.begin(9600);
  Serial.print("Pan-Tilt Kit Test1 YourDuino.com");  

}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{


  /*----( PAN Back and forth )----*/
  TiltServo.write(140);   
  for(PanPosition = PAN_MIN; PanPosition < PAN_MAX; PanPosition += 1) 
  {                                  // in steps of 1 degree 
    PanServo.write(PanPosition);              // tell servo to go to position in variable 'pos' 
    delay(50);                       // waits 15ms for the servo to reach the position 
  } 
  delay(500);
  for(PanPosition = PAN_MAX; PanPosition > PAN_MIN + 1; PanPosition -= 1) 
  {                                  // in steps of 1 degree 
    PanServo.write(PanPosition);              // tell servo to go to position in variable 'pos' 
    delay(50);                       // waits 15ms for the servo to reach the position 
  } 

  delay(2500); /* Wait a bit  */

  /*----( TILT Up and Down )----*/
  PanServo.write(90);  
  delay(1000);
  for(TiltPosition = TILT_MAX; TiltPosition > TILT_MIN-1; TiltPosition -= 1) 
  {                                  // in steps of 1 degree 
    TiltServo.write(TiltPosition);              // tell servo to go to position in variable 'pos' 
    delay(50);                       // waits 15ms for the servo to reach the position 
  } 
  delay(500);  
  for(TiltPosition = TILT_MIN; TiltPosition < TILT_MAX; TiltPosition += 1) 
  {                                  // in steps of 1 degree 
    TiltServo.write(TiltPosition);              // tell servo to go to position in variable 'pos' 
    delay(50);                       // waits 15ms for the servo to reach the position 
  } 


  delay(2500); /* Wait a bit  */


}/* --(end main loop )-- */

/* ( THE END ) */

