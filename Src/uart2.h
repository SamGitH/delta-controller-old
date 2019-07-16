#ifndef UART2_H
#define UART2_H

#define UART1_BUFSIZE 256





void uart_ini_u2(void);

void readATSerialBuff_u2(void);
//void user_uart_tx(char * a);
//void uart_echo (void);
void printint_u2(int i);
void printstr_u2(char *str);
void printfloat_u2(float f);
void add_buffer_u2(void);
//void change_buff(void);
//void clear_buff(void);
//char read_buff_char(void);
void parseBuff_u2(char *a);

#endif
