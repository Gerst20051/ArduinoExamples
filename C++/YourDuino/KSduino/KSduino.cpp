/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * KSduino.cpp
 *
 * KSduino-library for Arduino ver 0.0.1
 *
 * Copyright (C) Kirill Scherba 2012-2013 <kirill@scherba.ru>
 * 
 * KSduino Library is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License  as published by 
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * KSduino-library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://opensource.org/licenses/LGPL-2.1>
 *
 */

/**
 * @file KSduino.cpp
 *
 * KSduino functions
 *
 */
 
#include "KSduino.h" 

#ifdef __arduino__
# 
#ifdef __W5100__
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#
#include <utility/w5100.h>
#include <utility/socket.h>
#endif
#
#ifdef __ENC28J60__
#include <EtherCard.h>
#endif
#
#endif

#ifdef __linux__
#
#include <stdio.h>
#include <string.h>
#include "KSduinoEthernet.h"
#include "KSduinoEthernetUdp.h"
#
#endif

#define BUF_SIZE 40*2
#define XBUF_SIZE 32
#define MAX_WAIT_TIME 2500
#define MAX_NUM_TO_REPEAT_IN_WAIT_PACKET 5

#define TIMEOUT_BEFORE_RESET 300

#if PRINT_SERIAL_MESSAGES
#ifdef __arduino__
#define ksd_print(text) Serial.print(text)
#define ksd_println(text) Serial.println(text)
#endif
#ifdef __linux__
#include <iostream>
using namespace std; 
#define ksd_print(text) cout << text
#define ksd_println(text) cout << text; cout << "\n"
#endif
#else
#define ksd_print(text)   
#define ksd_println(text)
#endif

#ifdef __arduino__W5100__
  #define _parsePacket() parsePacket()
#else 
#ifdef __arduino__ENC28J60__
    #define _parsePacket() Udp.parsePacket(&_len, &_pos)  
#else
    #define _parsePacket() Udp.parsePacket()
#endif    
#endif

void(* resetArduino) (void) = 0;    		                  

/**
 * This Class constructor
 *
 * @param [in] devID Your Device ID
 * @param [in] devPwd Your Device password
 * @param [in] serverAddr <a href="http://ksduino.org">KSduino</a> Server address
 * @param [in] serverP <a href="http://ksduino.org">KSduino</a> Server port
 *
 * This function used to create KSduino class at the begining of sketch, before 
 * setup function.
 *
 * \see \ref simple_01.ino example for more information about using this function.
 */
KSduino::KSduino (unsigned int devID, unsigned int devPwd, byte *serverAddr, 
                  unsigned int serverP)
{
    deviceID = devID; 
    devicePasswd = devPwd;
    serverIpAddr = serverAddr;
    serverPort = serverP;  
    _loopTime = MAX_WAIT_TIME;
    _receptionFunc = NULL;
    #ifdef __arduino__W5100__
    _beforeResetFunc = NULL;
    #endif
    _lostPacketCallback = NULL;
    _totalPackets = 0;
    _lostPackets = 0;
    _wrongAnswer = 0;
    _lastSend = 0;
    _idx = 0;
}


/*
 * Get W5100 socket & set timeout
 */
#ifdef __arduino__W5100__
#define getSocket() \
  for (int i = 0; i < MAX_SOCK_NUM; i++) { \
    uint8_t s = W5100.readSnSR(i); \
    if (s == SnSR::CLOSED || s == SnSR::FIN_WAIT) { \
      _s = i; \
      break; \
    } \
  } \
  uint16_t _timeout = 2000; \
  W5100.setRetransmissionTime (_timeout);
#else
  #define getSocket()
#endif

#ifdef __arduino__ENC28J60__
#define Ethernet Ethernet_ethercard 
#endif


/**
 * Initializes the ethernet library and network settings.
 *
 * @param [in] mac *Device mac address
 * @param [in] ip Device IP addres
 * @param [in] dns Local Network DNS server
 * @param [in] gateway Local Network Gateway
 * @param [in] port *Device local port
 *
 * This function does not return any values.
 *
 * Setup Local network Ethernet parameters. Mac addreess and port are requerid 
 * parameters. 
 *
 * <b>Variants of calling:</b>
 *
 * \code
 *  void 	begin (byte *mac, byte *ip, byte *dns, byte *gateway, unsigned int port);
 *  void 	begin (byte *mac, byte *ip, byte *dns, unsigned int port);
 *  void  begin (byte *mac, byte *ip, unsigned int port);
 *  void 	begin (byte *mac, unsigned int port);
 * \endcode
 *
 * This function called at the beginning of setup function to initialise ethernet 
 * controller. This function replace standard Arduino Ethernet::begin function.
 *
 * The most used variant of calling this function is: 
 * \pre void  begin (byte *mac, byte *ip, unsigned int port);
 *
 * If you'll use variants without IP it takes 4K of your sketch size.
 *
 * \see \ref simple_01.ino example for more information about using this function.
 *
 */
void KSduino::begin (byte *mac, byte *ip, byte *dns, byte *gateway, 
                                      unsigned int port)
{
  Ethernet.begin (mac, ip, dns, gateway);
  getSocket();
  _port = port;
  Udp.begin (port);
}
void KSduino::begin (byte *mac, byte *ip, byte *dns, unsigned int port)
{
  Ethernet.begin (mac, ip, dns);
  getSocket();
  _port = port;
  Udp.begin (port);
}
void KSduino::begin (byte *mac, byte *ip, unsigned int port)
{
  Ethernet.begin (mac, ip);
  getSocket();
  _port = port;
  Udp.begin (port);
}
void KSduino::begin (byte *mac, unsigned int port)
{
  Ethernet.begin (mac);
  getSocket();
  _port = port;
  Udp.begin (port);
}
#ifndef __arduino__ENC28J60__
void KSduino::begin (byte *mac, byte *ip, byte *dns, byte *gateway, 
                                       byte* subnet, unsigned int port)
{
  Ethernet.begin (mac, ip, dns, gateway, subnet);
  getSocket();
  _port = port;
  Udp.begin (port);
}
#endif

