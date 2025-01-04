#include <Arduino.h>
#include <RFM69.h>
#include <RFM69registers.h>
#include <RFM69_ATC.h>
#include <RFM69_OTA.h>

//This part defines the LED pin and button pin
#define USER_LED        9 //LED on D9

#define FREQUENCY RF69_868MHZ
#define SEND_ID  2
#define NETWORKID 100
#define ENCRYPTKEY "sampleEncryptKey"
#define RECEIVE_ID  1

#define DEBUG_LOGLVL    5

RFM69 radio;
int current_state = LOW;
char hello_world[] = "Hello World!";
bool init_successfull = false;

// put function declarations here:
void write(char *msg);
void led();
void send(uint16_t toAddress, char *msg, uint8_t size);
void receive();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  #if DEBUG_LOGLVL > 3
  write(hello_world);
  #endif
  pinMode(USER_LED, OUTPUT);
  led();

  init_successfull = radio.initialize(FREQUENCY, SEND_ID, NETWORKID);
  if(init_successfull)
  {
    #if DEBUG_LOGLVL > 1
    char buff[50];
    memset(buff, 0x00, 50);
    snprintf(buff, 49, "\nListening at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
    write(buff);
    write("initializing radio successfull");
    #endif
    radio.encrypt(ENCRYPTKEY);
  }
  else
  {
    write("initializing radio un successfull");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  while(true)
  {

    if(init_successfull)
    {
      if(radio.receiveDone())
      {
        receive();
      }
      else
      {
        send(RECEIVE_ID, hello_world, strlen(hello_world));
      }
    }
    else
    {
      #if DEBUG_LOGLVL > 3
      write("initialization not successfull!");
      #endif
      led();
    }

    delay(1000);
  }
}

void write(char *msg)
{
  Serial.println(msg);
  Serial.flush();
}

void led()
{
  digitalWrite(USER_LED, current_state);

  #if DEBUG_LOGLVL > 3
  char buffer[32];
  memset (buffer, 0x00, 32);
  snprintf(buffer, 32, "current_state %u", current_state);
  write(buffer);
  #endif

  if(current_state == LOW)
  {
    current_state = HIGH;
  }
  else
  {
    current_state = LOW;
  }
}

void send(uint16_t toAddress, char *msg, uint8_t len)
{
  #if DEBUG_LOGLVL > 5
  char buff[64];
  memset(buff, 0x00, 64);
  snprintf(buff, 63, "TO: [%u] MSG: [%s] LEN: [%u]", toAddress, msg, len);
  write(buff);
  #elif DEBUG_LOGLVL > 3
  write("send with retry");
  #endif

  bool result = 
  radio.send(toAddress, "All About Circuits", 18, false); // radio.sendWithRetry(toAddress, "All About Circuits", 18);
  if(result)
  {
    #if DEBUG_LOGLVL > 3
    write("sent was true");
    #endif
    led();
    delay(100);
    led();
  }
  else
  {
    #if DEBUG_LOGLVL > 2
    write("failed to send");
    #endif
  }
}

void receive()
{
  // bool result = radio.receiveDone();
  // if(result)
  // {
  //   if(radio.ACKRequested())
  //   {
  //     radio.sendACK();
  //     #if DEBUG_LOGLVL > 2
  //     write("ACK sent");
  //     #endif
  //   }

  //   char buffer[32];
  //   memset (buffer, 0x00, 32);
  //   snprintf(buffer, 31, "SENDERID: [%u] DATA: [%s] LEN[%u]", radio.SENDERID, (char *) radio.DATA, radio.DATALEN);    
  //   write(buffer);

  //   memset (buffer, 0x00, 32);
  //   snprintf(buffer, 31, "RX_RSSI: [%i]", radio.RSSI);  
  // }

  //print message received to serial
  Serial.print('[');Serial.print(radio.SENDERID);Serial.print("] ");
  Serial.print((char*)radio.DATA);
  Serial.print("   [RX_RSSI:");Serial.print(radio.RSSI);Serial.print("]");
  Serial.println();

  if (radio.ACKRequested())
  {
    radio.sendACK();
    Serial.print(" - ACK sent");
  }
}