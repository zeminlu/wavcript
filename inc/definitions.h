/**
 * \file definitions.h
 *
 *  \date Created on: 11-apr-2011
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */
 
#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>

#include "types.h"

/**
 * \def TRUE
 *		\brief TRUE define
 */
#define		TRUE		1

/**
 * \def FALSE
 *		\brief FALSE define
 */
#define		FALSE		!TRUE

/**
 * \def _stdout_
 *		\brief Define of the standard output descriptor
 */
#define		_stdout_	1

/**
 * \def _stdin_
 *		\brief Define of the standard input descriptor
 */
#define		_stdin_		0

/**
 * \fn void varFree(int amm, ...)
 *
 * 		\brief This function frees up the variable pointers given as arguments
 *
 * 		\param amm The ammount of pointers being passed to this function
 * 		
 * 		\return
 *
 * 		Use:
 * 		\code
 * 		
 *		\endcode
 *
 * 		\sa
 *
 */
void varFree(int amm, ...);

#define INVALID_OPT -1;

#endif