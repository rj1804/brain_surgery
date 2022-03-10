#ifndef _USBD_H_
#define _USBD_H_

int  usbd_init(void);
void usbd_deinit(void);
int  usbd_connected(void);
void usbd_connect(void);
void usbd_disconnect(void);
void usbd_eject(void);

#endif