/**
 * Send formated buffer to server
 *
 * @param [in] userData *Text buffer with parameters & values
 * @param [in] checkAnswer 1,2 or 0; 2 is default value
 *
 * \b checkAnswer values:
 * @li 1 - wait server for answer
 * @li 2 - not wait server for answer but check the answer in update function
 * @li 0 - do nothing, just send packet
 *
 * \b userData value: 
 * @li before using this function, you have to create a text 
 * buffer containing all the parameters and their values in the following 
 * format: p1=v1[,p2=v2]... Here: p1 & p2 - parameters, it shoud be d1 & i1 for 
 * example, and v1 & v2 its values, 0 & 25 for example. I this case the result 
 * buffer should be: "d1=0,i1=25".
 * To send packets message add message parameter and message to the buffer: 
 * msg=\\"Hello World\", and you result buffer should be look like this:
 * "d1=0,i1=25,msg=\"Hello World\\""
 *
 * This function does not return any values.
 *
 * \see \ref simple_02.ino example for more information about using this function.
 *
 * \note This is most quickly and most save arduino resources KSduino function to send 
 * parameters to server. Perhaps this function is not very convenient for the 
 * programmer so there are other best samples to sent: KSduino::sendOne, 
 * KSduino::beginPacket ... KSduino::endPacket
 *
 */
void KSduino::sendBuffer (const char *userData, int checkAnswer)
{ 
  beginPacket ();                       // Start (init) Packet
  Udp.write (",");
  Udp.write (userData);           // Write User data
  endPacket (checkAnswer);  // Send Packet to server 
}
#ifdef __arduino__
/**
 * Send formated String to server
 *
 * @param [in] userData *String with parameters & values
 * @param [in] checkAnswer 1,2 or 0; 2 is default value
 *
 * \b checkAnswer values:
 * @li 1 - wait server for answer
 * @li 2 - not wait server for answer but check the answer in update function
 * @li 0 - do nothing, just send packet
 *
 * \b userData value: 
 * @li before using this function, you have to create a String object which 
 * will contain all the parameters and their values in the following 
 * format: p1=v1[,p2=v2]... Here: p1 & p2 - parameters, it shoud be d1 & i1 for 
 * example, and v1 & v2 its values, 0 & 25 for example. I this case the result 
 * buffer should be: "d1=0,i1=25".
 * To send packets message add message parameter and message to the buffer: 
 * msg=\\"Hello World\", and you result buffer should be look like this:
 * "d1=0,i1=25,msg=\"Hello World\\""
 *
 * This function similar to: 
 * \code void	sendBuffer (const char *userData, int checkAnswer=2) \endcode
 * but uses String instead of "const char * userData"
 *
 * \see \ref simple_03.ino example for more information about using this function.
 *
 */
void KSduino::sendBuffer (String userData, int checkAnswer)
{ 
  int i, len = userData.length();
  beginPacket ();           // Start (init) Packet
  Udp.write (",");
  for (i=0; i < len; i++)  Udp.write (userData.charAt(i));  // Write User data
  Udp.write ((byte)0);
  endPacket (checkAnswer);  // Send Packet to server 
}
#endif


/**
 * Send one parameter to server
 *
 * @param [in] parameter Name of Parameter
 * @param [in] value Parameters Value (any parameters type)
 * @param [in] message Packets message or NULL if not used
 * @param [in] checkAnswer 1,2 or 0; 2 is default value
 *
 * \b checkAnswer values:
 * @li 1 - wait server for answer
 * @li 2 - not wait server for answer but check the answer in update function
 * @li 0 - do nothing, just send packet
 *
 * This function does not return any values.
 *
 * This function used to send one parameter to 
 * <a href="http://ksduino.org">KSduino</a> server. To send group of parameters 
 * use KSduino::sendBuffer or KSduino::beginPacket ... KSduino::endPacket 
 * functions.
 *
 * \see \ref simple_01.ino example for more information about using this function.
 *
 */
void KSduino::sendOne (const char *parameter, const char *value, 
                       const char *message, int checkAnswer)
{
  beginPacket ();                     		// Start (init) Packet
  addParameter (parameter, value);		// Add parameter
  if (message != NULL) addMessage (message);	// Add message
  endPacket (checkAnswer);            		// Send Packet to server   
}

void KSduino::sendOne (const char *parameter, float value, const char *message, 
                       int checkAnswer )
{
  char fBuf [XBUF_SIZE];
  
  dtostrf (value, 1, 2, fBuf); 
  if (strstr(fBuf,"INF") != NULL) strcpy (fBuf,"0.00");
  sendOne (parameter, fBuf, message, checkAnswer);
}

void KSduino::sendOne (const char *parameter, byte value, const char *message, 
                       int checkAnswer )
{
  char dBuf [XBUF_SIZE];
  
  //snprintf (dBuf, XBUF_SIZE, "%d", value); 
  dtostrf (value, 1, 0, dBuf);
  sendOne (parameter, dBuf, message, checkAnswer);
}

void KSduino::sendOne (const char *parameter, int value, const char *message, 
                       int checkAnswer )
{
  char dBuf [XBUF_SIZE];
  
  //snprintf (dBuf, XBUF_SIZE, "%d", value); 
  dtostrf (value, 1, 0, dBuf);
  sendOne (parameter, dBuf, message, checkAnswer);
}

void KSduino::sendOne (const char *parameter, unsigned int value, 
                       const char *message, int checkAnswer )
{
  char dBuf [XBUF_SIZE];
  
  //snprintf (dBuf, XBUF_SIZE, "%u", value); 
  dtostrf (value, 1, 0, dBuf);
  sendOne (parameter, dBuf, message, checkAnswer);
}

void KSduino::sendOne (const char *parameter, unsigned long value, 
                       const char *message, int checkAnswer )
{
  char dBuf [XBUF_SIZE*2];
  
  //snprintf (dBuf, XBUF_SIZE*2, "%ul", value); 
  dtostrf (value, 1, 0, dBuf);
  sendOne (parameter, dBuf, message, checkAnswer);
}


