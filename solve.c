#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "solve.h"

void mSetBit(int bpw, unsigned int* mask, int b) {
    mask[b / bpw] |= (1u << (b % bpw));
}

int mTestBit(int bpw, unsigned int* mask, int b) {
    return (mask[b / bpw] >> (b % bpw)) & 1u;
}

void mSum(int words, unsigned int* result, unsigned int* a, unsigned int* b) {
    for(int i = 0; i < words; i++)
        result[i] = a[i] | b[i];
}

int mCompare(int words, unsigned int* a, unsigned int* b) {
    for(int i = 0; i < words; i++)
        if(a[i] != b[i]) return 0;
    return 1;
}

void mPrint(int words, int bpw, int vBreak, unsigned int* mask) {
    int vb = 1;
    for(int word = 0; word < words; word++) {
        for(int bit = 0; bit < bpw; bit++) {
            int currBit = (mask[word] >> bit & 1u);
            printf("%d", currBit);
            if(vb++%vBreak == 0) printf("|");
        }
        printf(" ");
    }
    printf("\n");
}

void findBestSolution(
    int maskId,
    int setsCount,
    int words,
    int minMoves,
    int moves,
    int* results,
    int* currentResult,
    unsigned int* fullCover,
    unsigned int* currentCover,
    unsigned int** masks,
    int* subSets
) {
    // if solution was found
    if(mCompare(words, fullCover, currentCover)) {
        if(moves < minMoves) {
            minMoves = moves;
            memcpy(results, currentResult, setsCount * sizeof(int));
            return;
        }
    }

    // out of range
    if(maskId >= setsCount) return;

    // check if current mask is valid
    if(!subSets[maskId]) {
        moves++;

        unsigned int* newCover = calloc(words, sizeof(unsigned int));
        mSum(words, newCover, currentCover, masks[maskId]);

        // if adding mask actually changes anything
        if(mCompare(words, newCover, currentCover) == 0) {
            currentResult[maskId] = 1;
            memcpy(currentCover, newCover, words * sizeof(unsigned int));
        }

        free(newCover);

        maskId++;
        findBestSolution(
            maskId,
            setsCount,
            words,
            minMoves,
            moves,
            results,
            currentResult,
            fullCover,
            currentCover,
            masks,
            subSets
        );
    }
    else {
        maskId++;
        findBestSolution(
            maskId,
            setsCount,
            words,
            minMoves,
            moves,
            results,
            currentResult,
            fullCover,
            currentCover,
            masks,
            subSets
        );
    }

}

void dfs(int v, int** adjArr, unsigned int* visited, int bpw, int current) {
    mSetBit(bpw, visited, current);

    for(int i = 0; i < v; i++) {
        if(adjArr[current][i] == 1 && mTestBit(bpw, visited, i) == 0)
            dfs(v, adjArr, visited, bpw, i);
    }
}

