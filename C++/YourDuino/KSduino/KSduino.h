/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * KSduino.h
 *
 * KSduino-library for Arduino ver 0.0.1
 *
 * Copyright (C) Kirill Scherba 2012 <kirill@scherba.ru>
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
 * @file KSduino.h
 *
 * Class declaration for KSduino
 *
 */
 
#ifndef KSduino_h
#define KSduino_h

#define PRINT_SERIAL_MESSAGES 0

#define PACKETS_QUEUE_SIZE  32 //64
#define OLD_QUEUES_PACKET  30 //60
#define FOUND_QUEUES_PACKET  255

#define __arduino__ 
//#define __linux__

#ifdef __arduino__
#
#define __W5100__
//#define __ENC28J60__
#
#include <Arduino.h>
#
#ifdef __W5100__
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <utility/w5100.h>
#endif
#
#ifdef __ENC28J60__
//#include <EtherCard.h>
#include "KSduinoEthernetUdp_ethercard.h"
#endif
#
#ifdef __W5100__
  #define __arduino__W5100__
#endif
#
#ifdef __ENC28J60__
  #define __arduino__ENC28J60__
#endif
#
#endif

#ifdef __linux__
#
#include <stdio.h>
#include "KSduinoEthernetUdp.h"
#
#endif

/**
 * Reception callback function type definition.
 *
 * @param [in] device_id Which id send this packet
 * @param [in] param Parameter type
 * @param [in] num Parameter number
 * @param [in] value Value of parameter
 * @param [in] message Message or NULL of not used
 *
 * @return This function should return 1
 *
 */
typedef int (*ksdReceptionFunc) (int device_id, 
                                 char param, int num, 
                                 void *value, char *message);
                                 
/**
 * Before reset callback function type definition.
 *
 * @return 1 - to continue reset, 0 - to skip reset
 *
 */
typedef int (*ksdBeforeReset) ();  // if return 1 - do reset, 0 - skip reset

/**
 * Lost packet callback function type definition.
 *
 * @param [in] type Type of callback: 1 - lost pscket, 2 - wrong packet
 * @param [in] packetNum Number of lost (or wrong) packet
 *
 * @return This function should return 1 
 * 
 */ 
typedef int (*ksdLostPacketCallback) (int type, int packetNum);  

typedef struct
{
  byte packet;
  byte num;	
}
ksdPacketsQueue;


/**
 * Main Library Class
 *
 * \author    Kirill Scherba
 * \version   0.0.1
 * \date      2012
 * \copyright Kirill Scherba <kirill@scherba.ru> GNU Public License.
 *
 * \details
 * This is main KSduino Libray class. It used to connect controllers to <a href="http://ksdiono.org">KSduino</a>
 * Device Network.
 */
class KSduino
{
  public:

		/**
		 * @name Configuration and initialization functions
		 *
     * These functions used to initialize KSduino library, it's necessary functions.
     *
  	 */
		/**@{*/		
    KSduino (unsigned int devID, unsigned int devPwd, byte *serverAddr, 
             unsigned int serverP);
    
    void begin (byte *mac, byte *ip, byte *dns, byte *gateway, 
                unsigned int port);
    void begin (byte *mac, byte *ip, byte *dns, unsigned int port);
    void begin (byte *mac, byte *ip, unsigned int port);
    void begin (byte *mac, unsigned int port);
    #ifndef  __arduino__ENC28J60__
    void begin (byte *mac, byte *ip, byte *dns, byte *gateway, byte* subnet, 
                unsigned int port);
    #endif
    /**@}*/

    
		/**
		 * @name Send one parameter functions
		 *
     * These functions used to send one parameters to the <a href="http://ksduino.org">KSduino</a> server
  	 */
		/**@{*/
    void sendOne (const char *parameter, const char *value, 
                  const char *message = NULL, 
                  int checkAnswer = 2);
    void sendOne (const char *parameter, byte value, 
                  const char *message = NULL, 
                  int checkAnswer = 2);
    void sendOne (const char *parameter, int value, 
                  const char *message = NULL, 
                  int checkAnswer = 2);
    void sendOne (const char *parameter, unsigned int value, 
                  const char *message = NULL, 
                  int checkAnswer = 2);
    void sendOne (const char *parameter, unsigned long value, 
                  const char *message = NULL, 
                  int checkAnswer = 2);
    void sendOne (const char *parameter, float value, 
                  const char *message = NULL, 
                  int checkAnswer = 2);