/**
 * Starts a connection to write group of packets to the server.
 *
 * This function starts process of transfering group of parameters to server. 
 * This process begins with this function and finishs  after calling the 
 * KSduino::endPacket function. Between these functions  you can send several 
 * parameters by using KSduino::addParameter function, and one packets message 
 * by using KSduino::addMessage function.
 *
 * This is most handy method of sending group of parameters to the 
 * <a href="http://ksduino.org">KSduino</a> server. 
 *
 * This function does not return any values.
 * 
 * \see \ref simple_04.ino example for more information about using this function.
 *
 * \ref KSduino::addParameter, KSduino::addMessage, KSduino::endPacket
 *
 */
void KSduino::beginPacket (void)
{
  char xBuf [XBUF_SIZE]; 

  _startTime = millis();
  packet++; if (!packet) packet = 1;
    
  ksd_print ("Sending packet ");
  ksd_println ((int)packet);

  Udp.beginPacket(serverIpAddr, serverPort);
  // Write ID
  snprintf (xBuf, XBUF_SIZE, "id=%d,", deviceID); 
  Udp.write (xBuf);   
  // Write Package number
  snprintf (xBuf, XBUF_SIZE, "pkg=%d,", packet); 
  Udp.write (xBuf);    
  // Write password
  snprintf (xBuf, XBUF_SIZE, "pwd=%d", devicePasswd+deviceID+packet); 
  Udp.write (xBuf);    
}


/**
 * Add parameter to send to server
 *
 * @param [in] parameter Parameter
 * @param [in] value Value of the parameter
 *
 * This function does not return any values.
 *
 * This function is part of functions group: KSduino::beginPacket 
 * KSduino::addParameter KSduino::endPacket. It should be called between 
 * KSduino::beginPacket and KSduino::endPacket functions. This function
 * adds parameter and it value to paket started with KSduino::beginPacket 
 * function.
 *
 * <b>Variants of calling:</b>
 *
 * \code
 * 	void 	addParameter (const char *parameter, const char *value)
 *	void 	addParameter (const char *parameter, byte value)
 *	void 	addParameter (const char *parameter, int value)
 *	void 	addParameter (const char *parameter, unsigned int value)
 *	void 	addParameter (const char *parameter, unsigned long value)
 *	void 	addParameter (const char *parameter, float value)
 * \endcode
 *
 * These variants is for different parameters types.
 *
 * See KSduino::beginPacket and KSduino::endPacket to get additional information.
 *
 * \see \ref simple_04.ino example which demonstrate of using this function.
 * 
 */
// char value for any parameters or for messager 
void KSduino::addParameter (const char *parameter, const char *value)
{
  Udp.write (",");
  Udp.write (parameter);
  Udp.write ("=");
  Udp.write (value);
}
// float value for f-type
void KSduino::addParameter (const char *parameter, float value)
{
  char fBuf [XBUF_SIZE];
  
  /* ERRROR: If float value ~= 0.00 the dtostrf - returns INF!!! 
     This code does not work:
     if (value == 0.0) strcpy (fBuf,"0.00");
     else dtostrf (value, 1, 2, fBuf);
  */
  dtostrf (value, 1, 2, fBuf);
  if (strstr(fBuf,"INF") != NULL) strcpy (fBuf,"0.00"); 
  addParameter (parameter, fBuf);
}
// byte value for d-type and b-type
void KSduino::addParameter (const char *parameter, byte value)
{
  char dBuf [XBUF_SIZE];
  
  //snprintf (dBuf, XBUF_SIZE, "%d", value);
  dtostrf (value, 1, 0, dBuf);
  addParameter (parameter, dBuf);
}
// int value for i-type
void KSduino::addParameter (const char *parameter, int value)
{
  char dBuf [XBUF_SIZE];
  
  //snprintf (dBuf, XBUF_SIZE, "%d", value);
  dtostrf (value, 1, 0, dBuf);
  addParameter (parameter, dBuf);
}
// unsigned int value for u-type
void KSduino::addParameter (const char *parameter, unsigned int value)
{
  char dBuf [XBUF_SIZE];
  
  //snprintf (dBuf, XBUF_SIZE, "%u", value);
  dtostrf (value, 1, 0, dBuf);
  addParameter (parameter, dBuf);
}
// unsigned long value for unsigned long
void KSduino::addParameter (const char *parameter, unsigned long value)
{
  char dBuf [XBUF_SIZE*2];
  
  //snprintf (dBuf, XBUF_SIZE*2, "%ul", value);
  dtostrf (value, 1, 0, dBuf);
  addParameter (parameter, dBuf);
}


/**
 * Add message to send to server
 *
 * @param [in] message Message
 *
 * This function does not return any values.
 *
 * This function is part of functions group: beginPacket addParameter addMessage 
 * endPacket. It should be called between beginPacket and endPacket functions. 
 * This function adds message to paket started with beginPacket function. This
 * message sends to server with KSduino::endPacket functions and displays at the
 * Device Parameter page.
 *
 * See KSduino::beginPacket, KSduino::addParameter and KSduino::endPacket to get 
 * additional information.
 *
 * \see \ref simple_05.ino example which demonstrate of using this function.
 *
 */
void KSduino::addMessage (const char *message)
{
  if (message[0] != 0)
  {
    Udp.write (",");
    Udp.write ("msg");
    Udp.write ("=\"");
    Udp.write (message);
    Udp.write ("\"");
  }
}


