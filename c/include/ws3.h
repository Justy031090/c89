#ifndef __WS3_H__
#define __WS3_H__
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

size_t StrLen(const char *str);

int StrCmp(const char* s1, const char* s2);

char *StrCpy(char *dst, const char *src);

char *StrnCpy(char *dst, const char *src, size_t dsize);

int StrnCmp(const char *str1, const char *str2, size_t n);

int StrCaseCmp(const char *s1, const char *s2);

char *StrChr(const char *s, int c);

char *StrDup(const char *s);

char *StrCat(char *dst, const char *src);

char *StrnCat(char *dst, const char *src, size_t ssize);

char *StrStr(const char *haystack, const char *needle);

size_t StrSpn(const char *s, const char *accept);

int IsPolindrome(const char *s);

char *RevieWhiteSpaces(char *s);

#endif /* __WS2_3__ */
