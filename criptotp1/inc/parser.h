/**
 * \file parser.h
 *
 *  \date Created on: 12-apr-2011
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */

#ifndef _PARSER_
#define _PARSER_

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "definitions.h"
#include "cutils.h"

t_input * parseInput(int argc, char *argv[]);

void initInWrongValues(t_input* input);

boolean wrongInput(t_input* input);

t_mode parseMode(char *mode);

t_alg parseAlgorithm(char *algorithm);

#endif