#include <stdlib.h>
#include <stdio.h>
#include "parseSrc.h"
#include "cJSON.h"

char* parseSrc(char* pathToSrc) {

    // OPEN FILE HOOK
    FILE* src = fopen(pathToSrc, "r");
    if(!src) {
        printf("Failed to open file at \"%s\", aborting!\n", pathToSrc);
        return NULL;
    }

    // GET THE FILE SIZE
    fseek(src, 0, SEEK_END);
    long srcSize = ftell(src);
    rewind(src); // move indicator back to the beggining of the file

    // PREPARE A BUFFER FOR THE DATA
    char* buffer = malloc(srcSize + 1); // +1 to fit \0
    if(!buffer) {
        printf("Failed to create a buffer, aborting!\n");
        fclose(src);
        return NULL;
    }

    // READ DATA INTO THE BUFFER
    fread(buffer, 1, srcSize, src);
    buffer[srcSize] = '\0';

    // CLOSE FILE HOOK
    fclose(src);

    return buffer;
}

char* parseStdIn() {
    char* source = NULL;
    size_t size = 0;
    size_t len = 0;
    int c;
    int braces = 0;
    int s = 0;

    while((c = getchar()) != EOF) {

        if(!s) {
            if(c == '{') {
                s = 1;
                braces = 1;
            }
        }
        else {
            if(c == '{') braces++;
            if(c == '}') braces--;
        }

        if(len + 1 >= size) {
            size = size ? size * 2 : 1024;
            source = realloc(source, size);
            if(!source) return 1;
        }
        source[len++] = (char)c;

        if(s && braces == 0) break;
    }

    source[len] = '\0';

    return source;
}