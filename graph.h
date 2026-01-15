#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "cJSON.h"

#define MAP_SIZE 1024

/**
    Hashmap entry structure
*/
typedef struct Entry {
    char* key;
    int value;
    struct Entry* next;
} Entry;

/**
    djb2 hash function

    @param const char* pointer to string

    @return unsigned hash
*/
unsigned hash(const char* e);

/**
    Puts key:value pair into the map

    @param const char* key
    @param int value
*/
void mapPut(const char* key, int value);

/**
    Retrieves the value based on a given key

    @param const char* key
    @param int* pointer to designated value holder

    @return bool 0 on failure
*/
int mapGet(const char* key, int* value);

/**
    Debug function
    Prints whole hash map
*/
void printMap();

/**
    Clears up map
*/
void clearMap();

/**
    Maps names to ids

    @param cJSON* pointer to a graph in cJSON format

    @return bool 0 on failure
*/
int cj_mapIdName(cJSON* json);

/**
    Cleans and sets the adjacency array

    @param cJSON* pointer to a graph in cJSON format
    @param int* pointer to adj array

    @return bool 0 on failure
*/
int cj_setAdjArr(cJSON* json, int v, int arr[v][v]);

/**
    Returns cJSON array length

    @param cJSON* pointer to a graph in cJSON format
    @param string array name

    @return int vertices count
*/
int cj_arrayLength(cJSON* json, char* arrayName);

/**
    Writes mapped ids to given array

    @param cJSON* pointer to a graph in cJSON format
    @param string array name
    @param int* pointer to destination array

    @return bool 0 on failure
*/
int cj_getIds(cJSON* json, char* arrayName, int* arr);

#endif // GRAPH_H_INCLUDED
