#ifndef _WAVEFILE_
#define _WAVEFILE_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define WAVE_FORMAT_PCM 1

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
} WaveFile;

short WaveFile_Write(const char *filename, WaveFile *wf, void *data);

short WaveFile_Read(const char *filename, WaveFile *wf, void **data);

#endif