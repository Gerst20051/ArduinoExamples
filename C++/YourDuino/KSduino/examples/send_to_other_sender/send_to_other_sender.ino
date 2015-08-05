/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * KSduino Send to Other Example Sender Sketch
 *
 * send_to_other_sender.ino
 *  
 * KSduino Send to Other Sender is KSduino-library example sketch
 * This example is a part of KSduino Libray for Arduino
 *
 * Copyright (C) Kirill Scherba 2012 <kirill@scherba.ru>
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
 * @send_to_other_sender.ino
 *
 * Send_to_other example shows how to send values from one Arduino to other Arduino 
 * device with KSduino Library.
 *
 * This sample used beginPacket, addParameter, addMessage, endPacket functions
 * to send D1 parameter and KSduinoParameter class to send D1,B1 parameters to 
 * server. Function sendOne uses to send B2 parameter.
 *
 * This sample demonstrate using of sendToOther function. This test simulates two device: 
 * sender and receiver. It uses sendToOther function to send value to other device as Sender
 * but send this value itself. Then this test receives value from other device using receptionFunc
 * function. This is callback function which setup in the Arduinos begin whith the setReceptionCallback.
 *
 * The algorithm of Send to Other is very easy:
 *
 *  - sender device uses the sendToOther function to send any parameter with value. These parameters 
 *    are not created at KSduino web server. Programmer can use any parameter to send it to other device
 *
 * - receiver device uses the ksdReceptionCallback function which calls when KSduino library get a packet
 *    from other device. Function parameters contains ID of device which sent the packet, parameter (one letter)
 *    and its number, value and message.
 *
 * In this example we send: b2 with value = b_value and message = "Received from other devices".
 * And receive: device_id  = deviceID, 'b', 2, value = b_value,message= "Received from other devices"
 *
 */

// Includes -------------------------------------------------------------

#include <SPI.h>
#include <EthernetUdp.h>

#include <KSduino.h>

// User definition -----------------------------------------------------

// Change this values to yours

// Your Arduinos ID & Password
unsigned int deviceID = 0000;
unsigned int devicePwd = 0000;

// Your Arduinos MAC, IP & port used in your local network
byte mac[] = { 0xDE, 0xAD, 0xBE, 0x55, 0xAE, 0x0D };
byte ip[] = { 192, 168, 0, 222 };
unsigned int port = 59978;

// Server definition -----------------------------------------------------

// Do not change this values

// KSduino Server address & port
byte serverIp[] = { 178,63,53,233 };
unsigned int serverPort = 9930;

// ----------------------------------------------------------------------------

// Create KSduino class
KSduino ksd (deviceID, devicePwd, serverIp, serverPort);
KSduinoParameter d1 (&ksd, "d1");
KSduinoParameter b1 (&ksd, "b1",10);

// ----------------------------------------------------------------------------

void setup()
{
  
  // Start KSduino at the beginning of setup
  ksd.begin (mac, ip, port);    
  
  Serial.begin(115200); 
  
  Serial.println ("KSduino Send to Other test ver 0.0.1\n");
}

void loop()
{
  static byte d_value = LOW;
  static byte d_time = 0;
  static byte b_value = 0;
  byte send_b1 = 0;

  // Set value to D1 variable  
  if (d_time++ >= 10)
  {
    if (d_value == LOW) d_value = HIGH; else  d_value = LOW;
    d_time = 0;
  }
  
  // Set value to B1 variable
  b_value++;
   
  // Send parameters to KSduino server
  ksd.beginPacket ();
  d1.addParameter (d_value);
  send_b1 = b1.addParameter (b_value);
  ksd.addMessage ("Hello World!");
  ksd.endPacket ();
  
  // Send B1 parameter value to other device as B2 parameter
  if (send_b1) ksd.sendToOther  (0000, 0000, "b2", b_value, "Received from other devices");   
  
  // KSduino update should be at the end of loop
  ksd.update ();    

  // Do delay
  delay (100-ksd.getLoopTime());  
}

