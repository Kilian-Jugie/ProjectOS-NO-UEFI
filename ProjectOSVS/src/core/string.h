#ifndef __STRING_H_GUARD
#define __STRING_H_GUARD

#include "defines.h"

#define INT32_LENGTH_STR INT32_LENGTH+2
#define INT64_LENGTH_STR INT64_LENGTH+2

 namespace cstring {
	 int atoi(const char* ch) {
		 int ret = 0, i = 0;
		 bool neg = 0;
		 if (ch[0] == '-') neg = ++i;
		 while (ch[i]) {
			 ret += (ch[i] - '0');
			 ret *= 10;
			 ++i;
		 }
		 return (neg ? -ret : ret) / 10;
	 }

	 unsigned int atou(const char* ch) {
		 unsigned int ret = 0, i = 0;
		 while (ch[i]) {
			 ret += (ch[i] - '0');
			 ret *= 10;
			 ++i;
		 }
		 return ret / 10;
	 }

	 long long atoll(const char* ch) {
		 long long ret = 0, i = 0;
		 bool neg = 0;
		 if (ch[0] == '-') neg = ++i;
		 while (ch[i]) {
			 ret += (ch[i] - '0');
			 ret *= 10;
			 ++i;
		 }
		 return (neg ? -ret : ret) / 10;
	 }

	 unsigned long long atoull(const char* ch) {
		 unsigned long long ret = 0, i = 0;
		 while (ch[i]) {
			 ret += (ch[i] - '0');
			 ret *= 10;
			 ++i;
		 }
		 return ret / 10;
	 }

	 /*
		Only base 10 & 16 !
	*/
	 static void utoa(char* buf, int base, unsigned int d) {
		 char* p = buf;
		 char* p1, *p2;
		 unsigned long ud = d;
		 int divisor = 10;

		 if (base == 16)
			 divisor = 16;

		 do {
			 int remainder = ud % divisor;

			 *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		 } while (ud /= divisor);

		 *p = 0;

		 p1 = buf;
		 p2 = p - 1;
		 while (p1 < p2) {
			 char tmp = *p1;
			 *p1 = *p2;
			 *p2 = tmp;
			 p1++;
			 p2--;
		 }
	 }

	 /*
		Only base 10 & 16 !
	*/
	 static void ulltoa(char* buf, int base, unsigned long long d) {
		 char* p = buf;
		 char* p1, *p2;
		 unsigned long long ud = d;
		 int divisor = 10;

		 if (base == 16)
			 divisor = 16;

		 do {
			 long long remainder = ud % divisor;

			 *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		 } while (ud /= divisor);

		 *p = 0;

		 p1 = buf;
		 p2 = p - 1;
		 while (p1 < p2) {
			 char tmp = *p1;
			 *p1 = *p2;
			 *p2 = tmp;
			 p1++;
			 p2--;
		 }
	 }

	 /*
		Only base 10 & 16 !
	*/
	 static void itoa(char* buf, int base, int d) {
		 char* p = buf;
		 char* p1, *p2;
		 unsigned long ud = d;
		 int divisor = 10;
		 if (base == 10 && d < 0) {
			 *p++ = '-';
			 buf++;
			 ud = -d;
		 }
		 else if (base == 16)
			 divisor = 16;
		 do {
			 int remainder = ud % divisor;

			 *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		 } while (ud /= divisor);
		 *p = 0;
		 p1 = buf;
		 p2 = p - 1;
		 while (p1 < p2) {
			 char tmp = *p1;
			 *p1 = *p2;
			 *p2 = tmp;
			 p1++;
			 p2--;
		 }
	 }

	 /*
		Only base 10 & 16 !
	*/
	 static void lltoa(char* buf, int base, long long d) {
		 char* p = buf;
		 char* p1, *p2;
		 unsigned long long ud = d;
		 int divisor = 10;

		 if (base == 10 && d < 0) {
			 *p++ = '-';
			 buf++;
			 ud = -d;
		 }
		 else if (base == 16)
			 divisor = 16;

		 do {
			 long long remainder = ud % divisor;

			 *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		 } while (ud /= divisor);

		 *p = 0;

		 p1 = buf;
		 p2 = p - 1;
		 while (p1 < p2) {
			 char tmp = *p1;
			 *p1 = *p2;
			 *p2 = tmp;
			 p1++;
			 p2--;
		 }
	 }
}

#endif // !__STRING_T_GUARD
