#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "cJSON.h"

int cj_arrayLength(cJSON* json, char* arrayName) {
    cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, arrayName);
    if(cJSON_IsArray(arr))
        return cJSON_GetArraySize(arr);

    return 0;
}

int cj_mapIdName(cJSON* json, int v, char** map) {
    cJSON* vertices = cJSON_GetObjectItemCaseSensitive(json, "Vertices");
    if(!vertices)
        return 0;

    cJSON* name = NULL;
    int i = 0;
    cJSON_ArrayForEach(name, vertices) {
        strcpy(map[i], name->valuestring);
        i++;
    }

    return 1;
}

int cj_mapGetId(int v, char** map, char* name, int* id) {
    for(int i = 0; i < v; i++) {
        if(strcmp(name, map[i]) == 0) {
            *id = i;
            return 1;
        }
    }

    return 0;
}

int cj_setAdjArr(cJSON* json, int v, int** arr, char** map) {

    // prepare edges object
    cJSON* edges = cJSON_GetObjectItemCaseSensitive(json, "Edges");
    if(!cJSON_IsObject(edges)) return 0;

    // prepare vertex array
    cJSON* vertex = NULL;
    cJSON_ArrayForEach(vertex, edges) {

        // get current vertex id
        int vertexId = -1;
        if(!cj_mapGetId(v, map, vertex->string, &vertexId)) return 0;

        // iterate through every edge and populate adjacency array
        cJSON* edge = NULL;
        cJSON_ArrayForEach(edge, vertex) {
            int neighbourId = -1;
            if(!cj_mapGetId(v, map, edge->valuestring, &neighbourId)) return 0;
            // printf("Mapping %s of id %d -> %s of id %d\n", vertex->string, vertexId, edge->valuestring, neighbourId);
            arr[vertexId][neighbourId] = 1;
        }
    }

    return 1;
}

int cj_getIds(cJSON* json, char* arrayName, int* arr, int v, char** map) {

    cJSON* jsonArr = cJSON_GetObjectItemCaseSensitive(json, arrayName);
    if(cJSON_IsArray(jsonArr)) {
        cJSON* item = NULL;
        int c = 0;
        cJSON_ArrayForEach(item, jsonArr) {
            int itemId = -1;
            if(!cj_mapGetId(v, map, item->valuestring, &itemId)) return 0;
            // printf("Assigning %s of id %d to index %d in %s array\n", item->valuestring, itemId, c, arrayName);
            // printf("arr[%d] = %d in %s\n", c, itemId, arrayName);
            arr[c++] = itemId;
        }

        return 1;
    }

    return 0;
}