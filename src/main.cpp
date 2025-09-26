#include <Arduino.h>
#include "led.h"
#include "rf_comm.h"
#include "debug.h"
#include <Wire.h>
#include <Adafruit_LPS2X.h>
#include <Adafruit_Sensor.h>

#define DEBUG_LOGLVL    5

#define CURRENT_ID      RECEIVE_ID
char hello_world[] = "Hello World!";

// For SPI mode, we need a CS pin
#define LPS_CS 10
// For software-SPI mode we need SCK/MOSI/MISO pins
#define LPS_SCK 13
#define LPS_MISO 12
#define LPS_MOSI 11

Adafruit_LPS25 lps25;

void setup() {
  debug_config(9600);
  // put your setup code here, to run once:
  #if DEBUG_LOGLVL > 3
  debug_write(hello_world);
  #endif  

  rf_comm_config(CURRENT_ID);
  led_config();

  if (!lps25.begin_I2C())
  {
    debug_write("Could not find lps25 chip");
  }
  else
  {
    debug_write("LPS25 Found!");
    lps25.setDataRate(LPS25_RATE_12_5_HZ);
    debug_write("Data rate set to: ");
    switch (lps25.getDataRate()) {
      case LPS25_RATE_ONE_SHOT: debug_write("One Shot"); break;
      case LPS25_RATE_1_HZ: debug_write("1 Hz"); break;
      case LPS25_RATE_7_HZ: debug_write("7 Hz"); break;
      case LPS25_RATE_12_5_HZ: debug_write("12.5 Hz"); break;
      case LPS25_RATE_25_HZ: debug_write("25 Hz"); break;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t temp;
  sensors_event_t pressure;
  char buffer[32];
  char output[15];

  while(true)
  {
    led_main();
    rf_comm_main(CURRENT_ID);

    // get pressure & temp
    if(lps25.getEvent(&pressure, &temp))
    {
      dtostrf(temp.temperature, 6, 2, output);
      memset(buffer, 0x00, 32);
      snprintf(buffer, 31, "Temp: %s degC", output);
      debug_write(buffer);

      dtostrf(pressure.pressure, 6, 2, output);
      memset(buffer, 0x00, 32);
      snprintf(buffer, 31, "Press: %s hPa", output);
      debug_write(buffer);
    }
  }
}

