/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * KSduino Control_01 Example Sketch
 *
 * control_01.ino
 *  
 * KSduino Control_01 is KSduino-library example sketch
 * This example is a part of KSduino Libray for Arduino
 *
 * Copyright (C) Kirill Scherba 2012-2013 <kirill@scherba.ru>
 * 
 * KSduino library & examples is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ksduino-server is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @example control_01.ino
 *
 * Control_01 example shows how to send values from <a href="http://ksduino.org">KSduino</a> server to your Arduino 
 * device with KSduino Library.
 *
 * This sample used beginPacket, addParameter, addMessage, endPacket functions
 * to send D1 parameter and KSduinoParameter class to send D1 parameters to 
 * the <a href="http://ksduino.org">KSduino</a> server. Function sendOne uses to send B1 parameter. This test uses 
 * setReceptionCallback function to setup receptionFunc function which receive values 
 * from the <a href="http://ksduino.org">KSduino</a> server.
 *
 * This demonstrate Control from server mechanism by using receptionFunc function.  
 * This test receives value from <a href="http://ksduino.org">KSduino</a> server by using receptionFunc function. This is 
 * callback function which setup in the Arduinos begin whith the setReceptionCallback.
 *
 * The algorithm of Control from <a href="http://ksduino.org">KSduino</a> server is very easy:
 *
 *  - use the server form or button to send parameter with value and message. These type of parameters 
 *    are not created at <a href="http://ksduino.org">KSduino</a> web server. Programmer can use any parameter to send it to his device
 *
 * - device uses the ksdReceptionCallback function which calls when KSduino library get a packet
 *    from the server. The Function parameters contains ID of server = 3300, parameter (one letter)
 *    number of parameter, value and message.
 *
 * In this example we send: b1 with any value and some message from the servers web form.
 * The device receive: device_id  = 3300, 'b', 1, value = b_value,message= "some_text". Then we send
 * received value to server as B1 parameter of our device to show this action. Parameters in servers web
 * form and Arduino  parameters is different. We don't define servers parameters no where and can use 
 * it as we like.
 *
 * The D1 parameter just shows that your device are running now. We don't use any equipment in this test.
 * Just setup and run this example send any value of B1 parameter from servers control from and watch it in 
 * B1 graph of  this device.
 *
 */

// Includes -------------------------------------------------------------

#include <SPI.h>
#include <EthernetUdp.h>

#include <KSduino.h>

// User definition -----------------------------------------------------

// Change this values to yours

// Your Arduinos ID & Password
unsigned int deviceID = 000;
unsigned int devicePwd = 0000;

// Your Arduinos MAC, IP & port used in your local network
byte mac[] = { 0xDE, 0xAD, 0xBE, 0x55, 0xAE, 0x0D };
byte ip[] = { 192, 168, 101, 222 };
unsigned int port = 59977;

// Server definition -----------------------------------------------------

// Do not change this values

// KSduino Server address & port
byte serverIp[] = { 178,63,53,233 };
unsigned int serverPort = 9930;

// ----------------------------------------------------------------------------

// Create KSduino class
KSduino ksd (deviceID, devicePwd, serverIp, serverPort); 
KSduinoParameter d1 (&ksd, "d1");  // Define D1 parameter

// ----------------------------------------------------------------------------

void setup()
{
  
  // Start KSduino at the beginning of setup
  ksd.begin (mac, ip, port);    
  ksd.setReceptionCallback (receptionFunc);
  
  Serial.begin(115200); 
  
  Serial.println ("KSduino Control_01 test ver 0.0.1\n");
}

void loop()
{
  static byte d_value = LOW;
  static byte d_time = 0;
  static byte b_value = 0;

  // Set value of D1 parameter, it will be changes ones per 2 second becouse the loops delay = 100 ms
  if (d_time++ >= 20)
  {
    if (d_value == LOW) d_value = HIGH; else  d_value = LOW;
    d_time = 0;
  }
  
  // Send parameters to KSduino server
  ksd.beginPacket ();
    // This value & message will be sent if d_value changed
    if (d1.addParameter (d_value))  {  
      ksd.addMessage ("Hello controlled World!");
    }    
  ksd.endPacket ();
  
  // KSduino update should be at the end of loop
  ksd.update ();    

  // Do delay
  delay (100-ksd.getLoopTime());  // This delay = 100 ms
}

// Check data received from server
int receptionFunc (int device_id, char param, int num, void *value, char *message)
{
  int *intval; 
  intval = (int*) value;
  
  // Resend received value to B1 parameter to show it on KSduino web site 
  if (device_id == 3300 && param == 'b' && num == 1) ksd.sendOne ("b1", *intval, message);

  // Display messages to serial (remove or comment it in production mode)
  Serial.print ("Device ID: ");
  Serial.print (device_id);
  
  Serial.print (", ");
  Serial.print (param);
  Serial.print (num);
  Serial.print (" = ");
  Serial.print(*intval);

  if (message)
  {
    Serial.print (", message = \"");
    Serial.print (message);  
    Serial.print ("\"");
  }
  
  Serial.println ();  
  
  return 0;
}


