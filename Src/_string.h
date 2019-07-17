#ifndef STRING_H_
#define STRING_H_

#include <stdlib.h>
#include "stdbool.h"

int indexOf(char *a,char findingchar);
void clearArray(char*a,int n);
void copy(char*a,char*b);
int length(char *a);
char * concat(char*a,char*b,char*c); //a первая строка, b - вторая, c - результат
char * substring(char *a,char *c,int start, int ends);
char * intToStr(int myint, char *a);
int lastIndexOf(char *a,char findchar);
int countOf(char *a,char chr);
char * trimChar(char*a,char*c,char chr);
bool startsWith (char *a,char *b );
void addChar(char *a, char*b, char c);
char * crypto_substring(char *a,char *c,int start, int ends);
char * crypto_concat(char*a,char*b,char*c, int n_start, int n_chars); //n - количество символов присоединения
int indexOfCount(char *a,char findchar, int k);
long int toInt(char *a);
int indexOfStr(char *a, char *b);
char * ftoa(double f, char * buf, int precision);

#endif /* STRING_H_ */