		/**@}*/
		/**
		 * @name Send parameters in text buffer or String functions
		 *
     * These functions used to send text buffer or String with parameters to 
		 * the <a href="http://ksduino.org">KSduino</a> server
  	 */
		/**@{*/
    void sendBuffer (const char *userData, int checkAnswer = 2);
    #ifdef __arduino__
    void sendBuffer (String userData, int checkAnswer = 2);
    #endif
    
		/**@}*/
		/**
		 * @name Send group of parameters functions
		 *
     * These functions used to send group of parameters to the <a href="http://ksduino.org">KSduino</a> server
  	 */
		/**@{*/
    void beginPacket (void);
    void addParameter (const char *parameter, const char *value);
    void addParameter (const char *parameter, byte value);
    void addParameter (const char *parameter, int value);
    void addParameter (const char *parameter, unsigned int value);
    void addParameter (const char *parameter, unsigned long value);
    void addParameter (const char *parameter, float value);
    void addMessage (const char *message);
    int  endPacket (int checkAnswer = 2);
		/**@}*/

		/**
		 * @name Send to Other devices functions
     *
     * These functions used to send parameter to other device
  	 */
		/**@{*/
    void sendToOther (int device_id, int device_passwd, const char *parameter, 
		      const char *value, const char *message = NULL, 
		      int checkAnswer = 2);
    void sendToOther (int device_id, int device_passwd, const char *parameter, 
		      byte value, const char *message = NULL, 
		      int checkAnswer = 2);
    void sendToOther (int device_id, int device_passwd, const char *parameter, 
		      int value, const char *message = NULL, 
		      int checkAnswer = 2);
    void sendToOther (int device_id, int device_passwd, const char *parameter, 
		      unsigned int value, const char *message = NULL, 
		      int checkAnswer = 2);
    void sendToOther (int device_id, int device_passwd, const char *parameter, 
		      unsigned long value, const char *message = NULL, 
		      int checkAnswer = 2);
    void sendToOther (int device_id, int device_passwd, const char *parameter, 
		      float value, const char *message = NULL, 
		      int checkAnswer = 2);
		/**@}*/

    
		/**
		 * @name Get KSduino current status information functions
     *
     * These functions return current status information from KSduino library
  	 */
		/**@{*/
    unsigned long getLostPackets (void);
    unsigned long getWrongAnswer (void);
    unsigned long getTotalPackets(void);
    unsigned long getLoopTime (void);
    int getBytesInQueue (void);
		// Define callback function
    void setLostPacketCallback (ksdLostPacketCallback);
    #ifdef __arduino__W5100__
		// Define callback function
    void setBeforeResetCallback (ksdBeforeReset beforeResetFunc);
    #endif
		/**@}*/    

   
		/**
		 * @name KSduino update and reception
     *
     * These functions used to reseive parameters from server or other devices
  	 */
		/**@{*/
    void update (void);
		// Define callback function
    void setReceptionCallback (ksdReceptionFunc receptionFunc);    
		/**@}*/

    #ifdef __arduino__ENC28J60__
    unsigned int _len;
    unsigned int _pos;
    #endif
    
  private:
  
    EthernetUDP Udp;
    ksdPacketsQueue _packetsQueue [PACKETS_QUEUE_SIZE];
    word _port;
    byte _idx;
    
    byte *serverIpAddr;
    unsigned int deviceID;
    unsigned int serverPort;
    unsigned int devicePasswd;
    
    byte packet;
    
    unsigned long _startTime;
    unsigned long _loopTime;
    unsigned long _lastSend;
    unsigned long _lostPackets;
    unsigned long _totalPackets;
    unsigned long _wrongAnswer;

    ksdReceptionFunc _receptionFunc;
    #ifdef __arduino__W5100__
    ksdBeforeReset _beforeResetFunc;
    #endif
    ksdLostPacketCallback _lostPacketCallback;
    
    int waitPacketAnswer (byte packet);
    int readPacket (int packetSize, char *buffer, int bufferSize);
    int getPacketNum (const unsigned char *buf, unsigned int bufferSize);
    int getAnswer ();
    int checkReception (int packet_code, char *packet_data);
    int pingServer ();
    int checkPacketsQueue (int packet = 0);

    #ifdef __arduino__W5100__
    char *_readbuf;
    int _readbufSize;
    int _readLen;
    SOCKET _s;

    uint8_t  _si_recvIP[4];
    uint16_t _si_recvPort;
		
    int _Udp_endPacket();
    int parsePacket ();
    #endif

    #ifdef __arduino__
    void  softReset ();
    #endif
};