#ifdef __arduino__W5100__
int KSduino::_Udp_endPacket()
{
  unsigned long st = millis();
  W5100.execCmdSn(_s, Sock_SEND);
  while ( (W5100.readSnIR(_s) & SnIR::SEND_OK) != SnIR::SEND_OK ) 
  {
    if (W5100.readSnIR(_s) & SnIR::TIMEOUT)
    {
      W5100.writeSnIR(_s, (SnIR::SEND_OK|SnIR::TIMEOUT));
      /* Timeout */
      return 0;
    }
    
    /* W5100 error - try to reconnect */
    if ( (millis() - st) >= TIMEOUT_BEFORE_RESET )
    {
      int doReset = 1;
      
      if (_beforeResetFunc != NULL) doReset  =  _beforeResetFunc ();      
      
      if (doReset == 1) resetArduino ();
      
      return -1;
    }
  }
  W5100.writeSnIR(_s, SnIR::SEND_OK);

  /* Sent ok */
  return 1;
}
#define Udp_endPacket() _Udp_endPacket()
#else
  #define Udp_endPacket() Udp.endPacket()
#endif


/**
 * End of parameters packet.
 *
 * @param [in] checkAnswer 1,2 or 0; 2 is default value
 *
 * \b checkAnswer values:
 *
 * @li 1 - wait server for answer
 * @li 2 - not wait server for answer but check the answer in update function
 * @li 0 - do nothing, just send packet
 *
 * @return 0 - if checkAnswer == 1 and server does not send answer, or return 1 
 *             in othee case
 *
 * This function called after writing parameters with KSduino::addParameter 
 * function. It completes group of functions: beginPacket addParameter endPacket.
 * This function sends packet with group of parameters to server. 
 *
 * See KSduino::peginPacket and KSduino::addParameter to get additional information.
 *
 * \see \ref simple_04.ino example which demonstrate of using this function.
 * 
 */
int KSduino::endPacket (int checkAnswer)
{
  int retval = 1;

  #ifdef __arduino__W5100__
  W5100.getRXReceivedSize(_s);
  #endif
  
  Udp_endPacket(); 
  
  _totalPackets++;
  _lastSend = millis();
  
  // read ansver from server
  if (checkAnswer == 1) retval = waitPacketAnswer (packet);  // Return 1 if success
  else if (checkAnswer == 2)
  {
    // add packet to Packets Queue
    _packetsQueue [_idx].packet = packet;
    _packetsQueue [_idx].num = 0;
    if (_idx < PACKETS_QUEUE_SIZE-1) _idx++;
    else
    {
      _idx++;
      // remove old records from queue
      if (!checkPacketsQueue ())
      {
        // remove first record from queue
	_packetsQueue[0].num = OLD_QUEUES_PACKET;
	checkPacketsQueue ();
      }
    }
  }

  // calculate one loops time
  _loopTime = millis() - _startTime;
  
  // print messages
  #if PRINT_SERIAL_MESSAGES
  ksd_print ("Total packets = ");
  ksd_println (_totalPackets);    
  if (_lostPackets)
  {
    ksd_print ("Total lost packets = ");
    ksd_println (_lostPackets);    
  }  
  if (_wrongAnswer)
  {
    ksd_print("Total wrong answer = ");
    ksd_println(_wrongAnswer);    
  }  
  if (_idx)
  {
    ksd_print ("Bytes in packets Queue = ");
    ksd_println ((int)_idx);    
  }  
  ksd_print ("After ");
  ksd_print (_loopTime);
  ksd_println (" ms");
  ksd_println (""); 
  #endif

  return retval;
}


/**
 * Send parameter and message to other device
 *
 * @param [in] device_id Other device ID
 * @param [in] device_passwd Other device password
 * @param [in] parameter Parameter
 * @param [in] value Parameters value
 * @param [in] message Packets message or NULL if not used
 * @param [in] checkAnswer 1,2 or 0; 2 is default value
 *
 * \b checkAnswer values:
 *
 * @li 1 - wait server for answer
 * @li 2 - not wait server for answer but check the answer in update function
 * @li 0 - do nothing, just send packet
 *
 * <b>Variants of calling:</b>
 *
 * \code
 * void 	sendToOther (int device_id, int device_passwd, const char *parameter, const char *value, const char *message=NULL, int checkAnswer=2)
 * void 	sendToOther (int device_id, int device_passwd, const char *parameter, byte value, const char *message=NULL, int checkAnswer=2)
 * void 	sendToOther (int device_id, int device_passwd, const char *parameter, int value, const char *message=NULL, int checkAnswer=2)
 * void 	sendToOther (int device_id, int device_passwd, const char *parameter, unsigned int value, const char *message=NULL, int checkAnswer=2)
 * void 	sendToOther (int device_id, int device_passwd, const char *parameter, unsigned long value, const char *message=NULL, int checkAnswer=2)
 * void 	sendToOther (int device_id, int device_passwd, const char *parameter, float value, const char *message=NULL, int checkAnswer=2)
 * \endcode
 *
 * These variants is for different parameters types.
 *
 * This function does not return any values.
 *
 */
void KSduino::sendToOther (int device_id, int device_passwd, 
                           const char *parameter, const char *value, 
                           const char *message, int checkAnswer)
{
  beginPacket ();                             // Start (init) Packet
  addParameter ("T1", device_id);             // Add T1 transit parameter
  addParameter ("P1", device_passwd);	      // Add P1 transit parameter
  addParameter (parameter, value);	      // Add parameter
  if (message != NULL) addMessage (message);  // Add message
  endPacket (checkAnswer);            	      // Send Packet to server   
}
void KSduino::sendToOther (int device_id, int device_passwd, 
                           const char *parameter, byte value, 
                           const char *message, int checkAnswer)
{
  char fBuf [XBUF_SIZE];
  
  dtostrf (value, 1, 0, fBuf);
  sendToOther (device_id, device_passwd, parameter, fBuf, message, checkAnswer);
}
void KSduino::sendToOther (int device_id, int device_passwd, 
                           const char *parameter, int value, 
                           const char *message, int checkAnswer)
{
  char fBuf [XBUF_SIZE];
  
  dtostrf (value, 1, 0, fBuf); 
  sendToOther (device_id, device_passwd, parameter, fBuf, message, checkAnswer);
}
void KSduino::sendToOther (int device_id, int device_passwd, 
                           const char *parameter, unsigned int value, 
                           const char *message, int checkAnswer)
{
  char fBuf [XBUF_SIZE];
  
  dtostrf (value, 1, 0, fBuf); 
  sendToOther (device_id, device_passwd, parameter, fBuf, message, checkAnswer);
}
void KSduino::sendToOther (int device_id, int device_passwd, 
                           const char *parameter, unsigned long value, 
                           const char *message, int checkAnswer)
{
  char fBuf [XBUF_SIZE];
  
  dtostrf (value, 1, 0, fBuf); 
  sendToOther (device_id, device_passwd, parameter, fBuf, message, checkAnswer);
}
void KSduino::sendToOther (int device_id, int device_passwd, 
                           const char *parameter, float value, 
                           const char *message, int checkAnswer)
{
  char fBuf [XBUF_SIZE];
  
  dtostrf (value, 1, 2, fBuf); 
  if (strstr(fBuf,"INF") != NULL) strcpy (fBuf,"0.00");
  sendToOther (device_id, device_passwd, parameter, fBuf, message, checkAnswer);
}


