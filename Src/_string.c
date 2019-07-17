//Вспомогательная библиотека для работы со строками

#include "_string.h"

#define MAX_PRECISION	(10)

static const double rounders[MAX_PRECISION + 1] =
{
		0.5,				// 0
		0.05,				// 1
		0.005,				// 2
		0.0005,				// 3
		0.00005,			// 4
		0.000005,			// 5
		0.0000005,			// 6
		0.00000005,			// 7
		0.000000005,		// 8
		0.0000000005,		// 9
		0.00000000005		// 10
};

char * crypto_concat(char*a,char*b,char*c, int n_start, int n_chars)//n - количество символов присоединения
{
	int n_ends = n_start + n_chars;
	char lstr[n_ends];
	char *alstr=lstr;

	for(int i=0;i<n_ends;i++)  //чистка созданного буфера
	{
		*alstr=0x00;
		alstr++;
	}
	alstr=lstr;
	//--------------------------
	int n=0;

	while(n<n_start)
	{
		*alstr=*a;
		a++;
		alstr++;
		n++;
	}

	while(n<n_ends)
	{
		*alstr=*b;
		b++;
		alstr++;
		n++;
	}

	alstr=lstr;
	char *ac=c;

	for (int i=0; i<n_ends; i++)  //заполняем строкой
	{
		*c=*alstr;
		c++;
		alstr++;
	}
	*c=*alstr;
	return ac;
}

char * crypto_substring(char *a,char *c,int start, int ends)
{
	if (start==ends)
		return 0x00;
	//вспомогательный адрес для выяснения n
	int n=0;

	char lstr[ends+1];
	char *k=lstr;
	for(int i=0;i<ends+1;i++)
	{
		*k=0x00;
		k++;
	}
	k=lstr;


	while(n!=ends)
	{
		if(n>=start)
		{
			*k=*a;
			k++;
		}
		a++;
		n++;
	}

	char *d=c;
	k=lstr;
	n=0;
	while(n<ends)  //заполняем строкой
			{
		n++;
		*d=*k;
		d++;
		k++;
			}
	*d=*k;

	return c;

}

void mooveChar(char *a, char*b, int c, int d)
{
	char *aa=a;
	int n=0;
	char chr=0;
	do
	{
		if(n==c){chr=*aa;}
		aa++;
		n++;
	} while(*aa!=0x00);

	char str[n+1];
	char* astr=str;
	n=0;
	aa=a;

	do
	{
		if(n==d){*astr=chr;astr++; }
		if(n==c){aa++; }
		*astr=*aa;
		astr++;
		aa++;
		n++;
	} while(*aa!=0x00);

	*astr=0x00;
	astr=str;

	while(*astr!=0x00)
	{
		*b=*astr;
		b++;
		astr++;
	}
	*b=0x00;
}

char* addCharTo(char *a, char*b, char c, int nc)
{
	char*aa=a;
	int k=0;
	do{
		k++;
		aa++;
	} while(*aa!=0);

	char tempChar[k+2];
	char *ta=tempChar;
	int n=0;

	while(*a!=0x00)
	{
		if(nc==n){*ta=c;ta++;}
		*ta=*a;
		ta++;
		a++;
		n++;
	}

	*ta=0x00;
	ta=tempChar;

	do
	{
		*b=*ta;
		b++;
		ta++;
	} while (*ta!=0);

	return b;
}

void replaseChar(char *a, char*b, int c, int d)
{
	if(d<c){int t=c; c=d; d=t;}
	int n=0;
	char chra=0,chrb=0;
	char *aa=a;
	while(*aa!=0x00)
	{
		if (n==c){chra=*aa;}
		if (n==d){chrb=*aa;break;}
		aa++;
		n++;
	}

	n=0;

	while(*a!=0x00)
	{
		if (n==c){*b=chrb;b++; a++;n++;}
		if (n==d){*b=chra;b++; a++;n++;}
		*b=*a;
		a++;
		b++;
		n++;
	}

	*b=0x00;
}

void changeChar(char *a, char*b, char c, char d)
{
	while(*a!=0x00)
	{

		*b=*a;
		if (*a==c)*b=d;
		a++;
		b++;
	}
	*b=0x00;
}

void addChar(char *a, char*b, char c)
{
	while(*a!=0x00)
	{
		*b=*a;
		a++;
		b++;
	}
	*b=c;
	b++;
	*b=0x00;
}

int length(char *a)
{
	int n=0;

	while(*a!=0x00)
	{
		a++;
		n++;
	}
	return n;
}

int countOf(char *a,char chr)
{
	int n=0;

	while (*a!=0)
	{
		if (*a==chr) n++;
		a++;
	}
	return n;
}

char charAt(char *a,int n)
{
	for(int i=0; i<n; i++) a++;
	return *a;
}

