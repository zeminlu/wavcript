/**
 * \file stegAlgorithms.h
 *
 *  \date Created on: 23-may-2011
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */
 
#ifndef _STEG_ALGORITHMS_
#define _STEG_ALGORITHMS_

/**
 * Hides the given msgToHide of length msgToHideLen in carrier of length
 * carrierLen. Will return a new message (should be freed after use) with
 * the msgToHide hidden in the given carrier or null if some error was
 * encountered. Note that the size of the resulting message is the same
 * as the carrier's message size. The *carrier should be pointing to the
 * first sample to modify (not the header).
 */
void *lsbNHide(void *carrier, long carrierLen, int sampleLength,
		void *msgToHide, long msgToHideLen, char *msgToHideExtension,
		int n);

/**
 * Will return the hidden message and will output in hiddenMessageSize
 * and hiddenMessageExtension the size and extension of the hidden message
 * found in message.
 */
void *lsbNExtract(void *message, long messageLen, int sampleLength,
		long *hiddenMessageSize, char *hiddenMessageExtension, int n);

#endif
