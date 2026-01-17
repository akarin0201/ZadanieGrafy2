#ifndef SOLVE_H_INCLUDED
#define SOLVE_H_INCLUDED

/**
    Prints paths according to draft

    @param int vertices count
    @param char** id:name map
    @param int** adjacency array
    @param int amount of input vertices
    @param int* input vertices ids array
    @param int amount of output vertices
    @param int* output vertices ids array
    @param int* verterx identity array
*/
void printPaths(
    int v,
    char** map,
    int** adjArr,
    int inC,
    int* in,
    int outC,
    int* out,
    int* identity
);

#endif // SOLVE_H_INCLUDED
