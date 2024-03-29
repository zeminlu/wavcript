/**
 * \file stegAlgorithms.c
 *
 *  \date Created on: 23-may-2011
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */
 
#include "../inc/stegAlgorithms.h"
#include "../inc/cutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef _SA_BOOLS_
#define SA_TRUE 1
#define SA_FALSE 0
#endif

/**
 * Private methods definitions
 * ===========================
 */
unsigned char getTargetByte(void *ret, unsigned int i, int sampleLength);

void *lsbNUnhide(void *message, long messageLen, int sampleLength,
		unsigned int *hiddenMessageSize, char **hiddenMessageExtension, int n,
		int expectsExtension, int bigEndianWav, int enhancedMode);

void *hideMessage(void *carrier, long carrierLen, int sampleLength,
		void *msgToHide, int msgToHideLen, int n, int bigEndianWav,
		int enhanceMode);

/**
 * Public Methods
 * ==============
 */

// Hide related methods.

void *lsbNHide(void *carrier, long carrierLen, int sampleLength,
		void *msgToHide, unsigned int msgToHideLen, char *msgToHideExtension,
		int n, int bigEndianWav) {
	int extLen = 
		(msgToHideExtension == NULL) ? 0 : strlen(msgToHideExtension) + 1;
	int completeMsgLen = 4 + msgToHideLen + extLen;
	// Will include the size and extension (if available)
	unsigned char* completeMsg = malloc(completeMsgLen);
	if (completeMsg == NULL) {
		fprintf(stderr, "Error memory\n");
		return NULL;
	}
	unsigned int msgToHideLenEndianIndependant = msgToHideLen;
	endian_swap(&msgToHideLenEndianIndependant);
	memcpy(completeMsg, &msgToHideLenEndianIndependant, 4);
	memcpy(completeMsg + 4, msgToHide, msgToHideLen);
	if (extLen > 0) {
		printf("The file to hide has an extension %s\n", msgToHideExtension);
		memcpy(completeMsg + 4 + msgToHideLen, msgToHideExtension, extLen);
	}
	int enhancedMode = SA_FALSE;
	if (n == 0) {
		// Operating in enhaced mode => n = 1, enhancedMode = True
		n = 1;
		enhancedMode = SA_TRUE;
		sampleLength = 8;
	}
	return hideMessage(carrier, carrierLen, sampleLength, completeMsg,
			completeMsgLen, n, bigEndianWav, enhancedMode);
}

void *lsbNHideCrypted(void *carrier, long carrierLen, int sampleLength,
		void *msgToHide, int msgToHideLen, int n, int bigEndianWav) {
	return lsbNHide(carrier, carrierLen, sampleLength, msgToHide,
			msgToHideLen, NULL, n, bigEndianWav);
}

// Extract related methods.

void *lsbNExtract(void *message, long messageLen, int sampleLen,
		unsigned int *hiddenMessageSize, char **hiddenMessageExtension,
		int n, int bigEndianWav) {
	int enhancedMode = SA_FALSE;
	if (n == 0) {
		// Operating in enhaced mode => n = 1, enhancedMode = True
		n = 1;
		enhancedMode = SA_TRUE;
		sampleLen = 8;
	}
	return lsbNUnhide(message, messageLen, sampleLen, hiddenMessageSize,
			hiddenMessageExtension, n, SA_TRUE, bigEndianWav,
			enhancedMode);
}

void *lsbNExtractCrypted(void *message, long messageLen, int sampleLen,
		unsigned int *hiddenMessageSize, int n, int bigEndianWav) {
	int enhancedMode = SA_FALSE;
	if (n == 0) {
		// Operating in enhaced mode => n = 1, enhancedMode = True
		// sampleLength = 8
		n = 1;
		enhancedMode = SA_TRUE;
		sampleLen = 8;
	}
	return lsbNUnhide(message, messageLen, sampleLen, hiddenMessageSize,
			NULL, n, SA_FALSE, bigEndianWav, enhancedMode);
}

/**
 * Private Methods
 * ===============
 */

