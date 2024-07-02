#ifndef __WS13_H__
#define __WS13_H__


#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define ENDIAN (1)
#else
#define ENDIAN (0)
#endif

char *IntToString(int x, char *str, int base);
int StringToInt(char *str, int base);
void ThreeArr (char *arr1, char *arr2, char *arr3);
char *IntToString10(int x, char *str);
int StringToInt10(char *str);



#endif /*__WS13_h__*/
