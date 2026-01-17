#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "parseSrc.h"
#include "graph.h"
#include "solve.h"

int main()
{
    int rc = 0;
    // ****************
    // IMPORT JSON DATA
    // ****************
    
    // IMPORT THROUGH FILE
    // char* source = parseSrc("data/g0.json");

    // IMPORT THROUGH STDIN
    char* source = parseStdIn();

    // CREATE CJSON STRUCTURE
    cJSON* jsonObj = cJSON_Parse(source);
    if(!jsonObj) { rc = 1; goto clean_1; }

    // ************
    // PREPARE DATA
    // ************

    // GET VERTICES COUNT
    int v = cj_arrayLength(jsonObj, "Vertices");
    if(!v) { rc = 1; goto clean_1; }

    // PREPARE I:NAME MAP
    char** names = malloc(v * sizeof(char*));
    for(int i = 0; i < v; i++) names[i] = malloc(3 * sizeof(char));
    if(!cj_mapIdName(jsonObj, v, names)) { rc = 1; goto clean_2; }
    // for(int i = 0; i < v; i++) printf("V: %s ID: %d\n", names[i], i);

    // PREPARE ADJACENCY ARRAY
    // this can bo optimized for memory alocation, but not now :)
    int** adjArr = malloc(v * sizeof(int*));
    for(int i = 0; i < v; i++) {
        adjArr[i] = malloc(v * sizeof(int));
        memset(adjArr[i], 0, v * sizeof(int));
    }
    if(!cj_setAdjArr(jsonObj, v, adjArr, names)) { rc = 1; goto clean_3; }
    // for(int i = 0; i < v; i++) {
    //     printf("[");
    //     for(int j = 0; j < v; j++) printf("%d,", adjArr[i][j]);
    //     printf("]\n");
    // }

    // PREPARE INPUT VERTICES ARRAY
    int inputCount = cj_arrayLength(jsonObj, "Input");
    if(!inputCount) { rc = 1; goto clean_3; }
    int* input = malloc(inputCount * sizeof(int));
    if(!cj_getIds(jsonObj, "Input", input, v, names)) { rc = 1; goto clean_4; }

    // PREPARE OUTPUT VERTICES ARRAY
    int outputCount = cj_arrayLength(jsonObj, "Output");
    if(!outputCount) { rc = 1; goto clean_4; }
    int* output = malloc(outputCount * sizeof(int));
    if(!cj_getIds(jsonObj, "Output", output, v, names)) { rc = 1; goto clean_5; }

    // PREPARE IDENTITY ARRAY
    // -1 - input
    // 0 - internal
    // 1 - output
    int* identity = calloc(v, sizeof(int));
    for(int i = 0; i < inputCount; i++) identity[input[i]] = -1;
    for(int i = 0; i < outputCount; i++) identity[output[i]] = 1;


    // ***************
    // CALCULATE PATHS
    // ***************

    printPaths(
        v,
        names,
        adjArr,
        inputCount,
        input,
        outputCount,
        output,
        identity
    );

    // ***************
    // CLEAN UP MEMORY
    // ***************
    free(identity);
clean_5:
    free(output);
clean_4:
    free(input);
clean_3:
    for(int i = 0; i < v; i++) free(adjArr[i]);
    free(adjArr);
clean_2:
    for(int i = 0; i < v; i++) free(names[i]);
    free(names);
clean_1:
    cJSON_Delete(jsonObj);
    return 0;
}