#include <stdio.h>
#include "ws8.h"

#define ARRAY_SIZE (10)

int main()
{
	unsigned int numbers[ARRAY_SIZE] = {1,2,3,4,5,6,7,14,11,9};
	
	printf("*************TESTING Pow2 FUNCTION**************\n");
	printf("Pow(3, 2) - Expecting to get 3*(2^2) = 12;\nFunction output: %lu\n", Pow2(3, 2));
	printf("Pow(3, 0) - Expecting to get 3*(2^0) = 3;\nFunction output: %lu\n", Pow2(3, 2));
	printf("Pow(0, 2) - Expecting to get 0*(2^2) = 0;\nFunction output: %lu\n", Pow2(0, 2));
	printf("\n\n");
	printf("*************TESTING IsPow2Loop FUNCTION**************\n");
	printf("IsPow2Loop(2) - Expecting to get True;\nFunction output: %lu\n", IsPow2Loop(2));
	printf("IsPow2Loop(257) - Expecting to get False\nFunction output: %lu\n", IsPow2Loop(257));
	printf("IsPow2Loop(128) - Expecting to get True;\nFunction output: %lu\n", IsPow2Loop(128));
	printf("IsPow2Loop(0) - Expecting to get False;\nFunction output: %lu\n", IsPow2Loop(0));
	printf("\n\n");
	printf("*************TESTING IsPow FUNCTION**************\n");
	printf("IsPow2(2) - Expecting to get True;\nFunction output: %lu\n", IsPow2(2));
	printf("IsPow2(257) - Expecting to get False\nFunction output: %lu\n", IsPow2(257));
	printf("IsPow2(128) - Expecting to get True;\nFunction output: %lu\n", IsPow2(128));
	printf("IsPow2(0) - Expecting to get False;\nFunction output: %lu\n", IsPow2(0));
	printf("\n\n");
	printf("*************TESTING AddOne FUNCTION**************\n");
	printf("AddOne(2) - Expecting to get 3;\nFunction output: %lu\n", AddOne(2));
	printf("AddOne(257) - Expecting to get 258\nFunction output: %lu\n", AddOne(257));
	printf("AddOne(128) - Expecting to get 129;\nFunction output: %lu\n", AddOne(128));
	printf("AddOne(0) - Expecting to get 1;\nFunction output: %lu\n", AddOne(0));
	printf("\n\n");
	printf("*************TESTING IsThreeOn FUNCTION**************\n");
	printf("IsThreeOn(1,2,3,4,5,6,7,14,11,9) - Expecting to get 7, 14, 11\n");
	IsThreeOn(numbers);
	printf("\n\n");
	printf("*************TESTING ByteMirrorLoop FUNCTION**************\n");
	printf("ByteMirrorLoop(2) - Expecting to get 1073741824;\nFunction output: %u\n", ByteMirrorLoop(2));
	printf("ByteMirrorLoop(257) - Expecting to get 2155872256\nFunction output: %u\n", ByteMirrorLoop(257));
	printf("ByteMirrorLoop(128) - Expecting to get 16777216;\nFunction output: %u\n", ByteMirrorLoop(128));
	printf("ByteMirrorLoop(0) - Expecting to get 0;\nFunction output: %u\n", ByteMirrorLoop(0));
	printf("ByteMirrorLoop(1) - Expecting to get 2147483648;\nFunction output: %u\n", ByteMirrorLoop(1));
	printf("\n\n");
	printf("*************TESTING ByteMirror FUNCTION**************\n");
	printf("ByteMirror(2) - Expecting to get 1073741824;\nFunction output: %u\n", ByteMirror(2));
	printf("ByteMirror(257) - Expecting to get 2155872256\nFunction output: %u\n", ByteMirror(257));
	printf("ByteMirror(128) - Expecting to get 16777216;\nFunction output: %u\n", ByteMirror(128));
	printf("ByteMirror(0) - Expecting to get 0;\nFunction output: %u\n", ByteMirror(0));
	printf("ByteMirror(1) - Expecting to get 2147483648;\nFunction output: %u\n", ByteMirror(1));
	printf("\n\n");
	printf("*************TESTING SixAndTwoOn FUNCTION**************\n");
	printf("SixAndTwoOn(35) - Expecting to get True;\nFunction output: %d\n", SixAndTwoOn(35));
	printf("SixAndTwoOn(38) - Expecting to get True\nFunction output: %d\n", SixAndTwoOn(38));
	printf("SixAndTwoOn(34) - Expecting to get True;\nFunction output: %d\n", SixAndTwoOn(34));
	printf("SixAndTwoOn(33) - Expecting to get False;\nFunction output: %d\n", SixAndTwoOn(33));
	printf("SixAndTwoOn(0) - Expecting to get False;\nFunction output: %d\n", SixAndTwoOn(0));
	printf("\n\n");
	printf("*************TESTING SixOrTwoOn FUNCTION**************\n");
	printf("SixOrTwoOn(35) - Expecting to get True;\nFunction output: %d\n", SixOrTwoOn(35));
	printf("SixOrTwoOn(38) - Expecting to get True\nFunction output: %d\n", SixOrTwoOn(38));
	printf("SixOrTwoOn(34) - Expecting to get True;\nFunction output: %d\n", SixOrTwoOn(34));
	printf("SixOrTwoOn(33) - Expecting to get True;\nFunction output: %d\n", SixOrTwoOn(33));
	printf("SixOrTwoOn(2) - Expecting to get True;\nFunction output: %d\n", SixOrTwoOn(2));
	printf("SixOrTwoOn(32) - Expecting to get True;\nFunction output: %d\n", SixOrTwoOn(32));
	printf("SixOrTwoOn(0) - Expecting to get False;\nFunction output: %d\n", SixOrTwoOn(0));
	printf("SixOrTwoOn(5) - Expecting to get False;\nFunction output: %d\n", SixOrTwoOn(5));
	printf("\n\n");
	printf("*************TESTING SwapThreeAndFive FUNCTION**************\n");
	printf("SwapThreeAndFive(16) - Expecting to get 4;\nFunction output: %d\n", SwapThreeAndFive(16));
	printf("SwapThreeAndFive(4) - Expecting to get 16\nFunction output: %d\n", SwapThreeAndFive(4));
	printf("SwapThreeAndFive(5) - Expecting to get 17;\nFunction output: %d\n", SwapThreeAndFive(5));
	printf("SwapThreeAndFive(18) - Expecting to get 17;\nFunction output: %d\n", SwapThreeAndFive(6));
	printf("\n\n");
	printf("*************TESTING DivisionOfSixteen FUNCTION**************\n");
	printf("DivisionOfSixteen(33) - Expecting to get 32;\nFunction output: %d\n", DivisionOfSixteen(33));
	printf("DivisionOfSixteen(17) - Expecting to get 16;\nFunction output: %d\n", DivisionOfSixteen(17));
	printf("DivisionOfSixteen(22) - Expecting to get 16;\nFunction output: %d\n", DivisionOfSixteen(22));
	printf("\n\n");
	printf("*************TESTING NumberOfSetBitsLoop FUNCTION**************\n");
	printf("NumberOfSetBitsLoop(33) - Expecting to get 2;\nFunction output: %u\n", NumberOfSetBitsLoop(33));
	printf("NumberOfSetBitsLoop(17) - Expecting to get 2;\nFunction output: %u\n", NumberOfSetBitsLoop(17));
	printf("NumberOfSetBitsLoop(3) - Expecting to get 2;\nFunction output: %u\n", NumberOfSetBitsLoop(3));
	printf("NumberOfSetBitsLoop(1) - Expecting to get 1;\nFunction output: %u\n", NumberOfSetBitsLoop(1));
	printf("NumberOfSetBitsLoop(2) - Expecting to get 1;\nFunction output: %u\n", NumberOfSetBitsLoop(2));
	printf("NumberOfSetBitsLoop(7) - Expecting to get 3;\nFunction output: %u\n", NumberOfSetBitsLoop(7));
	printf("\n\n");
	printf("*************TESTING NumberOfSetBits FUNCTION**************\n");
	printf("NumberOfSetBits(33) - Expecting to get 2;\nFunction output: %u\n", NumberOfSetBits(33));
	printf("NumberOfSetBits(17) - Expecting to get 2;\nFunction output: %u\n", NumberOfSetBits(17));
	printf("NumberOfSetBits(3) - Expecting to get 2;\nFunction output: %u\n", NumberOfSetBits(3));
	printf("NumberOfSetBits(1) - Expecting to get 1;\nFunction output: %u\n", NumberOfSetBits(1));
	printf("NumberOfSetBits(2) - Expecting to get 1;\nFunction output: %u\n", NumberOfSetBits(2));
	printf("NumberOfSetBitsLoop(7) - Expecting to get 3;\nFunction output: %u\n", NumberOfSetBitsLoop(7));
	printf("\n\n");
	PrintFLoat(5.2);
	
	return 0;
}