/**
 * Get results from server
 *
 * This is Loops End function to read answer & commands from server, get 
 * messages from other device and send ping command to server. This function is 
 * necessary. It should be placed at the end of arduinos loop function.
 *
 * This function does not return any values.
 *
 * \see \ref simple_01.ino example for more information about using this function.
 *
 */
void KSduino::update()
{
  getAnswer ();
  pingServer ();
}


// Check reception and select wrong answer
#define 	_checkReception(packetNum, buf)  \
        {  \
          if (!checkReception (packetNum, buf)) \
	  {  \
            _wrongAnswer++;  \
            if (_lostPacketCallback != NULL) { _lostPacketCallback (2, packetNum); } \
            \
	    ksd_print ("Wrong Answer#: "); \
	    ksd_println (packetNum); \
	    ksd_print ("Data: "); \
	    ksd_println (buf); \
	    ksd_println (""); \
	  }  \
        }
        

/* *
 * Wait answer from server to selected packet
 */
int KSduino::waitPacketAnswer (byte packet)
{
  int retval = 0;
  int packetSize = 0;
  int numToRepeate = 0;
  char buf [BUF_SIZE];       // Buffer to hold incoming packet  

  #ifdef __arduino__W5100__
  _readbuf = buf;
  _readbufSize = BUF_SIZE;
  #endif
	
  // Wait for answer
  while (1)
  {
    int packetNum = 0;
    unsigned long waitTime = millis();

    while ( (packetSize = _parsePacket()) > 0 )
    {
      int maxWaitTime=(_loopTime <= MAX_WAIT_TIME)?_loopTime+50:MAX_WAIT_TIME;
        
      if (millis() - waitTime > maxWaitTime)
      {
        ksd_print("The Packet ");
        ksd_print(packet);
        ksd_println(" was lost ");
        break;
      }
     }

     // Check reseived  data
     if(packetSize)
     {    
       packetNum = readPacket (packetSize, buf, BUF_SIZE);
        
       if (packetNum != packet)
       {
         _checkReception (packetNum, buf);
       }
       else { retval = 1; break; }
      }
      
      if (++numToRepeate > MAX_NUM_TO_REPEAT_IN_WAIT_PACKET) 
      {         
        _lostPackets++; 
        if (_lostPacketCallback != NULL) _lostPacketCallback(1, packetNum);
	break; 
      }
    }  
    
    return retval;
}


/**
 * Parse received packet for W5100
 */
#ifdef __arduino__W5100__
int KSduino::parsePacket ()
{
  _readLen = 0;
  
  if (W5100.getRXReceivedSize(_s) > 0)
  {
    _readLen = recvfrom(_s, (uint8_t *)_readbuf, _readbufSize, 
	                    _si_recvIP,
 		            &_si_recvPort);  
  }
  
  if (_readLen > 0) return _readLen; 
  else return 0;	
}
#endif


/**
 * Read packet from ethernet controller
 */
int KSduino::readPacket (int packetSize, char *buf, int bufferSize)
{
  
  int packetNum = 0;

  // print message
  ksd_print("Received ");
  ksd_print(packetSize);
  ksd_print(" byte packet from: ");
  #if PRINT_SERIAL_MESSAGES
  #ifdef __arduino__W5100__
  IPAddress ip = _si_recvIP;
  word port = _si_recvPort;
  #endif
  #ifdef __arduino__ENC28J60__
  char *ip = "";
  word port = 0;
  #endif
  #ifdef __linux__
  char *ip = Udp.remoteIP();
  word port = Udp.remotePort();
  #endif
  #endif
  ksd_print(ip);
  ksd_print(":");
  ksd_println(port);
        
  // read the packet into packetBufffer & check it packet number
  #ifdef __arduino__W5100__
  if (_readLen > 0)
  #else
  buf[0] = 0;
  if (Udp.read (buf, bufferSize) > 0)
  #endif	
  {	
    buf [packetSize<bufferSize ? packetSize:(bufferSize-1)] = 0;
    packetNum = getPacketNum ((const unsigned char*)buf, bufferSize);
  }	

  // print message
  ksd_print("Packet #: ");
  ksd_println(packetNum);
  ksd_print("Data: ");
  ksd_println(buf);
  ksd_println("");
        
  return packetNum;
}


/**
 * Check current packet and Remove old answers from Packets Queue
 * 
 * @param [in] packet packet number
 *
 * @return 0 if packet not found in the packets queue
 * 
 */
