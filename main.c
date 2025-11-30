/*
NAME:VIJAYALAXMI SHANKARGOUDA BIRADAR.
DATE:08/09/2025.
PROJECT:LEXICAL ANALYZER.
*/


#include <stdio.h>
#include "token.h"

int main(int argc,char* argv[])
{
    if(argc<2)
    {
        printf("ERROR: Should contain 2 arguments\nUsage: ./a.out file_name\n");
        return 1;
    }
    if(openFile(argv[1]) != 0) return 1;  //pass file name to lexical


    while(nexttoken());// reading tokens until EOF

    printHeader();

    return 0;
}