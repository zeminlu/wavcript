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

void *lsbNUnhide(void *message, long messageLen, int sampleLength,
		unsigned int *hiddenMessageSize, char **hiddenMessageExtension, int n,
		int expectsExtension);

void *hideMessage(void *carrier, long carrierLen, int sampleLength,
		void *msgToHide, int msgToHideLen, int n);

void printBits(unsigned char bits) {
	unsigned char mask = 0x80;
	int i;
	for (i = 0; i < 8; ++i) {
		if (mask & bits) {
			printf("1");
		} else {
			printf("0");
		}
		mask = mask >> 1;
	}
}

/**
 * Public Methods
 * ==============
 */

/**
 * Hides the given msgToHide of length msgToHideLen in carrier of length
 * carrierLen. Will return a new message (should be freed after use) with
 * the msgToHide hidden in the given carrier or null if some error was
 * encountered. Note that the size of the resulting message is the same
 * as the carrier's message size. The *carrier should be pointing to the
 * first sample to modify (not the header). Sample length is in bits.
 */
void *lsbNHide(void *carrier, long carrierLen, int sampleLength,
		void *msgToHide, unsigned int msgToHideLen, char *msgToHideExtension,
		int n) {
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
		memcpy(completeMsg + 4 + msgToHideLen, msgToHideExtension, extLen);
	}
	return hideMessage(carrier, carrierLen, sampleLength, completeMsg,
			completeMsgLen, n);
}

void *lsbNHideCrypted(void *carrier, long carrierLen, int sampleLength,
		void *msgToHide, int msgToHideLen, int n) {
	return lsbNHide(carrier, carrierLen, sampleLength, msgToHide,
			msgToHideLen, NULL, n);
}

