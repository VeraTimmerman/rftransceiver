#include <Arduino.h>
#include "debug.h"

void debug_config(unsigned long baud)
{
    Serial.begin(baud);
}

void debug_write(char *msg)
{
  Serial.println(msg);
  Serial.flush();
}