#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "cJSON.h"

#define MAP_SIZE 1024

static Entry* map[MAP_SIZE] = {0};

unsigned hash(const char* e) {
    unsigned h = 5381;

    while(*e)
        h = ((h << 5) + h) + (unsigned char)*e++;
    
    return h;
}

void mapPut(const char* key, int value) {
    unsigned idx = hash(key) % MAP_SIZE;

    // if key exists, assign value
    for(Entry* e = map[idx]; e; e = e->next) {
        if(strcmp(e->key, key) == 0) {
            e->value = value;
            return;
        }
    }

    // create new key:value
    Entry* e = malloc(sizeof *e);
    e->key = strdup(key);
    e->value = value;
    e->next = map[idx];
    map[idx] = e;
}

int mapGet(const char* key, int* value) {
    unsigned idx = hash(key) % MAP_SIZE;

    for(Entry* e = map[idx]; e; e = e->next) {
        if(strcmp(e->key, key) == 0) {
            *value = e->value;
            return 1;
        }
    }

    return 0;
}

void printMap() {
    for(unsigned i = 0; i < MAP_SIZE; i++) {
        Entry* e = map[i];
        while(e) {
            printf("%s => %d\n", e->key, e->value);
            e = e->next;
        }
    }
}

void clearMap() {
    for(int i = 0; i < MAP_SIZE; i++) {
        Entry* e = map[i];
        while(e != NULL) {
            Entry *next = e->next;
            free(e->key);
            free(e);
            e = next;
        }
        map[i] = NULL;
    }
}

int cj_countVertices(cJSON* json) {
    cJSON* vertices = cJSON_GetObjectItemCaseSensitive(json, "Vertices");
    if(!vertices)
        return 0;

    return cJSON_GetArraySize(vertices);
}

int cj_mapIdName(cJSON* json) {
    cJSON* vertices = cJSON_GetObjectItemCaseSensitive(json, "Vertices");
    if(!vertices)
        return 0;

    cJSON* name = NULL;
    int i = 0;
    cJSON_ArrayForEach(name, vertices) {
        mapPut(name->valuestring, i);
        i++;
    }

    return 1;
}

int cj_setAdjArr(cJSON* json, int v, int arr[v][v]) {

    // clean the array
    memset(arr, 0, v * v * sizeof(int));

    // prepare edges object
    cJSON* edges = cJSON_GetObjectItemCaseSensitive(json, "Edges");
    if(!cJSON_IsObject(edges)) return 0;

    // prepare vertex array
    cJSON* vertex = NULL;
    cJSON_ArrayForEach(vertex, edges) {

        // get current vertex id
        int vertexId = -1;
        if(!mapGet(vertex->string, &vertexId)) return 0;

        // iterate through every edge and populate adjacency array
        cJSON* edge = NULL;
        cJSON_ArrayForEach(edge, vertex) {
            int neighbourId = -1;
            if(!mapGet(edge->valuestring, &neighbourId)) return 0;
            printf("Mapping %s of id %d -> %s of id %d\n", vertex->string, vertexId, edge->valuestring, neighbourId);
            arr[vertexId][neighbourId] = 1;
        }
    }

    return 1;
}