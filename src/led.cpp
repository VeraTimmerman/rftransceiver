#include <Arduino.h>
#include "led.h"

#define NBR_LEDS        1

// This part defines the LED pin

typedef enum led_status
{
    led_off,
    request_on,
    led_on,
    request_off
} led_status_t;

typedef struct led_settings
{
    led_status_t    led_status;
    unsigned int    pin;
    unsigned long   start_time_ms;
    unsigned long   on_time_ms;
} led_settings_t;

led_settings_t leds[NBR_LEDS];

void led_config()
{
    leds[USER_LED].pin = 9; // user led on D9
    leds[USER_LED].led_status = led_off;
    leds[USER_LED].on_time_ms = 100;

    for(unsigned int i = 0; i < NBR_LEDS; i++)
    {
        pinMode(leds[i].pin, OUTPUT);
    }
}

void led_main()
{
    for( unsigned int i = 0; i < NBR_LEDS; i++ )
    {
        switch(leds[i].led_status)
        {
            case led_off:
            break;

            case request_on:
            {
                digitalWrite(leds[i].pin, HIGH);
                leds[i].led_status = led_on;
                leds[i].start_time_ms = millis();
                break;
            }

            case led_on:
            {
                if(millis() - leds[i].start_time_ms >= leds[i].on_time_ms)
                {
                    leds[i].led_status = request_off;
                }
                break;
            }

            case request_off:
            {
                digitalWrite(leds[i].pin, LOW);
                leds[i].led_status = led_off;
                break;
            }
        }
    }
}

void set_led(unsigned int index, bool on)
{
    if(index >= NBR_LEDS)
    {
        return;        
    }

    if( on == true )
    {
        leds[index].led_status = request_on;
    }
    else if( on == false )
    {
        leds[index].led_status = request_off;
    }
}