#ifndef _WAVEFILE_
#define _WAVEFILE_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define WAVE_FORMAT_PCM 1

typedef struct {
/* File header */
  char riff[4];
  long filesize;
  char rifftype[4];
} RiffHeader;

typedef struct {
  char chunk_id[4];
  long chunksize;
} Chunk; 

typedef struct {
  short wFormatTag;
  short nChannels;
  long nSamplesPerSec;
  long nAvgBytesPerSec;
  short nBlockAlign;
} WAVEFORMAT;

typedef struct {
  WAVEFORMAT wf;
  short wBitsPerSample;
} PCMWAVEFORMAT;

typedef struct {
    char riff[4];
    long filesize;
    char rifftype[4];
    char chunk_format_id[4];
    long chunkformatsize;
    short wFormatTag;
    short nChannels;
    long nSamplesPerSec;
    long nAvgBytesPerSec;
    short nBlockAlign;
    short wBitsPerSample;
    char chunk_data_id[4];
    long chunkdatasize;
  // Data follows here.
} WaveFile;

void WaveFile_Init(WaveFile *wf);

void WaveFile_Set(WaveFile *wf,
  short channels,
  long samplerate,
  short datasize);

void WaveFile_Fin(WaveFile *wf,
  short channels,
  long samplerate,
  short datasize,
  long totalbyte);

short WaveFile_Write(const char *filename, WaveFile *wf, void *data);

short WaveFile_Read(const char *filename, WaveFile *wf, void **data);

float wave_length(WaveFile *wf);

#endif _WAVEFILE_