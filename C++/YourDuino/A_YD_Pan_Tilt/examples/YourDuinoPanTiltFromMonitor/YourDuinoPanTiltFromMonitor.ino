/* YourDuino.com PAN-Tilt Command 
 Send Commands from Serial Monitor to Servos
 Servos connect to pin 3 (Pan) and 5 (Tilt)
 Run Serial monitor; Use NumLock and arrow keyes then <Enter>
 6=R,4=L,8=U,2=D
 You may type many characters and THEN hit <Enter> to send
 "c" Re-centers the servos
 Some servos have a limit of less than 0..180
 terry@yourduino.com */

//*-----( Import needed libraries )-----*/
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
int  PanPosition;  
int  TiltPosition;
int  inByte = 0;         // incoming serial byte


void setup()     /*----( SETUP: RUNS ONCE )----*/
{
  PanServo.attach(PAN_PIN);  // attaches the servo to the servo object   
  TiltServo.attach(TILT_PIN);
  PanPosition = 100;  
  PanServo.write(PanPosition);    // Start at nn degrees  
  delay(500);
  TiltPosition = 110;  
  TiltServo.write(TiltPosition);    // Start at nn degrees 

  Serial.begin(9600);  // start serial port at 9600 bps
  Serial.println("Move Pan/Tilt from Serial Monitor  YourDuino.com ");
  Serial.println("Servo Position (Pan) (Tilt) in Degrees:");  
}/*--(end setup )---*/

void loop()
{
  if (Serial.available() > 0)    // look for valid byte
  {

    inByte = Serial.read();    // get incoming byte
    /*------( PAN Control )-------------*/
    if (inByte == '4') // Change Servo position + or -
    {
      PanPosition++;  
    } 
    if (inByte == '6')
    {
      PanPosition--;  
    }     
    /*------( TILT Control )-------------*/
    if (inByte == '2') // Change Servo position + or -
    {
      TiltPosition++;  
      
    } 
    if (inByte == '8')
    {
      TiltPosition--;  
    }     


    if (inByte == 'c') // Re-center the servo
    {
      PanPosition  = 90;      
      TiltPosition = 90;  
    }  
    /*------( Move servos )-------------*/
    PanServo.write(PanPosition);     
    TiltServo.write(TiltPosition); 

    Serial.print(PanPosition);  
    Serial.print("    "); 
    Serial.println(TiltPosition);     
  }

}/* --(end main loop )-- */

/* ( THE END ) */