/**
 * Subclass with additional functions for working with parameters
 *
 * \author    Kirill Scherba
 * \version   0.0.1
 * \date      2012
 * \copyright Kirill Scherba <kirill@scherba.ru> GNU Public License.
 *
 * \details
 *
 * This is subclass to manage KSduino parameters. It used to define parameters 
 * which sends to <a href="http://ksdiono.org">KSduino</a> Device Network.
 *
 */
class KSduinoParameter
{
  public:
    
   /**
    * Constructor to define KSduinoParameter subclass
    *
    * @param [in] ksd Reference to KSduino class
    * @param [in] parameter_name Parameter name 
    * @param [in] parameter_accuracy Accuracy of parameter: difference between 
		*								previouse and current value. Values sends to server if 
		*								difference between previouse and current value more than 
		*								parameter_sendall value.    
    * @param [in] rt Repeat time: time in seconds after which one parameters
    *								value wil be sent even though values of parameter_sendall and 
		*								parameter_accuracy
    * @param [in] parameter_sendall Send all values: 0 - send only different 
		*								values (default), 1 - send all values
    *
    */
    KSduinoParameter (KSduino *ksd, char *parameter_name, float parameter_accuracy = 0.00, uint16_t rt = 0, byte parameter_sendall = 0)
    {
      ksduino = ksd;
      
      sendall = parameter_sendall;
			repeattime = rt;
      
      int len = strlen (parameter_name);
      _name = (char*) malloc (len+1);
      strcpy (_name, parameter_name);
      
      if (!sendall)
      {
        #define casebodyst(type) \
        { \
          lastvalue = malloc (sizeof (type)); \
          accuracy = malloc (sizeof (type)); \
          type *ac = (type *) accuracy; \
          *ac = (type) parameter_accuracy; \
        }
      
        switch ( toupper(_name[0]) )
        {
          case 'D':
          case 'B':
            casebodyst(byte);
              break;
          case 'I':
              casebodyst(int);
              break;
          case 'U':
              casebodyst(unsigned int);
              break;
          case 'L':
              casebodyst(unsigned long);
              break;
          case 'F':
              casebodyst(float);
              break;
        }
      }      
    }
    

		/** 
     * Add parameter
     *
     * @param [in] value Parameters value
		 *
     * @return 1 - if this parametr added, or 0 - if this parameter not ready to 
     *         send (see KSduinoParameter::check)
     *
     * This function shoud be send between KSduino::beginPacket and 
		 * KSduino::endPacket functions. This function adds parameter and it value 
     * to paket started with KSduino::beginPacket function.
		 *
     * See KSduino::beginPacket, KSduino::addParameter and KSduino::endPacket 
     * to get additional information.
     *
     */
    template <typename ValueType>
    int addParameter (ValueType value)
    {
      int added;
      if ( (added=check (value)) ) ksduino->addParameter (name(), value);
      return added;
    }

    
		/** 
     * Get this parameter name
		 *
     * @return parameter name char* array
     *
     */
    char *name (void) 
		{ 
			return _name; 
		}

    
    /**
     * Check if this parameter ready to send
     *
     * @param [in] value Parameters value
     *
     * @return 1 - if this Parameters value is ready to send
     *
     */
    template <typename ValueType>
    int check (ValueType value) 
    {
      if (sendall) return 1;
      
      byte show = 0;
      unsigned long tm;
      
      if (repeattime)
      {
        tm = millis();
        if ( (tm - lastaccess) > (unsigned long) repeattime*1000) show = 1; 
      }
      
      #define casebody(type) \
      { \
        type *lastval = (type*) lastvalue; \
        type *acc = (type*) accuracy; \        
        if ( (abs( (type) value - (type)*lastval ) > (type)*acc)  || show) \
        { \
          *lastval = (type) value; \
          show = 1; \
        } \
      }
      
      switch ( toupper(_name[0]) )
      {
        case 'D':
        case 'B':
            casebody(byte);
            break;
        case 'I':
            casebody(int);
            break;
        case 'U':
            casebody(unsigned int);
            break;
        case 'L':
            casebody(unsigned long);
            break;
        case 'F':
            casebody(float);
            break;
      }
      if (show && repeattime) lastaccess = tm; 
      
      return show;
    }
    

    ~KSduinoParameter ()
    {
      if (!sendall)
      {
        free(_name);
        free(lastvalue);
        free(accuracy);
      }
    }
    
  private:
  
    byte sendall;
    char *_name;
    void *lastvalue;
    void *accuracy; 
    KSduino *ksduino;
    uint16_t repeattime;
    unsigned long lastaccess;    
};

#endif
