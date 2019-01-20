#ifndef _RPMSG_H_
#define _RPMSG_H_

#include <stdint.h>

#define RPMSG_MSG_SZ 396

extern const int RPMSG_RECEIVED;
extern const int RPMSG_NOT_CONNECTED;
extern uint8_t rpmsg_receive_buf[RPMSG_MSG_SZ];

void rpmsg_setup(void);
void rpmsg_try_receive(void (*handler)(void));
uint8_t rpmsg_send(char *message);

#endif
