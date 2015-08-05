/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * KSduino Simple-02 Example Sketch
 *
 * simple_02.ino
 *  
 * KSduino Simple-01 is KSduino-library example sketch
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
 * @file simple_02.ino
 *
 * Simple-02 is an Example for Getting Started with KSduino Library
 * This sample used sendBuffer function to send data to server
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
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D };
byte ip[] = { 192, 168, 0, 222 };
unsigned int port = 58833;

// Server definition -----------------------------------------------------

// Do not change this values

// KSduino Server address & port
byte serverIp[] = { 178,63,53,233 };
unsigned int serverPort = 9930;

// ----------------------------------------------------------------------------

// Create KSduino class
KSduino ksd (deviceID, devicePwd, serverIp, serverPort);

// ----------------------------------------------------------------------------

void setup()
{
  // Start KSduino at the beginning of setup
  ksd.begin (mac, ip, port);    
}

void loop()
{
  static byte d_value = LOW;
  char buffer[32];

  // Set value to D1 variable  
  if (d_value == LOW) d_value = HIGH; else  d_value = LOW;      
  
  // Create text buffer with parameter d1
  sprintf (buffer, "d1=%d", d_value);
  
  // Send buffer with parameters & values to KSduino server
  ksd.sendBuffer (buffer);  
  
  // Do delay
  delay (1000);
  
  // KSduino update should be at the end of loop
  ksd.update ();  
}