/**
 * Returns the corresponding target byte depending on the wav
 * endianism.
 */
unsigned char *getTargetBytePointer(void *ret, int i, int sampleLength,
		int bigEndianWav) {
	if (bigEndianWav) {
		return ((unsigned char *) ret + i + (sampleLength / 8) - 1);
	} else {
		return ((unsigned char *) ret + i);
	}	
}

void *hideMessage(void *carrier, long carrierLen, int sampleLength,
		void *msgToHide, int msgToHideLen, int n, int bigEndianWav,
		int enhancedMode) {
	if (carrierLen <= 0 || sampleLength <= 0 || (sampleLength % 8) != 0
			|| msgToHideLen <= 0 || n > 8 || n < 0) {
		fprintf(stderr, "Illegal parameters.\n");
		return NULL;
	}
	void *ret = malloc(carrierLen);
	if (ret == NULL) {
		return ret;
	}
	char *sampleBuff = malloc(sampleLength);
	if (sampleBuff == NULL) {
		free(ret);
		return NULL;
	}
	int i = 0, j = 0, b = 0;
	unsigned char carrierMask = 0xFF << n; // leaves the space for n bits
	for (; i < carrierLen && j < msgToHideLen; i += sampleLength / 8) {
		// Copiamos una muestra a la salida.
		memcpy((unsigned char *) ret + i,
			(unsigned char *) carrier + i, sampleLength / 8);
		if (enhancedMode && *((unsigned char *) ret + i) != 0xFF
				&& *((unsigned char *) ret + i) != 0xFE) {
			continue;
		}
		// Al ultimo byte de la muestra le ponemos el hueco para poner n bits
		// En carryingByte quedo lo que habia antes con n ceros al final
		// para pisar.
		unsigned char carryingByte =
			//*((char *) ret + i + (sampleLength / 8) - 1)
			*getTargetBytePointer(ret, i, sampleLength, bigEndianWav)
			& carrierMask;
		unsigned char nZerosInTheBack = 0xFF << (8 - n);
		// LeavingMask se usa para obtener del mensaje a ocultar los bits
		// correspondientes para esta iteracion.
		unsigned char leavingMask = 
			(
			// Deja n unos al principio
			((unsigned char) ~((nZerosInTheBack >> n) | (nZerosInTheBack << (8 -n))))
			// Que movemos a la derecha b veces para arrancar en el indice correcto.
			>> b);

		// Al byte que corresponde el mensaje a esconder extraemos los
		// n bits que corresponden. 
		unsigned char bitsToPlace =
			// Al byte original
			((unsigned char) (*((unsigned char *) msgToHide + j)
			// Le dejamos solo los n que nos interesan.
			& leavingMask))
			// Que luego movemos al final
			>> (8 - (b + n));
		// Al LSB lo pisamos con el resultante
		*getTargetBytePointer(ret, i, sampleLength, bigEndianWav) =
		//*((unsigned char *) ret + i + sampleLength / 8 - 1) =
			carryingByte | bitsToPlace;
		b += n;
		if (b > 7) {
			b %= 8;
			++j;
		}
	}
	if (i == carrierLen) {
		fprintf(stderr, "No space in carrier to hide msg with the given n=%d\n", n);
		fprintf(stderr, "The maximum capacity of the carrier is %d bytes\n",
				(carrierLen / sampleLength) * n / 8);
		fprintf(stderr, "Size of message to hide: %d\n", msgToHideLen);
		free(ret);
		return NULL;
	}
	if (j == msgToHideLen) {
		memcpy((unsigned char *) ret + i,
			(unsigned char *) carrier + i, carrierLen - i + 1);
	}
	return ret;
}

