#ifndef SCI_H
#define SCI_H

#define RX_PIN					TRISC7
#define TX_PIN					TRISC6

void init_uart(void);
void put_char(unsigned char byte);
int put_string(const char *s);

#endif
