#include "Application.h"

int main()
{
    Application application;
    if (!application.init())
        return 1;
    return application.run();
}
