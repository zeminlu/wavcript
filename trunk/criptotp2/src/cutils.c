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

void endian_swap(unsigned int * x){
    *x = (*x>>24) | ((*x<<8) & 0x00FF0000) | ((*x>>8) & 0x0000FF00) | (*x<<24);
    
    return;
}

char * getFileExtension(char *filename){
    char *extension = calloc(1, sizeof(char *) * 32);
    char *peek = filename + filename [strlen(filename) - 1];
    while (peek >= filename){
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
        return 1;
    } else {
        fprintf(stderr, "Error in opening the file!\n");
        return -1;
    }
}

long readFile(const char *filename, void **data){
    FILE *file;
    int i = 0;
    
    if ((file = fopen(filename, "rb")) != NULL){
        *data = (char *) malloc(1);
    
        while (fread(((char *) *data + i), 1, 1, file) != 0) {
			++i;
            *data = (char*) realloc(*data, i + 1);
        }
     
        fclose(file);
     
        return i;
    } else {
        fprintf(stderr, "Error in opening the file!\n"); 
        return -1;
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
