#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int contaPal(char* str){
    char* p = str;
    int palavras;
    for(palavras = 0;(*p) != '\0';p++)
        if((*p) == ' ') palavras++;
    palavras++;
    return palavras;
}

char** separa(char* str){
    int palavras = contaPal(str);
    char** separa;
    char* p = str;
    int j, i;
    separa = malloc(sizeof(char*) * palavras+1);
    for(i = 0;i<palavras;i++){
        separa[i] = malloc(sizeof(char) * 1024);
        char* pal = p;
        for(j = 0;(*pal) != ' ' && (*pal) != '\0';j++,pal++){
            separa[i][j] = (*pal);
        }
        pal++;
        p = pal;
        j++;
        separa[i][j] = '\0';
    }
    separa[palavras] = malloc(sizeof(char)*1);
    separa[palavras] = NULL;
    
    return separa;
}
