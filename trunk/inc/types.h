/**
 * \file types.h
 *
 *  \date Created on: 3-apr-2010
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */

#ifndef TYPES_H_
#define TYPES_H_

/*
 * Aca todas las estructuras
 */

typedef unsigned char boolean;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned char BYTE;

typedef DWORD FOURCC; 		// Four-character code
typedef FOURCC CKID;		// Four-character-code chunk identifier
typedef DWORD CKSIZE;		// 32-bit unsigned size value

struct wavStr {
	RIFF_CK riff_desc;		// MANDATORY
	FMT_CK fmt;				// Format Chunk MANDATORY
	//FACT_CK fact;			// Fact Chunk OPTIONAL
	//CUE_CK cue;			// Cue points Chunk OPTIONAL
	//PLIST_CK	plist;		// Playlist Chunk OPTIONAL
	//LIST_CK list;			// Associated data list Chunk OPTIONAL 
	// more optional data...
	DATA_CK data;			// Wave Data Chunk MANDATORY
};

typedef struct {
	CKID chunkID;			// 'RIFF' 
	CKSIZE chunkSize; 		// File Size
	CKID format;			// Format: 'WAVE'
} RIFF_CK;

typedef struct {
	CKID chunkID;			//'fmt ' 
	CKSIZE chunkSize;		// 16 para PCM.Size of rest of subchunk. 
	/* Common fields */
	WORD wFormatTag;		// Format category,i.e.:PCM = 1 (no compres.)
	WORD wChannels;			// Number of channels:1, mono; 2, stereo
	DWORD dwSamplesPerSec;	// Sampling rate: Mhz
	DWORD dwAvgBytesPerSec; 
	WORD wBlockAlign 
	WORD wBitsPerSample;	//8, 16, etc.
	WORD extraParamSize;	// If PCM, doesn't exist
	BYTE *extraParams;		//space for extra params
} FMT_CK;

typedef struct {
	CKID chunkID;			// 'data' 
	CKSIZE chunkSize;		// Bytes of data 
	BYTE *soundData;		// Sound data.
} DATA_CK;

typedef enum {
	ENC = 1,
	DEC
} t_opt;

typedef enum {
	AES128 = 1,
	AES192,
	AES256,
	DES
} t_alg;

typedef enum {
	ECB = 1,
	CFB,
	OFB,
	CBC
} t_mode;

typedef struct {
	char *input;			// Input file's path
	char *output;			// Output file's path
	t_opt operation;		// Encryption or decryption
	char *pass;				// Password
	char *key;				// Key generated from password
	char *iv;				// Initialization vector
	t_alg algorithm;		// Encryption or decryption algorithm used.
	t_mode mode;			// Encryption or decryption mode used.
} t_input;

#endif
