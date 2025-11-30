#include<stdio.h>
#include<ctype.h>
#include<string.h>

#include "token.h"
#include "keywords.h"

char* keywords[] ={
    "int","float","char","double",
    "signed","unsigned","short","long",
    "typedef","enum",
    "auto","register","static","extern",
    "for","while","do",
    "if","else","switch","case","default",
    "goto","continue","break",
    "struct","union",
    "void","return",
    "const","volatile","sizeof"};

    //int total_keywords = 32;
    int f;
    int iskeyword(char* word)
    {
        for(int i = 0 ; i<32; i++)
        {
            if(strcmp(word,keywords[i]) == 0)
            return 1;  //true it is keyword
        }
        return 0;   //not keyword
    }
