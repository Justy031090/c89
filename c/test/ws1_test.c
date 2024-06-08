#include "ws1.h"

int main()
{
	PrintHello();
	printf("%.2f\n", PowerOfTen(4));     /* Testing for Positive Number */
	printf("%f\n", PowerOfTen(-3));      /* Testing for Negative number */
	printf("%.2f\n", PowerOfTen(0));     /* Testing for Zero            */
	printf("%d\n", FlipOrder(253));      /* Testing for Positive number */
	printf("%d\n", FlipOrder(-542));     /* Testing for Negative number */
	return 0;
}