void copy(char*a,char*b)
{
	while (*a!=0)
	{
		*b=*a;
		a++;
		b++;
	}
	*b=0x00;
}

void clearArray(char*a,int n)
{
	for (int i=0; i<n; i++)
	{
		*a=0x00;
		a++;
	}
}

void clearArray_int(int*a,int n)
{
	for (int i=0; i<n; i++)
	{
		*a=0x00;
		a++;
	}
}

void clearStr(char*a)
{
	while (*a!=0)
	{
		*a=0x00;
		a++;
	}
}

int indexOf(char *a,char findingchar)
{
	int n=0;

	while(*a!=0x00)
	{
		if(*a==findingchar)return n;
		a++;
		n++;
	}

	return -1;
}

int indexOfStr(char *a, char *b)
{
	char * f_a;
	char * f_b;
	int curr_n=0, eq_n=0;
	bool flag = false;
	f_a = a;
	f_b = b;

	while(*f_a!=0x00)
	{
		curr_n++;

		if (*f_a == *f_b) //если текущий символ строки и первый символ искомой строки соответсвуют
				{
			flag= true;
			if (eq_n==0)eq_n=curr_n; //запоминаем номер соответствия первго символа
			f_b++;
				}
		else if (flag == true) //иначе если символ уже соответсвовал
		{
			f_b = b; //сбрасываем начальный адрес искомой строки
			f_a = a;	//сбрасываем начальный адрес основной строки
			f_a+=eq_n-1;//задаем значение последнего найденного символа
			curr_n--;
			eq_n=0;
			flag = false;
		}
		if (flag == true && *f_b==0x00) return eq_n-1;
		f_a++;
	}

	return -1;
}

int lastIndexOf(char *a,char findchar)
{
	int n=0;
	int nchar=0;

	while(*a!=0x00)
	{
		if(findchar==*a)
			nchar=n;
		a++;
		n++;
	}
	return nchar;
}

int indexOfAfter(char *a,char findchar, int k)
{
	int n=0;

	while(*a!=0x00)
	{
		if((findchar==*a) & (n>k)) return n;
		a++;
		n++;
	}
	return 0;
}

int indexOfCount(char *a,char findchar, int k)
{
	int n=0,m=0;

	while(*a!=0x00)
	{
		if(findchar==*a)
		{
			m++;
			if (m==k) return n;
		}
		a++;
		n++;
	}
	return 0;
}

bool startsWith (char *a,char *b )
{
	if (length(a) < length(b)) return 0;
	while(*b!=0x00)
	{
		if(*a!=*b)
			return 0;
		a++;
		b++;
	}
	return 1;
}

bool startsWithEnyLength (char *a,char *b )
{
	if (length(a)<1)return 0;
	while(*b!=0x00 && *a!=0x00)
	{
		if(*a!=*b)
			return 0;
		a++;
		b++;
	}
	return 1;
}

bool equality (char *a,char *b ) //точное сравнение
{
	char *aa=a;
	char *bb=b;

	while(*aa!=0x00)
	{
		if(*aa!=*bb)
			return 0;
		aa++;
		bb++;
	}

	while(*bb!=0x00)
	{
		if(*aa!=*bb)
			return 0;
		aa++;
		bb++;
	}
	return 1;
}

char * concat(char*a,char*b,char*c) //a первая строка, b - вторая, c - результат
{
	int n = length(a)+length(b);
	n++;
	char lstr[n+1];
	char *alstr=lstr;

	for(int i=0;i<n;i++)
	{
		*alstr=0x00;
		alstr++;
	}
	alstr=lstr;
	//--------------------------
	n=0;
	while(*a!=0x00)
	{
		*alstr=*a;
		a++;
		alstr++;
		n++;
	}

	while(*b!=0x00)
	{
		*alstr=*b;
		b++;
		alstr++;
		n++;
	}

	n++;
	alstr++;
	*alstr=0x00;
	alstr=lstr;
	char *ac=c;

	for (int i=0; i<n; i++)  //заполняем строкой
	{
		*c=*alstr;
		c++;
		alstr++;
	}
	*c=*alstr;
	return ac;
}

char * concat_count(char*a,char*b,char*c,int k)
{
	int n = length(a)+length(b);
	n++;

	if (n>k) return a;

	char lstr[n];
	char *alstr=lstr;

	for(int i=0;i<n;i++)
	{
		*alstr=0x00;
		alstr++;
	}

	alstr=lstr;
	//--------------------------
	n=0;

	while(*a!=0x00)
	{
		*alstr=*a;
		a++;
		alstr++;
		n++;
	}

	while(*b!=0x00)
	{
		*alstr=*b;
		b++;
		alstr++;
		n++;
	}

	alstr=lstr;
	char *ac=c;

	for (int i=0; i<n; i++)  //заполняем строкой
	{
		*c=*alstr;
		c++;
		alstr++;
	}
	*c=*alstr;
	return ac;
}

