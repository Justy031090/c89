#include <stdio.h>

/* 1. Check if a Number is a Power of Two */
int isPowerOfTwo(int n) {
    /* A number is a power of two if it has exactly one '1' bit in its binary representation. */
    /* Using the condition `n > 0` to avoid negative numbers and zero. */
    /* The expression (n & (n - 1)) clears the rightmost '1' bit. */
    /* If the result is zero, then n is a power of two. */
    return n > 0 && (n & (n - 1)) == 0;
}

/* 2. Count the Number of 1 Bits (Hamming Weight) */
int countOneBits(int n) {
    int count = 0; /* Initialize the count of '1' bits to zero */
    
    /* Loop until all bits in `n` are zero */
    while (n) {
        n &= (n - 1); /* Clears the rightmost set bit in `n` */
        count++;      /* Increment the count for each cleared bit */
    }
    return count; /* Return the total count of '1' bits */
}

/* 3. Find the Only Non-Repeating Element in an Array */
int findUnique(int arr[], int size) {
    int unique = 0; /* Initialize the unique element variable */
    
    /* XOR all elements in the array */
    /* Since XOR cancels out duplicate elements, only the unique element remains */
    for (int i = 0; i < size; i++) {
        unique ^= arr[i];
    }
    
    return unique; /* Return the single non-repeating element */
}

/* 4. Reverse the Bits of an Integer (32-bit) */
unsigned int reverseBits(unsigned int n) {
    unsigned int result = 0; /* Initialize the result to zero */
    int i;
    
    /* Iterate through all 32 bits of the integer */
    for (i = 0; i < 32; i++) {
        result <<= 1;   /* Shift result left to make space for the next bit */
        result |= (n & 1);  /* Set the least significant bit of `result` to the LSB of `n` */
        n >>= 1;        /* Shift `n` to the right to process the next bit */
    }
    
    return result; /* Return the reversed bit representation */
}

/* 5. Find the Missing Number in an Array from 0 to n */
int findMissingNumber(int arr[], int size) {
    int totalXOR = 0, arrXOR = 0;
    int i;

    /* Compute XOR of all numbers from 0 to `size` */
    for (i = 0; i <= size; i++) {
        totalXOR ^= i;
    }

    /* Compute XOR of all elements in the array */
    for (i = 0; i < size; i++) {
        arrXOR ^= arr[i];
    }

    /* The missing number is given by XORing `totalXOR` and `arrXOR` */
    return totalXOR ^ arrXOR;
}

/* 6. Determine the Sign of a Product of Three Numbers */
int productSign(int a, int b, int c) {
    int sign = 1; /* Initialize sign to positive (1) */
    
    /* If a number is negative, flip the sign */
    if (a < 0) sign = -sign;
    if (b < 0) sign = -sign;
    if (c < 0) sign = -sign;
    
    /* Return the sign: 1 for positive, -1 for negative */
    /* If any of the numbers is zero, the sign remains positive */
    return sign;
}

/* 7. Swap Two Numbers Without Using a Temporary Variable */
void swap(int *x, int *y) {
    /* Use XOR to swap values without a temporary variable */
    *x = *x ^ *y; /* Step 1: x becomes x ^ y */
    *y = *x ^ *y; /* Step 2: y becomes x (since x ^ y ^ y = x) */
    *x = *x ^ *y; /* Step 3: x becomes y (since x ^ y ^ x = y) */
}

/* 8. Find the Two Non-Repeating Elements in an Array */
void findTwoUniqueNumbers(int arr[], int size, int *x, int *y) {
    int xorSum = 0; /* XOR of all elements in the array */
    int setBit;     /* Variable to store the rightmost set bit */
    int i;
    
    /* XOR all elements to get XOR of the two unique elements */
    for (i = 0; i < size; i++) {
        xorSum ^= arr[i];
    }
    
    /* Find the rightmost set bit in xorSum */
    setBit = xorSum & ~(xorSum - 1);
    
    *x = 0; /* Initialize first unique element */
    *y = 0; /* Initialize second unique element */
    
    /* Divide elements into two groups based on the set bit */
    /* XOR elements in each group to get the unique elements */
    for (i = 0; i < size; i++) {
        if (arr[i] & setBit) {
            *x ^= arr[i]; /* XOR elements with the set bit */
        } else {
            *y ^= arr[i]; /* XOR elements without the set bit */
        }
    }
}



/* 1. Left Rotate Bits */
unsigned int leftRotate(unsigned int n, unsigned int d) {
    /* Rotate `n` to the left by `d` positions */
    return (n << d) | (n >> (32 - d));
}

/* 2. Right Rotate Bits */
unsigned int rightRotate(unsigned int n, unsigned int d) {
    /* Rotate `n` to the right by `d` positions */
    return (n >> d) | (n << (32 - d));
}

/* 3. Reverse (Mirror) Bits Without Using a Loop */
unsigned int reverseBitsNoLoop(unsigned int n) {
    /* Step-by-step approach to reverse bits without a loop */
    n = ((n >> 1) & 0x55555555) | ((n & 0x55555555) << 1);  /* Swap adjacent bits */
    n = ((n >> 2) & 0x33333333) | ((n & 0x33333333) << 2);  /* Swap pairs of bits */
    n = ((n >> 4) & 0x0F0F0F0F) | ((n & 0x0F0F0F0F) << 4);  /* Swap nibbles (4-bit groups) */
    n = ((n >> 8) & 0x00FF00FF) | ((n & 0x00FF00FF) << 8);  /* Swap bytes */
    n = (n >> 16) | (n << 16);                              /* Swap 16-bit groups */
    return n;  /* Return the mirrored (reversed) bits */
}

/* Swap the bits at position `idx1` and `idx2` in the integer `n` without using an `if` statement */
int SwapBits(int n, int idx1, int idx2) {
    /* Calculate XOR of the two bits */
    int bitMask = ((n >> idx1) ^ (n >> idx2)) & 1;

    /* Use XOR to flip both bits only if they differ (otherwise, XORing with 0 has no effect) */
    return n ^ ((bitMask << idx1) | (bitMask << idx2));
}



int main() {
    /* You can add test cases here to demonstrate the functions */
    return 0;
}
