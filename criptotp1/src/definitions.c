/**
 * \file definitions.c
 *
 *  \date Created on: 11-apr-2011
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */

#include "definitions.h"

void varFree(int amm, ...){
	va_list ap;
	int i;

	va_start(ap, amm);
	for (i = 0 ; i < amm ; ++i)
	{
		free(va_arg(ap, void *));
	}
	va_end(ap);
}