// Si no tiene una extension null terminada devuelve NULL en
// *hiddenMessageExtension
void *lsbNUnhide(void *message, long messageLen, int sampleLength,
		unsigned int *hiddenMessageSize, char **hiddenMessageExtension, int n,
		int expectsExtension, int bigEndianWav, int enhancedMode) {
	if (messageLen <= 0 || sampleLength <= 0 || (sampleLength % 8) != 0
			|| message == NULL || hiddenMessageSize == NULL || n > 8
			|| n < 0) {
		fprintf(stderr, "Illegal parameters.\n");
		return NULL;
	}
	if (expectsExtension) {
		*hiddenMessageExtension = NULL;	
	}
	void *ret = calloc(1, 1);
	unsigned char carrierMask = ~(0xFF << n); // leaves the space for n bits
	unsigned int i = 0, j = 0, b = 0;
	unsigned int msgSize = 0;
	int isInExtension = 0;
	for (; i < messageLen; i += (sampleLength / 8)) {
		// printf("DEBUG: i: %d, j: %d b:%d\n", i, j, b);
		unsigned char info = 
			*getTargetBytePointer(message, i, sampleLength, bigEndianWav);
		if (enhancedMode && info != 0xFF && info != 0xFE) {
			continue;
		}
		if (j == 4 && b == 0 && msgSize == 0) {
			// Ya tenemos el numero
			memcpy(&msgSize, ret, 4); 
			endian_swap(&msgSize);
			printf("The size of the hidden msg is: %d bytes\n", msgSize);
			*hiddenMessageSize = msgSize;
			if (msgSize > messageLen || msgSize < 0) {
				fprintf(stderr, "The message hidden size is aparrently bigger" \
						" than the total message or less than 0. This is invalid.\n");
				free(ret);
				return NULL;
			}
			// Ponemos j = 0 para pisar en la salida el numero
			j = 0;
		}
		if (j != 0 && j == msgSize && b == 0) {
			if (expectsExtension) {
				// Arranca la extension
				isInExtension = 1;
				printf("The extension begins\n");
			} else {
				// Sino terminamos (pq no se espera extension)
				printf("No extension expected. Ending\n");
				break;
			}
		}
		if (isInExtension && j > msgSize && b == 0) {
			// Ya podría estar el null
			if (*((unsigned char *) ret + j - 1) == '\0') {
				printf("La extension del archivo es: %s\n",
					(unsigned char *) ret + msgSize);
				char *ext = (char *) ret + msgSize;
				int extLen = strlen(ext);
				*hiddenMessageExtension = malloc(extLen + 1);
				memcpy(*hiddenMessageExtension, ext, extLen + 1);
				break;
			}
		}
		// Al ultimo byte de la muestra nos quedamos solo con los n
		// que importan.
		unsigned char data =
			//*((unsigned char *) message + i + (sampleLength / 8) - 1)
			*getTargetBytePointer(message, i, sampleLength, bigEndianWav)
			& carrierMask;
		unsigned char currentValue =
			*((unsigned char *) ret + j);
		*((unsigned char *) ret + j) =
			currentValue | (data << (8 - (b + n)));
		b += n;
		if (b > 7) {
			b %= 8;
			++j;
			ret = realloc(ret, j + 2);
			*((unsigned char *) ret + j) = 0;
		}
	}
	return (char *) ret;
}

/* Testing main
int main(int argc, char **argv) {
	void *carrier = malloc(sizeof(char *));
	long carrierSize = readFile("carrier", &carrier);
	printf("Read %d chars of carrier\n", (int) carrierSize);

	void *msg = malloc(sizeof(char *));
	long msgSize = readFile("message", &msg);
	printf("Read %d chars of message to hide\n", (int) msgSize);

	char *stegMsg = lsbNHide(carrier, carrierSize, 8, msg, msgSize, ".html", 4);
	printf("---Steg msg---\n");
	print(stegMsg, carrierSize);
	printf("---End of steg msg---\n");

	unsigned int hiddenMessageSize;
	char *resp = lsbNExtract(stegMsg, carrierSize, 8, &hiddenMessageSize, NULL, 4);
	printf("Obtained hidden msg of size %d\n", hiddenMessageSize);
	printf("---Hidden Msg---\n");
	print(resp, hiddenMessageSize);
	printf("---End of hidden msg---\n");

	return EXIT_SUCCESS;
} */
