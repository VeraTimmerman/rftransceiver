#ifndef LED_H
#define LED_H

#define USER_LED        0

#ifdef __cplusplus
extern "C" {
#endif

void led_config();
void led_main();
void set_led(unsigned int index, bool on);

#ifdef __cplusplus
}
#endif

#endif