#include <stdio.h>
#include <math.h>
#include "calculator.h"

void CheckResult(const char *expression, double expected, double result, status_t status) {
    if (status == SUCCESS) {
        if (fabs(result - expected) < 1e-6) {
            printf("Test Passed: %s = %f\n", expression, result);
        } else {
            printf("Test Failed: %s\n", expression);
            printf("Expected: %f, Got: %f\n", expected, result);
        }
    } else {
        printf("Test Failed: %s\n", expression);
        printf("Status: %d\n", status);
    }
}


void TestCalculator() {
    double result = 0;
    status_t status;

    status = Calculator("5+3", &result);
    CheckResult("5+3", 8.0, result, status);


    status = Calculator("10-2", &result);
    CheckResult("10-2", 8.0, result, status);


    status = Calculator("4*5", &result);
    CheckResult("4*5", 20.0, result, status);


    status = Calculator("20/4", &result);
    CheckResult("20/4", 5.0, result, status);


    status = Calculator("10/0", &result);
    if (status == DIV_ZERO) {
        printf("Test Passed: 10/0\n");
    } else {
        printf("Test Failed: 10/0\n");
        printf("Status: %d\n", status);
    }


    status = Calculator("2^3", &result);
    CheckResult("2^3", 8.0, result, status);


    status = Calculator("(2+3)*4", &result);
    CheckResult("(2+3)*4", 20.0, result, status);

    status = Calculator("3+5*2-(8/4)^2", &result);
    CheckResult("3+5*2-(8/4)^2", 7.0, result, status);

    status = Calculator("3+5a", &result);
    if (status == INVALID_INPUT) {
        printf("Test Passed: 3+5a\n");
    } else {
        printf("Test Failed: 3+5a\n");
        printf("Status: %d\n", status);
    }


    status = Calculator("3++5", &result);
    if (status == INVALID_INPUT) {
        printf("Test Passed: 3++5\n");
    } else {
        printf("Test Failed: 3++5\n");
        printf("Status: %d\n", status);
    }
}


int main() {
    TestCalculator();
    return 0;
}
