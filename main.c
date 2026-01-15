#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "parseSrc.h"
#include "graph.h"

#define exitWithError() \
    do { \
        clearMap(); \
        cJSON_Delete(jsonObj); \
        return 1; \
    } while(0)

int main()
{
    // IMPORT JSON
    // PREPARE ALTERNATIVE stdin() source !
    char* source = parseSrc("data/g1.json");
    cJSON* jsonObj = cJSON_Parse(source);
    if(!jsonObj) {
        printf("Failed to parese source string!\n");
        return 1;
    }

    // PREPARE DATA

    // GET VERTICES COUNT
    int v = cj_countVertices(jsonObj);
    if(!v) exitWithError();

    // PREPARE I:NAME MAP
    char names[v][3];
    if(!cj_mapIdName(jsonObj)) exitWithError();

    // PREPARE ADJACENCY ARRAY
    int adjArr[v][v];
    if(!cj_setAdjArr(jsonObj, v, adjArr)) exitWithError();

    // CALCULATE

    // PRINT ADJ ARRAY
    for(int z = 0; z < v; z++) {
        printf("[");
        for(int x = 0; x < v; x++) {
            printf("%d,", adjArr[z][x]);
        }
        printf("]\n");
    }
    
    clearMap();
    cJSON_Delete(jsonObj);
    return 0;
}