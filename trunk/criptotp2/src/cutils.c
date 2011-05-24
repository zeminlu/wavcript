/**
 * \file cutils.c
 *
 *  \date Created on: 11-apr-2011
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */
 
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "../inc/cutils.h"

char * getFileExtension(char *filename){
    char *extension = calloc(1, sizeof(char *) * 32);
    char *peek = filename + filename [strlen (filename) - 1];
    while (peek >= filename)
    {
        if (*peek == '.')
        {
            strcpy (extension, peek + 1);
            break;
        }
        peek--;
    }
    return extension;
}

long writeFile(const char *filename, void *data, long filesize){
    FILE *file;
  
    if ((file = fopen(filename, "wb")) != NULL){
        fwrite(data, filesize, 1, file);
        fclose(file);
      
        return(1);
    } else {
        printf("Error in opening the file!\n");
    
        return(0);
    }
}

long readFile(const char *filename, void **data){
    FILE *file;
    int amm = 0;
    
    if ((file = fopen(filename, "rb")) != NULL){
        *data = (char *) malloc(sizeof(char *));
    
        while (fread(((char *)data + amm), sizeof(char *), 1, file) != EOF){
            amm += 1;
            *data = (char*) realloc(data, sizeof(char *) * amm + 1);
        }
     
        fclose(file);
     
        return(amm);
    } else {
        printf("Error in opening the file!\n"); 
     
        return(0);
    }
}

void reverse(char s[]){
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char s[]){
    int i, sign;

    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void sToLower(char **string){
    int i;
    for (i = 0 ; i < strlen(*string) ; ++i){
        (*string)[i] = tolower((*string)[i]);
    }
    return;
}