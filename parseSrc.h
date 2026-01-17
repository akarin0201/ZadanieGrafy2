#ifndef PARSESRC_H_INCLUDED
#define PARSESRC_H_INCLUDED

/**
    Returns string from file

    @param char* relative path to source file

    @return json string
*/
char* parseSrc(char* pathToSrc);

/**
    Returns string from stdin

    @return json string
*/
char* parseStdIn();

#endif // PARSESRC_H_INCLUDED
