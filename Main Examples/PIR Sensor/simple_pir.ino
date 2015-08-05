/* Simple PIR */
/* http://arduinobasics.blogspot.com/2013/12/pir-sensor-part-2.html */

void setup()
{
    pinMode(13, OUTPUT);
    pinMode(2, INPUT);
}

void loop()
{
    digitalWrite(13, digitalRead(2));
}
