#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "cJSON.h"

/**
    Returns cJSON array length or 0 if the obiect is not countable

    @param cJSON* pointer to a graph in cJSON format
    @param string array name

    @return int vertices count
*/
int cj_arrayLength(cJSON* json, char* arrayName);

/**
    Maps names to ids

    @param cJSON* pointer to a graph in cJSON format
    @param char** array of strings

    @return bool 0 on failure
*/
int cj_mapIdName(cJSON* json, int v, char** map);

/**
    Saves id of given name to specified int

    @param int vertices count
    @param char** pointer to an id:name map
    @param string name
    @param int* pointer to destination

    @return 0 if no id was found
*/
int cj_mapGetId(int v, char** map, char* name, int* id);

/**
    Cleans and sets the adjacency array

    @param cJSON* pointer to a graph in cJSON format
    @param int vertices count
    @param int** pointer to adj array
    @param char** pointer to an id:name map

    @return bool 0 on failure
*/
int cj_setAdjArr(cJSON* json, int v, int** arr, char** map);

/**
    Writes mapped ids to given array

    @param cJSON* pointer to a graph in cJSON format
    @param string json array name
    @param int* pointer to destination array
    @param int map length
    @param char** pointer to an id:name map

    @return bool 0 on failure
*/
int cj_getIds(cJSON* json, char* arrayName, int* arr, int v, char** map);

#endif // GRAPH_H_INCLUDED