int KSduino::checkPacketsQueue (int packet)
{
  int retval = 0;

  // Check current packet
  if (packet)
  {
    // Check queue & Mark record if packet found or not found
    int i = 0;
    for (i = 0; i < _idx; i++)
    {
      if (_packetsQueue[i].packet == packet) 
      { 
        // Packet found. Mark the record to remove from Queue
        _packetsQueue[i].num = FOUND_QUEUES_PACKET; 
        retval++; 
      }
      // Packet not found. Mark the record as not found - increment it's wait variavle
      else _packetsQueue[i].num++;
     }
  }

  // Remove old packects from queue
  int i = 0;
  while (i < _idx)
  {
    if (_packetsQueue[i].num >= OLD_QUEUES_PACKET)
    {
      if (_packetsQueue[i].num != FOUND_QUEUES_PACKET)
      {	
        ksd_print ("Lost packet #: ");
    	ksd_println ((int)_packetsQueue[i].packet);
    	_lostPackets++;
        if (_lostPacketCallback != NULL) _lostPacketCallback (1, (int)_packetsQueue[i].packet);
      }	
      if (!packet) retval++;

      // remove one first packet from queue (if queue holds only one packet)
      if (i == 0 && _idx == 1)
      {	
        _idx = 0;
	break;
      }
      // remove a packet from queue
      else
      {	
        if (i < (_idx-1) )
        {	
          memmove (&_packetsQueue[i], &_packetsQueue[i+1], 
          sizeof(ksdPacketsQueue)*(_idx-(i+1)) );
	}	
	_idx--;
      }
    }
    else i++;
  }

  return retval;
}


/** 
 * Get answer from server
 * 
 * @return packet number or -1 if anwer not present, or 0 if packet number not found in server answer
 * 
 */
int KSduino::getAnswer ()
{
  int packetNum = -1;
  int packetSize = 0;
  char buf [BUF_SIZE];       // Buffer to hold incoming packet  

  #ifdef __arduino__W5100__
  _readbuf = buf;
  _readbufSize = BUF_SIZE;
  #endif
	
  // check for answer
  while ( (packetSize = _parsePacket()) > 0)
  {
    packetNum = readPacket (packetSize, buf, BUF_SIZE);
    
    // find the packet number in the packet queue
    int success = checkPacketsQueue (packetNum);
		
    if (!success)
    {
      _checkReception (packetNum, buf);
    }
  }
    
  return packetNum;
}


/**
 * Get packet number from buffer
 * 
 * @param [in] buffer pointer to packet buffer
 *
 * @return packet number or 0 if packet number not found in this packet
 *
 * <b>Description:</b>
 * 
 * Read packet number in buffer and return it as integer 
 * 
 */
int KSduino::getPacketNum (const unsigned char *buffer, unsigned int bufferSize)
{
  int packet = 0, i = 0;
  
  while(1)
  {
    unsigned char c = buffer[i];
    if (i >= bufferSize);
    else if (c == 0) break;
    else if (c == ',')
    {
      for(int j = 0; j < i; j++) packet = packet * 10 + (int) (buffer[j] - '0');
      break;
    }
    else if ( !(c>='0' && c <= '9') ) break;
    i++;
  }
  
  return packet;
}


/**
 * Get number of lost packets
 *
 * @return number of lost packets - packets without answer from server
 *
 */
unsigned long KSduino::getLostPackets (void)
{
  return _lostPackets;
}


/**
 * Get number of wrong packets
 *
 * @return number of wrong packets - packates which not waiting by KSduino library
 *
 */
unsigned long KSduino::getWrongAnswer (void)
{
  return _wrongAnswer;
}


/**
 * Get total number of sending packets
 *
 * @return total number of sending packets
 *
 */
unsigned long KSduino::getTotalPackets(void)
{
  return _totalPackets;
}


/**
 * Get last loop time
 *
 * @return last loop time in millisecond
 *
 */
unsigned long KSduino::getLoopTime (void)
{
  return _loopTime;
}


/**
 * Get current size of Reseive Queue buffer
 *
 * @return current size of Reseive Queue buffer
 *
 */
int KSduino::getBytesInQueue(void)
{
  return (int)_idx;
}


/**
 * Set reception callback function
 *
 * @param [in] receptionFunc Reception callback function
 *
 * Reception callback function calls when KSduino library get messages from web 
 * server or other devices
 *
 * This function does not return any values.
 *
 */
void KSduino::setReceptionCallback (ksdReceptionFunc receptionFunc)
{
  _receptionFunc = receptionFunc;
}


#ifdef __arduino__W5100__
/**
 * Set before reset callback function
 *
 * @param [in] beforeResetFunc Before reset function
 *
 * Before reset function is callback function which will called before reset 
 * Arduino with W5100 Ethernet shield. This reset happend sometimes due to 
 * incorrect operation of W5100.
 *
 * This function does not return any values.
 *
 */
void KSduino::setBeforeResetCallback (ksdBeforeReset beforeResetFunc)
{
  _beforeResetFunc = beforeResetFunc;
}
#endif


/**
 * Set lost packet callback function
 *
 * @param [in] lostPacketCallback Lost packet function
 *
 * Set callback function which will called when lost or wrong packets will detected.
 *
 * This function does not return any values.
 *
 */
void KSduino::setLostPacketCallback (ksdLostPacketCallback lostPacketCallback)
{
  _lostPacketCallback = lostPacketCallback;
}


/**
 * Parse received packet
 */ 
int KSduino::checkReception (int packet_code, char *packet_data)
{	
  int retval=0;
	
  if (_receptionFunc != NULL)
  {
    char *msg = NULL;
    char param;
    int  num;
    float value_float;
    int   value_int;
    unsigned int   value_uint;
    unsigned long  value_ulong;
    int   id;
    char  *i, *i2;

    // Packets data example: 7701,T1:id=3300,d1=1,msg="Test message" 
    if (packet_code == 7701)
    {	
      ksd_print ("Received T1 packet: ");
      ksd_println (packet_data);
			
      // Parse the packet and call user function
      const char *fs = "7701,T1:id=";
      int fsLen = strlen(fs);
      if (strstr(packet_data,fs) == packet_data)
      {
        // Get id
	i = &packet_data[fsLen];
	if ( (i = strchr(i,',')) != NULL)
	{	
	  i[0] = 0;
	  sscanf(&packet_data[fsLen],"%d",&id);
	  i[0] = ',';
	  // Get parameter
	  i++;       
	  param = i[0];
	  if ( (i2 = strchr(i,'=')) != NULL) 
	  {
	    i2[0] = 0;
	    sscanf(&i[1],"%d",&num);
	    i2[0] = '=';
	    // Get parametr value
	    i2++;
	    i = strstr(i2,",msg=\"");
	    if (i != NULL ) i[0] = 0;
	    switch (param)
	    {	
	      case 'f':
	        sscanf(i2,"%f",&value_float);
		break;
								
	      case 'u':
		sscanf(i2,"%u",&value_uint);
		break;
								
	      case 'l':
		sscanf(i2,"%lu",&value_ulong);
		break;
								
	      default:
		sscanf(i2,"%d",&value_int);
	    }	
	    if (i != NULL )
	    {	
	      i[0] = ',';
	      // Get message
	      i += 6;
	      msg=i;
				int msglen = strlen(msg);
				if (msglen) 
				if (msg[msglen-1] == '\"') msg[msglen-1] = 0;
	    }
	    retval = 1;
						
	    switch (param)
	    {
	      case 'f':
		_receptionFunc (id, param, num, &value_float, msg);
		break;
								
	      case 'u':
		_receptionFunc (id, param, num, &value_uint, msg);
		break;

	      case 'l':
		_receptionFunc (id, param, num, &value_ulong, msg);
		break;

	      default:	
		_receptionFunc (id, param, num, &value_int, msg);
	    }
          }	
        }	
      }
    }
  }

  return retval;
}


