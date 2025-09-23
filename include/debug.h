#ifndef DEBUG
#define DEBUG

#ifdef __cplusplus
extern "C" {
#endif

void debug_config(unsigned long baud);
void debug_write(char *msg);

#ifdef __cplusplus
}
#endif

#endif