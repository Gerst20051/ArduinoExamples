/* 	YD_Pan_Tilt: Arduino Library for Dagu Pan-Tilt Kit
	(This is the Declaration / Header part of the Library)
	
	Run YD_Pan_Tilt_Example1 in "examples" folder here. There should be no compile errors.
	
	NOTE: After making library changes, before recompiling, you may need to delete the <LibName>.o file 
	which can be found by searching, or in a file similar to:
	C:\Documents and Settings\Administrator\Local Settings\Temp\build3998508668703718780.tmp
	NOTE: To include existing Arduino Library in new libray you must:
	-- Use relative path to existing libraries ie. #include "..\Servo\Servo.h"
	-- ALSO #include the Arduino Library used IN THE SKETCH "#include <Servo.h>"
	
	V1.1 16Mar2012 Terry King  terry@yourduino.com
	
*/	
/*--------( This ensures that the .h file is included in the output only once)----*/
#ifndef YD_Pan_Tilt_h
#define YD_Pan_Tilt_h

/*--------( Include the correct Arduino definitions file, for Arduino1.0 or earlier versions)----*/
#if (ARDUINO >= 100)
#include <Arduino.h>   // For 1.0 +
#else
#include <WProgram.h>  // For 0023 and earlier
#endif
//???
//#include "C:\Documents and Settings\Administrator\Desktop\arduino-0023\libraries\Servo\Servo.h"
/*--------( Must use the relative path to find existing Arduino Libraries)----*/
#include "..\Servo\Servo.h"

/*--------( Create your class definitions)----*/
class YD_Pan_Tilt
{
  public:  
    // CONSTRUCTORS
    YD_Pan_Tilt(); // Default 
    YD_Pan_Tilt(unsigned char SERVO_PIN1, unsigned char SERVO_PIN2); // Variables
    
    // PUBLIC METHODS
    void init(); // Whatever init time things you need to have happen..
    void panPosition(int Pan_Position); // 
	void tiltPosition(int Tilt_Position); // 
    void panSpeed(int Pan_Speed); // 
    void tiltSpeed(int Tilt_Speed); // 			

    
  private:
    unsigned char 	_SERVO_PIN1;
    unsigned char	_SERVO_PIN2;
    int 			_Pan_Position;  // To be used to hold variables
    int 			_Tilt_Position; 
    int 			_Pan_Speed; 
    int 			_Tilt_Speed; 
;
    
}; // End Class

#endif // End the conditional include of this file 