/*
 * Send ping signal to server if program does not send something 1 second longer
 */
int KSduino::pingServer ()
{
  if ( (millis() - _lastSend) > 1000 ) sendBuffer ("ping");
	
  return 1;
}

/**
 * Arduino software reset
 */
#ifdef __arduino__
void  KSduino::softReset ()
{
  resetArduino ();
}
#endif


/**
 * @example simple_01.ino
 *
 * Simple-01 is an Example for Getting Started with KSduino Library.
 *
 * This is simple example of using KSduino Library for Arduino.
 * This sample uses one digital Parameter D1 which periodicaly send values to 
 * <a href="http://ksduino.org">KSduino</a> server using KSduino::sendOne function. 
 *
 * Sample-01 is usefull for KSduino beginners as first KSduino sketch.
 *
 * This example uses four KSduino functions: \ref KSduino::KSduino "KSduino",
 * \ref KSduino::begin "begin",
 * \ref KSduino::sendOne "sendOne" and 
 * \ref KSduino::update "update"
 *
 */

/**
 * @example simple_02.ino
 *
 * Simple-02 is an Example for Getting Started with KSduino Library.
 *
 * This is simple example of using KSduino Library for Arduino.
 * This sample uses one digital Parameter D1 which periodicaly send values to 
 * <a href="http://ksduino.org">KSduino</a> server using KSduino::sendBuffer function.
 *
 * Sample-02 is usefull for KSduino beginners as first KSduino sketch.
 *
 * This example uses four KSduino functions: \ref KSduino::KSduino "KSduino",
 * \ref KSduino::begin "begin",
 * \ref KSduino::sendBuffer "sendBuffer" and 
 * \ref KSduino::update "update"
 *
 */

/**
 * @example simple_03.ino
 *
 * Simple-03 is an Example for Getting Started with KSduino Library.
 *
 * This is simple example of using KSduino Library for Arduino.
 * This sample uses one digital Parameter D1 which periodicaly send values to 
 * <a href="http://ksduino.org">KSduino</a> server using KSduino::sendBuffer with 
 * String object function.
 *
 * Sample-03 is usefull for KSduino beginners as first KSduino sketch.
 *
 * This example uses four KSduino functions: \ref KSduino::KSduino "KSduino",
 * \ref KSduino::begin "begin",
 * \ref KSduino::sendBuffer "sendBuffer" and 
 * \ref KSduino::update "update"
 *
 */

/**
 * @example simple_04.ino
 *
 * Simple-04 is an Example for Getting Started with KSduino Library.
 *
 * This is simple example of using KSduino Library for Arduino.
 * This sample uses one digital Parameter D1 which periodicaly send values to 
 * <a href="http://ksduino.org">KSduino</a> server using KSduino::beginPacket,
 * KSduino::addParameter, KSduino::endPacket functions.
 *
 * Sample-04 is usefull for KSduino beginners as first KSduino sketch.
 *
 * This example uses six KSduino functions: \ref KSduino::KSduino "KSduino",
 * \ref KSduino::begin "begin",
 * \ref KSduino::beginPacket "beginPacket", 
 * \ref KSduino::addParameter "addParameter",
 * \ref KSduino::endPacket "endPacket" and 
 * \ref KSduino::update "update"
 *
 */

/**
 * @example simple_05.ino
 *
 * Simple-05 is an Example for Getting Started with KSduino Library.
 *
 * This is simple example of using KSduino Library for Arduino.
 * This sample uses one digital Parameter D1 which periodicaly send values to 
 * <a href="http://ksduino.org">KSduino</a> server using KSduino::beginPacket,
 * KSduino::addParameter, KSduino::endPacket functions. This sample add message
 * to sending packet with KSduino::addMessage function.
 *
 * Sample-05 is usefull for KSduino beginners as first KSduino sketch.
 *
 * This example uses seven KSduino functions: \ref KSduino::KSduino "KSduino",
 * \ref KSduino::begin "begin",
 * \ref KSduino::beginPacket "beginPacket", 
 * \ref KSduino::addParameter "addParameter",
 * \ref KSduino::addMessage "addMessage",
 * \ref KSduino::endPacket "endPacket" and 
 * \ref KSduino::update "update"
 *
 */

/**
 * @example simple_06.ino
 *
 * Simple-06 is an Example for Getting Started with KSduino Library.
 *
 * This is simple example of using KSduino Library for Arduino.
 * This sample uses one digital Parameter D1 which periodicaly send values to 
 * <a href="http://ksduino.org">KSduino</a> server using KSduino::beginPacket,
 * KSduino::addParameter, KSduino::endPacket functions. This sample add message
 * to sending packet with KSduino::addMessage function. This sample used 
 * KSduinoParameter class to send B1 parameter. This parameter sends to server 
 * when its current value is greater than the previous value for 5.
 *
 * Sample-06 is usefull for KSduino beginners as first KSduino sketch.
 *
 * This example uses eight KSduino functions: \ref KSduino::KSduino "KSduino",
 * \ref KSduino::begin "begin",
 * \ref KSduino::beginPacket "beginPacket", 
 * \ref KSduino::addParameter "addParameter",
 * \ref KSduinoParameter::addParameter "KSduinoParameter::addParameter",
 * \ref KSduino::addMessage "addMessage",
 * \ref KSduino::endPacket "endPacket" and 
 * \ref KSduino::update "update"
 *
 */

