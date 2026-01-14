#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "parseSrc.h"

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
    int v = 0;
    cJSON* srcVertices = cJSON_GetObjectItemCaseSensitive(jsonObj, "Vertices");
    if(!srcVertices) {
        cJSON_Delete(jsonObj);
        return 1;
    }
    v = cJSON_GetArraySize(srcVertices);

    // PREPARE I:NAME MAP
    char names[v][3];
    cJSON* name = NULL;
    int i = 0;
    cJSON_ArrayForEach(name, srcVertices) {
        strncpy(names[i], name->valuestring, 2);
        names[i][2] = '\0';
        i++;
    }

    // PREPARE ADJACENCY ARRAY
    int adjArr[v][v];
    for(int q = 0; q < v; q++) {
        for(int w = 0; w < v; w++) {
            adjArr[q][w] = 0;
        }
    }

    // POPULATE ADJACENCY ARRAY
    // CREATE EDGES OBJECT
    cJSON* srcEdges = cJSON_GetObjectItemCaseSensitive(jsonObj, "Edges");
    if(!cJSON_IsObject(srcEdges)) {
        cJSON_Delete(jsonObj);
        return 1;
    }

    // CREATE EDGE ARRAY
    cJSON* srcEdgesVertex = NULL;
    cJSON_ArrayForEach(srcEdgesVertex, srcEdges) {
        
        // MAP INDEX
        int currentId = 0;
        for(int k = 0; k < v; k++) {
            if(strcmp(srcEdgesVertex->string, names[k]) == 0) {
                currentId = k;
                break;
            }
        }
        // printf("Vertex %s of id: %d, neighbours:\n", srcEdgesVertex->string, currentId);

        // FIND NEIGHBOURS
        cJSON* srcEdgesVertexEdges = NULL;
        cJSON_ArrayForEach(srcEdgesVertexEdges, srcEdgesVertex) {
            for(int k = 0; k < v; k++) {
                if(strcmp(srcEdgesVertexEdges->valuestring, names[k]) == 0) {
                    // printf("%s, of id: %d\n", srcEdgesVertexEdges->valuestring, k);
                    adjArr[currentId][k] = 1;
                    break;
                }
            }
        }
    }

    // PRINT ADJ ARRAY
    for(int z = 0; z < v; z++) {
        printf("[");
        for(int x = 0; x < v; x++) {
            printf("%d,", adjArr[z][x]);
        }
        printf("]\n");
    }

    // printf("%d", srcEdgesCount);
    cJSON_Delete(jsonObj);

    return 0;
}
