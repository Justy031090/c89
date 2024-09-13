
#include <stdio.h>  /*printf*/
#include <unistd.h> /*sleep*/

int main()
{
    printf("Test executable started...\n");
    fflush(stdout);

    sleep(5);

    printf("Test executable completed.\n");
    return 0;
}