void *hideMessage(void *carrier, long carrierLen, int sampleLength,
		void *msgToHide, int msgToHideLen, int n) {
	if (carrierLen <= 0 || sampleLength <= 0 || (sampleLength % 8) != 0
			|| msgToHideLen <= 0) {
		fprintf(stderr, "Illegal parameters.");
		return NULL;
	}
	if (n > 8) {
		fprintf(stderr, "n should be <= 8");
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
	for (; i < carrierLen && j < msgToHideLen; i += (sampleLength / 8)) {
		printf("DEBUG: i: %d, j: %d b:%d\n", i, j, b);
		// Copiamos una muestra a la salida.
		memcpy((unsigned char *) ret + i,
			(unsigned char *) carrier + i, (sampleLength / 8));
		// Al ultimo byte de la muestra le ponemos el hueco para poner n bits
		printf("Original: '%c' - ", *((char *) ret + i +
			(sampleLength / 8) - 1));
		// En carryingByte quedo lo que habia antes con n ceros al final
		// para pisar.
		unsigned char carryingByte =
			*((char *) ret + i + (sampleLength / 8) - 1) & carrierMask;
		printf("CarryingB: '%c' - ", carryingByte);
		unsigned char nZerosInTheBack = 0xFF << (8 - n);
		// LeavingMask se usa para obtener del mensaje a ocultar los bits
		// correspondientes para esta iteracion.
		unsigned char leavingMask = 
			(
			// Deja n unos al principio
			((unsigned char) ~((nZerosInTheBack >> n) | (nZerosInTheBack << (8 -n))))
			// Que movemos a la derecha b veces para arrancar en el indice correcto.
			>> b);
		printf("Byte being hide: ");
		printBits(*((unsigned char *) msgToHide + j));
		printf("Leaving mask: ");
		printBits(leavingMask);

		// Al byte que corresponde el mensaje a esconder extraemos los
		// n bits que corresponden. 
		unsigned char bitsToPlace =
			// Al byte original
			((unsigned char) (*((unsigned char *) msgToHide + j)
			// Le dejamos solo los n que nos interesan.
			& leavingMask))
			// Que luego movemos al final
			>> (8 - (b + n));
		printf(" BitsToPlace: "); 
		printBits(bitsToPlace);
		printf("\n");
		// Al LSB lo pisamos con el resultante
		*((unsigned char *) ret + i + sampleLength / 8 - 1) =
			carryingByte | bitsToPlace;
		printf("result: '%c'\n",
			*((unsigned char *) ret + i + sampleLength / 8 - 1));
		b += n;
		if (b > 7) {
			b %= 8;
			++j;
		}
	}
	if (i == carrierLen) {
		fprintf(stderr, "No space in carrier to hide msg with the given n");
		free(ret);
		return NULL;
	}
	return ret;
}

void print(char *string, unsigned int len) {
	unsigned int i = 0;
	for (; i < len; ++i) {
		printf("%c", *(string + i));
	}
}

void *lsbNExtract(void *message, long messageLen, int sampleLen,
		unsigned int *hiddenMessageSize, char **hiddenMessageExtension, int n) {
	return lsbNUnhide(message, messageLen, sampleLen, hiddenMessageSize,
			hiddenMessageExtension, n, SA_TRUE);
}

void *lsbNExtractCrypted(void *message, long messageLen, int sampleLen,
		unsigned int *hiddenMessageSize, int n) {
	return lsbNUnhide(message, messageLen, sampleLen, hiddenMessageSize,
			NULL, n, SA_FALSE);
}

void *lsbNUnhide(void *message, long messageLen, int sampleLength,
		unsigned int *hiddenMessageSize, char **hiddenMessageExtension, int n,
		int expectsExtension) {
	if (messageLen <= 0 || sampleLength <= 0 || (sampleLength % 8) != 0
			|| message == NULL || hiddenMessageSize == NULL) {
		fprintf(stderr, "Illegal parameters.");
		return NULL;
	}
	if (n > 8) {
		fprintf(stderr, "n should be <= 8");
		return NULL;
	}
	void *ret = calloc(1, 1);
	unsigned char carrierMask = ~(0xFF << n); // leaves the space for n bits
	int i = 0, j = 0, b = 0;
	unsigned int msgSize = 0;
	int isInExtension = 0;
	for (; i < messageLen; i += (sampleLength / 8)) {
		printf("DEBUG: i: %d, j: %d b:%d\n", i, j, b);
		if (j == 4 && b == 0) {
			// Ya tenemos el numero
			memcpy(&msgSize, ret, 4); 
			printf("The size of the hidden msg is: %d\n", msgSize);
			*hiddenMessageSize = msgSize;
			endian_swap(hiddenMessageSize);
		}
		if (j == msgSize && b == 0) {
			if (expectsExtension) {
				// Arranca la extension
				isInExtension = 1;
				printf("The extension begins");
			} else {
				// Sino terminamos (pq no se espera extension)
				printf("No extension expected. Ending");
				break;
			}
		}
		if (isInExtension && j > msgSize) {
			// Ya podría estar el null
			if (((unsigned char *) ret + j - 1) == '\0') {
				printf("La extension del archivo es: %s",
					(unsigned char *) ret + msgSize);
				char *ext = (char *) ret + msgSize;
				int extLen = strlen(ext);
				*hiddenMessageExtension = malloc(extLen);
				memcpy(*hiddenMessageExtension, ext, extLen);
				break;
			}
		}
		printf("Original: '%c' - ", *((unsigned char *) message + i +
			(sampleLength / 8) - 1));
		// Al ultimo byte de la muestra nos quedamos solo con los n
		// que importan.
		unsigned char data =
			*((unsigned char *) message + i +
					(sampleLength / 8) - 1) & carrierMask;
		printf("data: ");
		printBits(data);
		unsigned char currentValue =
			*((unsigned char *) ret + j);
		printf(" before update: ");
		printBits(currentValue);
		*((unsigned char *) ret + j) =
			currentValue | (data << (8 - (b + n)));
		printf("result: '%c'\n",
				*((unsigned char *) ret + j));
		b += n;
		if (b > 7) {
			b %= 8;
			++j;
			ret = realloc(ret, j + 2);
			*((unsigned char *) ret + j ) = 0;
		}
	}
	return (char *) ret + 4;
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