void printPaths(int v, char** map, int** adjArr, int inC, int* in, int outC, int* out, int* identity, int debug) {
    
    // ****************
    // DATA PREPARATION
    // ****************
    // PREPARE MASKS DEFINITIONS
    int mBits = v;
    int mBPW = (sizeof(unsigned int) * CHAR_BIT);
    int mWords = ((mBits + mBPW -1) / mBPW);

    // PREPARE INPUT MASKS
    unsigned int** inMasks = malloc(inC * sizeof(unsigned int*));
    for(int i = 0; i < inC; i++) inMasks[i] = calloc(mWords, sizeof(unsigned int));

    // populate masks
    for(int i = 0; i < inC; i++) {
        for(int j = 0; j < v; j++) {
            if(adjArr[in[i]][j] == 1)
                dfs(v, adjArr, inMasks[i], mBPW, j);
        }
        // mPrint(mWords, mBPW, v, inMasks[i]);
    }


    // PREPARE OUTPUT MASKS
    unsigned int** outMasks = malloc(outC * sizeof(unsigned int*));
    for(int i = 0; i < outC; i++) outMasks[i] = calloc(mWords, sizeof(unsigned int));

    // transpose adjArr
    int** adjArrT = malloc(v * sizeof(int*));
    for(int i = 0; i < v; i++) adjArrT[i] = malloc(v * sizeof(int));
    for(int i = 0; i < v; i++) {
        for(int j = 0; j < v; j++) {
            adjArrT[j][i] = adjArr[i][j];
        }
    }

    // populate masks
    for(int i = 0; i < outC; i++) {
        for(int j = 0; j < v; j++) {
            if(adjArrT[out[i]][j] == 1)
                dfs(v, adjArrT, outMasks[i], mBPW, j);
        }
        // mPrint(mWords, mBPW, v, outMasks[i]);
    }

    // PREPARE COVER MASKS & FISH OUT EMPTY SETS
    int coverC = inC * outC;
    unsigned int** coverMasks = malloc(coverC * sizeof(unsigned int*));
    for(int i = 0; i < coverC; i++) coverMasks[i] = calloc(mWords, sizeof(unsigned int));

    int* nonEmptySets = calloc(coverC, sizeof(int));

    // populate masks
    for(int i = 0; i < inC; i++) {
        for(int j = 0; j < outC; j++) {
            int k = i * outC + j;

            for(int word = 0; word < mWords; word++) {
                coverMasks[k][word] = inMasks[i][word] & outMasks[j][word];
                nonEmptySets[k] |= (coverMasks[k][word] != 0);
            }
        }
    }

    // FISH OUT SUBSETS
    int* subSets = calloc(coverC, sizeof(int));
    for(int i = 0; i < coverC; i++) {
        // assume mask is not a subset
        subSets[i] = 0;
        for(int j = 0; j < coverC; j++) {
            if(i == j) continue;

            // if mask is covered by at least one other mask
            // but is not equal to that mask
            // the mask is considered a subset
            int isEqual = 1;
            int isCovered = 1;

            for(int k = 0; k < mWords; k++) {
                // for every word in a mask

                // if at least one word is not equal, masks are not equal
                if(coverMasks[i][k] != coverMasks[j][k]) isEqual = 0;

                // if at least one word is not covered, mask is not covered
                if((coverMasks[i][k] & coverMasks[j][k]) != coverMasks[i][k]) isCovered = 0;
            }

            // if mask is not equal
            // but is covered
            // it is a subset
            if(!isEqual && isCovered) subSets[i] = 1;
        }
    }

    if(debug) {
        printf("All masks:\n");
        for(int i = 0; i < coverC; i++) {
            printf("%d:", i);
            mPrint(mWords, mBPW, v, coverMasks[i]);
        }

        printf("Non empty masks:\n");
        for(int i = 0; i < coverC; i++) {
            if(nonEmptySets[i] == 1) {
                printf("%d:", i);
                mPrint(mWords, mBPW, v, coverMasks[i]);
            }
        }

        printf("Subsets (ommiting empty masks):\n");
        for(int i = 0; i < coverC; i++) {
            if(subSets[i] == 1 && nonEmptySets[i] == 1) {
                printf("%d:", i);
                mPrint(mWords, mBPW, v, coverMasks[i]);
            }
        }
    }

    // PREPARE DESIRED COVERAGE MASK
    unsigned int* fullCoverMask = calloc(mWords, sizeof(unsigned int));
    for(int i = 0; i < v; i++) if(identity[i] == 0) mSetBit(mBPW, fullCoverMask, i);


    // ************
    // CALCULATIONS
    // ************

    // SELECTED MASKS
    int* results = calloc(coverC, sizeof(int));
    int minMoves = coverC + 1;

    int* currentResult = calloc(coverC, sizeof(int));
    unsigned int* currentCover = calloc(mWords, sizeof(unsigned int));

    for(int i = 0; i < coverC; i++) {
        if(!subSets[i]) {
            findBestSolution(
                    i,
                    coverC,
                    mWords,                         // needed for bitmask comparason
                    &minMoves,                       // current min moves
                    0,                              // current moves
                    results,                        // results array
                    currentResult,
                    fullCoverMask,
                    currentCover,
                    coverMasks,
                    subSets
                );
        }
    }

    if(debug) {
        printf("Steps to solve: %d\n", minMoves);
        printf("Selected masks:\n");
        for(int i = 0; i < coverC; i++) {
            if(results[i] == 1) {
                mPrint(mWords, mBPW, v, coverMasks[i]);
            }
        }

        printf("Sum to desired cover comparison:\n");
        mPrint(mWords, mBPW, v, currentCover);
        mPrint(mWords, mBPW, v, fullCoverMask);
    }
    printf("{ ");
    for(int i = 0; i < coverC; i++) {
        if(results[i] == 1) {
            printf("(%s, %s) ", map[in[i / outC]], map[out[i % outC]]);
        }
    }
    printf("}\n");

    // **************
    // MEMORY CLEANUP
    // **************
    free(currentCover);

    free(currentResult);

    free(results);

    free(fullCoverMask);

    free(subSets);

    for(int i = 0; i < coverC; i++) free(coverMasks[i]);
    free(coverMasks);

    for(int i = 0; i < outC; i++) free(outMasks[i]);
    free(outMasks);

    for(int i = 0; i < v; i++) free(adjArrT[i]);
    free(adjArrT);

    for(int i = 0; i < inC; i++) free(inMasks[i]);
    free(inMasks);
}