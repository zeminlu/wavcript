/**
 * \file cutils.c
 *
 *  \date Created on: 11-apr-2011
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */
 
#include <string.h>
#include "../inc/cutils.h"

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