void toLowerCase(char *a, char *c)//toLowerCase(вход.массив,возвращенный массив); Меняет верхний регистр на нижний.
{
	char *b=a;  //вспомогательный адрес для выяснения n
	int n=0;

	while(*b!=0x00)
	{
		n++;
		b++;
	}

	n++;
	char lstr[n];
	char *k=lstr;

	for(int i=0;i<n;i++)
	{
		*k=0x00;
		k++;
	}

	k=lstr;

	while(*a!=0x00) //считываем обрабатываемую строку
	{
		if ((*a>64)&(*a<91)) //обрабатываем
		{
			*k=*a+32;

		}
		else
		{
			*k=*a; //заносим в массив
		}
		k++;
		a++;
	}

	k=lstr;

	while(*c!=0x00)  //заполняем строкой
	{
		*c=*k;
		c++;
		k++;
	}
	*c=*k;
}

char* toUpperCase(char *a, char *c)
{
	char *b=a;  //вспомогательный адрес для выяснения n
	int n=0;

	while(*b!=0x00)
	{
		n++;
		b++;
	}

	n++;
	char lstr[n];
	char *k=lstr;

	for(int i=0;i<n;i++)
	{
		*k=0x00;
		k++;
	}

	k=lstr;

	while(*a!=0x00) //считываем обрабатываемую строку
	{
		if ((*a>96)&(*a<123)) //обрабатываем
		{

			*k=*a-32;

		}
		else
		{
			*k=*a; //заносим в массив
		}
		k++;
		a++;
	}

	k=lstr;
	char*d=c;

	while(*d!=0x00)  //заполняем строкой
	{
		*d=*k;
		d++;
		k++;
	}

	*d=*k;

	return c;  //возвращаем.
}

char * substring(char *a,char *c,int start, int ends)
{
	if (start==ends){return 0x00;}

	char *b=a;  //вспомогательный адрес для выяснения n
	int n=0;

	while(*b!=0x00)
	{
		n++;
		b++;
	}
	if (n<start) return a;

	n++;
	char lstr[n];
	char *k=lstr;

	for(int i=0;i<n;i++)
	{
		*k=0x00;
		k++;
	}

	k=lstr;
	n=0;

	while(*a!=0x00)
	{
		if (n>=ends) { k++;break;}
		if(n>=start)
		{
			*k=*a;
			k++;
		}
		a++;
		n++;
	}

	*k=0x00;
	char *d=c;
	k=lstr;

	while(*k!=0x00)  //заполняем строкой
	{
		*d=*k;
		d++;
		k++;
	}

	*d=*k;

	return c;
}

char * trimChar(char*a,char*c,char chr)
{
	char *b=a;  //вспомогательный адрес для выяснения n
	int n=0;

	while(*b!=0x00)
	{
		n++;
		b++;
	}

	n++;
	char lstr[n];
	char *alstr=lstr;

	for(int i=0;i<n;i++)
	{
		*alstr=0x00;
		alstr++;
	}

	alstr=lstr;

	while(*a!=0x00)
	{
		if(*a==chr){a++; continue;}
		*alstr=*a;
		a++;
		alstr++;
	}

	char *d=c;
	alstr=lstr;

	while(*d!=0x00)  //заполняем строкой
	{
		*d=*alstr;
		d++;
		alstr++;
	}

	*d=*alstr;

	return c;
}

char * trim(char *a,char*c, char chr)
{
	char *b=a;  //вспомогательный адрес для выяснения n
	int n=0;

	while(*b!=0x00)
	{
		n++;
		b++;
	}

	n++;
	char lstr[n];
	char *alstr=lstr;

	for(int i=0;i<n;i++)
	{
		*alstr=0x00;
		alstr++;
	}

	alstr=lstr;

	while(*a!=0x00)
	{
		a++;
		n++;
		alstr++;
	}

	bool flag_notEmty=0;

	while(n!=0)
	{
		a--;
		alstr--;
		n--;

		if(*a!=chr && !flag_notEmty){*alstr=*a;flag_notEmty=1;}
		if (flag_notEmty)*alstr=*a;
	}

	char *d=c;
	alstr=lstr;

	while(*d!=0x00)  //заполняем строкой
	{
		*d=*alstr;
		d++;
		alstr++;
	}

	*d=*alstr;

	return c;
}

