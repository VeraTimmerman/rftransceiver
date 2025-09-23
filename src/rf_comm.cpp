#include <Arduino.h>
#include <RFM69.h>
#include <RFM69registers.h>
#include <RFM69_ATC.h>
#include <RFM69_OTA.h>
#include "rf_comm.h"
#include "led.h"
#include "debug.h"

#define FREQUENCY       RF69_868MHZ

#define NETWORKID       100
#define ENCRYPTKEY      "sampleEncryptKey"

#define RF_COMM_LOG_LVL     5

RFM69 radio;
bool init_successfull = false;
char send_message[] = "Hello World!";
bool msg_send = false;
unsigned long sentTime = 0;

// put function declarations here:
void send(uint16_t toAddress, char *msg, uint8_t size);
bool receive();

void rf_comm_config(uint16_t current_id)
{  
    msg_send = false;  
    init_successfull = radio.initialize(FREQUENCY, current_id, NETWORKID);
    if(init_successfull)
    {
        #if RF_COMM_LOG_LVL > 1
        char buff[50];
        memset(buff, 0x00, 50);
        snprintf(buff, 49, "\nListening at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
        debug_write(buff);
        debug_write("initializing radio successfull");
        #endif
        radio.encrypt(ENCRYPTKEY);
    }
    else
    {
        debug_write("initializing radio un successfull");
    }
}

void rf_comm_main(uint16_t current_id)
{
    if(current_id == RECEIVE_ID)
    {
      bool received = receive();
      if(received)
      {
        debug_write("RECEIVER: SEND");
        send(SEND_ID, send_message, strlen(send_message));
      }
    }
    else if ( current_id == SEND_ID )
    {
      if(msg_send == 0)
      {
        debug_write("SENDER: SEND");
        send(RECEIVE_ID, send_message, strlen(send_message));
        msg_send = 1;
        sentTime = millis();
      }
      else
      {
        bool received = receive();
        if(received)
        {
          debug_write("SENDER: RECEIVED");
          set_led(USER_LED, true);
          msg_send = 0;
        }
        else if(millis() - sentTime >= 5000)
        {
          debug_write("Timeout");
          msg_send = 0;
        }
      }   
    }
    else
    {
      #if RF_COMM_LOG_LVL > 3
      debug_write("initialization not successfull!");
      #endif
      set_led(USER_LED, true);
    }
}

void send(uint16_t toAddress, char *msg, uint8_t len)
{
  #if RF_COMM_LOG_LVL > 5
  char buff[64];
  memset(buff, 0x00, 64);
  snprintf(buff, 63, "TO: [%u] MSG: [%s] LEN: [%u]", toAddress, msg, len);
  debug_write(buff);
  #elif RF_COMM_LOG_LVL > 3
  debug_write("send with retry");
  #endif

  bool result = 
  radio.send(toAddress, "All About Circuits", 18, false); // radio.sendWithRetry(toAddress, "All About Circuits", 18);
  if(result)
  {
    #if RF_COMM_LOG_LVL > 3
    debug_write("sent was true");
    #endif
  }
  else
  {
    #if RF_COMM_LOG_LVL > 2
    debug_write("failed to send");
    #endif
  }
}

bool receive()
{
  bool result = radio.receiveDone();
  if(result)
  {
    if(radio.ACKRequested())
    {
      radio.sendACK();
      #if RF_COMM_LOG_LVL > 2
      debug_write("ACK sent");
      #endif
    }
    else
    {
      debug_write("ACK not sent");
    }

    char buffer[64];
    memset (buffer, 0x00, 64);
    snprintf(buffer, 63, "SENDERID: [%u] DATA: [%s] LEN[%u]", radio.SENDERID, (char *) radio.DATA, radio.DATALEN);    
    debug_write(buffer);

    memset (buffer, 0x00, 64);
    snprintf(buffer, 63, "RX_RSSI: [%i]", radio.RSSI);  
    debug_write(buffer);
  }
  else
  {
    #if RF_COMM_LOG_LVL > 5
    debug_write("receive not Done");
    #endif
  }

  //print message received to serial
  // Serial.print('[');Serial.print(radio.SENDERID);Serial.print("] ");
  // Serial.print((char*)radio.DATA);
  // Serial.print("   [RX_RSSI:");Serial.print(radio.RSSI);Serial.print("]");
  // Serial.println();

  // if (radio.ACKRequested())
  // {
  //   radio.sendACK();
  //   Serial.print(" - ACK sent");
  // }
  return result;
}