/**
 * @example send_to_other.ino 
 *
 * Send to Other example shows how to send values from one Arduino to other 
 * Arduino device with KSduino Library.
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
 *    are not created at <a href="http://ksduino.org">KSduino</a> web server. Programmer can use any parameter to send it to other device
 *
 * - receiver device uses the ksdReceptionCallback function which calls when KSduino library get a packet
 *    from other device. Function parameters contains ID of device which sent the packet, parameter (one letter)
 *    and its number, value and message.
 *
 * In this example we send: b2 with value = b_value and message = "Received from other devices".
 * And receive: device_id  = deviceID, 'b', 2, value = b_value,message= "Received from other devices"
 *
 * To execute this test D1, B1, B2 Device parameters should be createt at <a href="http://ksduino.org">KSduino</a> web server.
 *
 * The send_to_other_sender and send_to_other_receiver is seporate part of this test (to use in two Arduinos).
 * 
 */

/**
 * @example send_to_other_sender.ino
 */

/**
 * @example send_to_other_receiver.ino
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

/**
 * @mainpage 

 * <div style="width: 640px;">
 *
 * @section Goals Design Goals
 *
 * This library was designed to Connect your controllers to 
 * the <a href="http://ksduino.org">KSduino</a> Device Network.<br>
 * <br>
 * This library is designed to be...
 * @li Maximally compliant with the Arduinos ethernet shield
 * @li Easy for beginners to use
 * @li Consumed with a public interface that's similiar to other Arduino standard libraries
 * <br>
 * <br>
 *
 * @section Downloads Downloads
 *
 * Download lates stable release from the <a href="http://ksduino.org/?downloads">DOWNLOADS</a> page
 * or
 * non stable but newest prerelease from the <a href="http://ksduino.org/?development">DEVELOPMENT</a> page.
 * <br>
 * <br>
 *
 * @section Connect How to connect your Arduinos
 *
 * To coonect your Arduino to the <a href="http://ksduino.org">KSduino</a> Device 
 * Network see the <a href="http://help.ksduino.org/post/36586795328/how-to-connect-your-device-to-the-ksduino-device">
 * How to connect your device to the KSduino Device Network</a>
 * article at the <a href="http://help.ksduino.org">HELP</a> page.
 * <br>
 * <br>
 *
 * @section The KSduino library
 *
 * KSduino library consists of a main class KSduino, subclass KSduinoParameter 
 * and examples.
 * The main class contains all the necessary functions for sending and receiving 
 * server messages. Subclass has additional functions for working with parameters.
 * <br>
 * <br>
 *
 * @section Examples Examples
 *
 * There are several examples of using this Library. It will help you to connect
 * your devices to <a href="http://ksduino.org">KSduino</a> and to transfer all 
 * necessary controller parameters to KSduinos web site:
 *
 * @li \ref simple_01.ino "simple_01.ino" simple example of using KSduino 
 * Library for Arduino,<br> 
 * it uses \ref KSduino::sendOne "sendOne"
 * <br>
 *
 * @li \ref simple_02.ino "simple_02.ino" simple example of using KSduino 
 * Library for Arduino,<br> 
 * it uses \ref KSduino::sendBuffer "sendBuffer"
 * <br>
 *
 * @li \ref simple_03.ino "simple_03.ino" simple example of using KSduino 
 * Library for Arduino,<br> 
 * uses \ref KSduino::sendBuffer "sendBuffer" with String object
 * <br>
 *
 * @li \ref simple_04.ino "simple_04.ino" simple example of using KSduino 
 * Library for Arduino,<br> 
 * it uses \ref KSduino::beginPacket, KSduino::addParameter, KSduino::endPacket functions
 * <br>
 *
 * @li \ref simple_05.ino "simple_05.ino" simple example of using KSduino 
 * Library for Arduino,<br> 
 * it uses \ref KSduino::beginPacket, KSduino::addParameter, KSduino::addMessage, 
 * KSduino::endPacket functions
 * <br>
 *
 * @li \ref simple_06.ino "simple_06.ino" simple example of using KSduino 
 * Library for Arduino,<br> 
 * it uses \ref KSduino::beginPacket, KSduino::addParameter, KSduino::addMessage, 
 * KSduino::endPacket functions to send D1 parameter to server and 
 * KSduinoParameter class to send B1 parameter to server
 * <br>
 *
 * @li \ref send_to_other.ino "send_to_other.ino" example shows how to send 
 * values from one Arduino to other Arduino device with KSduino Library,<br>
 * it used KSduino::beginPacket, KSduino::addParameter, KSduino::addMessage,
 * KSduino::endPacket functions to send D1 parameter and KSduinoParameter class 
 * to send D1,B1 parameters to server. Function KSduino::sendOne used to send B2 
 * parameter. This test uses setReceptionCallback function to setup receptionFunc 
 * function which receive values from other Arduino device.
 * <br>
 *
 * @li \ref control_01.ino "control_01.ino" example shows how to send values
 * from KSduino server to the Arduino Device with KSduino Library,<br>
 * it used KSduino::beginPacket, KSduinoParameter::addParameter, KSduino::addMessage,
 * KSduino::endPacket functions to send D1 parameter. Function KSduino::sendOne used
 * to send B1 parameter back to <a href="http://ksduino.org">KSduino</a> Server. This test uses setReceptionCallback 
 * function to setup receptionFunc function which receive values from the <a href="http://ksduino.org">KSduino</a> 
 * server.
 * <br>
 *
 * </div>
 *
 */

