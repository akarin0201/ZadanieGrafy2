#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "parseSrc.h"
#include "graph.h"
#include "solve.h"

int main(int argc, char* argv[])
{
    // *************
    // DECLARE FLAGS
    // *************
    int debug = 0;
    int inputFile = 0;
    char* filename = NULL;
    int rc = 0;

    // ***************
    // SERVE ARGUMENTS
    // ***************
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-h") == 0) {
            printf("Standard input: provide input in json structure after launch, press enter to get the result\nInput from file: -f path/to/filename.json\nHelp: -h\nDebug: -d\n");
            return 0;
        }
        else if(strcmp(argv[i], "-d") == 0) {
            debug = 1;
        }
        else if(strcmp(argv[i], "-f") == 0) {
            if(i + 1 < argc) {
                inputFile = 1;
                filename = argv[i+1];
                i++;
            }
            else {
                fprintf(stderr, "-f requires filename! Correct usage: ZadanieGrafy2.exe -f path/to/filename.json");
                return 1;
            }
        }
        else {
            fprintf(stderr, "Unrecognized argument %s! Use: ZadanieGrafy2.exe -h for help", argv[i]);
            return 1;
        }
    }

    // ****************
    // IMPORT JSON DATA
    // ****************

    // IMPORT THROUGH FILE
    // char* source = parseSrc("data/g0.json");

    // IMPORT THROUGH STDIN
    // char* source = parseStdIn();

    char* source = NULL;

    if(inputFile)
        source = parseSrc(filename);
    else
        source = parseStdIn();

    if(!source) return 1;

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

    if(debug) {
        printf("Name -> Id map:\n");
        for(int i = 0; i < v; i++) printf("V: %s ID: %d\n", names[i], i);
    }

    // PREPARE ADJACENCY ARRAY
    // this can bo optimized for memory alocation, but not now :)
    int** adjArr = malloc(v * sizeof(int*));
    for(int i = 0; i < v; i++) {
        adjArr[i] = malloc(v * sizeof(int));
        memset(adjArr[i], 0, v * sizeof(int));
    }
    if(!cj_setAdjArr(jsonObj, v, adjArr, names)) { rc = 1; goto clean_3; }

    if(debug) {
        printf("Adjacency array:\n");
        for(int i = 0; i < v; i++) {
            printf("[");
            for(int j = 0; j < v; j++) printf("%d,", adjArr[i][j]);
            printf("]\n");
        }
    }
    

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
        identity,
        debug
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