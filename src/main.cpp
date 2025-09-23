#include <Arduino.h>
#include "led.h"
#include "rf_comm.h"
#include "debug.h"


#define DEBUG_LOGLVL    5

#define CURRENT_ID      RECEIVE_ID
char hello_world[] = "Hello World!";

void setup() {
  debug_config(9600);
  // put your setup code here, to run once:
  #if DEBUG_LOGLVL > 3
  debug_write(hello_world);
  #endif  

  rf_comm_config(CURRENT_ID);
  led_config();
}

void loop() {
  // put your main code here, to run repeatedly:

  while(true)
  {
    led_main();
    rf_comm_main(CURRENT_ID);
  }
}