char * intToStr(int myint, char *a)
{
	char lstr[12];
	char *astr=lstr;

	for (int i=0; i<12; i++)
	{
		*astr=0x00;
		astr++;
	}

	astr=lstr;

	if(myint<0)
	{
		*astr='-';
		astr++;
		myint*=-1;

	}

	long int delitel=1000000000;
	bool flag_zero=1;

	for (int i=0; i<10; i++)
	{
		int k;
		k=myint/delitel;
		myint-=(k*delitel);
		delitel/=10;
		if(i==6&&k==0)*astr=48;
		if(k==0&&flag_zero)continue;
		if(k!=0)flag_zero=0;
		*astr=k+48;
		astr++;

	}

	astr=lstr;
	char *b=a;

	for (int i=0; i<10; i++)  //заполняем строкой
	{
		*b=*astr;
		b++;
		astr++;
	}

	*b=*astr;

	return a;
}

float toFloat(char *a)
{
	int n=0;
	long int dec=1;
	long int integer=0;
	bool usign=0;
	float flt=0;
	long int dot=1,ndot=1;

	while(*a!=0x00)
	{
		a++;
		n++;
	}

	while(n!=0)
	{
		n--;
		a--;
		if (*a=='.'||*a==','){dot=ndot;continue;}
		if (*a==0x2D){usign=1;continue;}
		if ((*a>47) & (*a<58))
		{
			integer+=(*a-48)*dec;
			dec*=10;
			ndot*=10;
		}
	}

	if(usign)integer=-1*integer;

	flt=integer;
	flt/=dot;

	return flt;
}

long int toInt(char *a)
{
	int n=0;
	long int dec=1;
	long int integer=0;
	bool usign=0;

	while(*a!=0x00)
	{
		a++;
		n++;
	}

	if(n<1)return 0;

	while(n!=0)
	{
		n--;
		a--;

		if (*a==0x2D){usign=1;continue;}

		if ((*a>47) & (*a<58))
		{
			integer+=(*a-48)*dec;
			dec=dec*10;

		}

	}

	if(usign)integer=-1*integer;

	return integer;
}

long int toIntLast(char *a)
{
	int n=0;
	long int dec=1;
	long int integer=0;
	bool flag_gettingInteger=0;
	bool usign=0;

	while(*a!=0x00)
	{
		a++;
		n++;
	}

	if(n<1) return 0;

	while(n!=0)
	{
		n--;
		a--;

		if (*a==0x2D)
		{
			usign=1;
			continue;
		}

		if ((*a>47) & (*a<58))
		{
			integer+=(*a-48)*dec;
			dec=dec*10;
			flag_gettingInteger=1;
			continue;
		}

		if (flag_gettingInteger==1)
		{
			if(usign)integer=-1*integer;
			return integer;
		}
	}

	if(usign) integer=-1*integer;

	return integer;
}

long int toIntFirst(char *a)
{
	int n=0;
	long int dec=1;
	long int integer=0;
	bool usign=0,flag_emty=0;

	while(*a!=0x00)
	{
		a++;
		n++;
	}

	if(n<1)return 0;

	while(n!=0)
	{
		n--;
		a--;

		if(*a==45) {usign=1; continue;}

		if ((*a>47) & (*a<58))
		{
			if (flag_emty)
			{
				integer=0;
				dec=1;
				flag_emty=0;
			}
			integer+=(*a-48)*dec;
			dec=dec*10;
			continue;
		}

		if ((*a<47) || (*a>58)) flag_emty=1;

	}

	if(usign)integer=-1*integer;

	return integer;
}

char * ftoa(double f, char * buf, int precision)
{
	char * ptr = buf;
	char * p = ptr;
	char * p1;
	char c;
	long intPart;

	if (precision > MAX_PRECISION)
		precision = MAX_PRECISION;

	// sign stuff
	if (f < 0)
	{
		f = -f;
		*ptr++ = '-';
	}

	if (precision < 0)  // negative precision == automatic precision guess
	{
		if (f < 1.0) precision = 6;
		else if (f < 10.0) precision = 5;
		else if (f < 100.0) precision = 4;
		else if (f < 1000.0) precision = 3;
		else if (f < 10000.0) precision = 2;
		else if (f < 100000.0) precision = 1;
		else precision = 0;
	}

	// round value according the precision
	if (precision)
		f += rounders[precision];

	// integer part...
	intPart = f;
	f -= intPart;

	if (!intPart)
		*ptr++ = '0';
	else
	{
		// save start pointer
		p = ptr;

		// convert (reverse order)
		while (intPart)
		{
			*p++ = '0' + intPart % 10;
			intPart /= 10;
		}

		// save end pos
		p1 = p;

		// reverse result
		while (p > ptr)
		{
			c = *--p;
			*p = *ptr;
			*ptr++ = c;
		}

		// restore end pos
		ptr = p1;
	}

	// decimal part
	if (precision)
	{
		// place decimal point
		*ptr++ = '.';

		// convert
		while (precision--)
		{
			f *= 10.0;
			c = f;
			*ptr++ = '0' + c;
			f -= c;
		}
	}

	// terminating zero
	*ptr = 0;

	return buf;
}
