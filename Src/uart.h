#ifndef UART_H
#define UART_H

#define UART1_BUFSIZE 256



void uart_ini(void);

void readATSerialBuff(void);
//void user_uart_tx(char * a);
//void uart_echo (void);
void printint(int i);
void printint_(int i);
void printstr(char *str);
void printstr_(char *str);
void printstrdma(char *str);
void printfloat(float f);
void printfloat_(float f);
void add_buffer(void);
//void change_buff(void);
//void clear_buff(void);
//char read_buff_char(void);
void parseBuff(char *a);

#endif
