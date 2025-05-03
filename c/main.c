#include <stdio.h>

#include "application.h"

int main(void)
{
    printf("Hello, World!\n");
    if (!application_init())
        return 1;
    application_run();
    return 0;
}