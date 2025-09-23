#ifndef RF_COMM
#define RF_COMM

#define RECEIVE_ID      2
#define SEND_ID         1

#ifdef __cplusplus
extern "C" {
#endif

void rf_comm_config(uint16_t current_id);
void rf_comm_main(uint16_t current_id);

#ifdef __cplusplus
}
#endif

#endif