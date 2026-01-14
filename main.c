#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

int main()
{
    cJSON* test = cJSON_CreateObject();
    cJSON_AddStringToObject(test, "name", "Franek");
    char* testString = cJSON_Print(test);
    printf("%s", testString);

    return 0;
}
