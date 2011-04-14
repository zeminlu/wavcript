/**
 * \file main.c
 *
 *  \date Created on: 11-apr-2011
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */

#include "../inc/main.h"

int main (int argc, char* argv[]) {

	t_input * inputStruct = parseInput(argc, argv);
    void *sound;
    WaveFile *wf = malloc(sizeof(WaveFile));
    WaveFile_Read("../la-fa.wav", wf, &sound);
    printf("riff: %c%c%c%c\nfilesize: %d\nrifftype: %c%c%c%c\nchunk_format_id: %c%c%c%c\nchunkformatsize: %d\n", 
        wf->riff[0], wf->riff[1], wf->riff[2], wf->riff[3], wf->filesize, wf->rifftype[0], wf->rifftype[1], 
        wf->rifftype[2], wf->rifftype[3], wf->chunk_format_id[0], wf->chunk_format_id[1], wf->chunk_format_id[2], 
        wf->chunk_format_id[3], wf->chunkformatsize);
    printf("wFormatTag: %d\nnChannels: %d\nnSamplesPerSec: %d\nnAvgBytesPerSec: %d\nnBlockAlign: %d\n",
        wf->wFormatTag, wf->nChannels, wf->nSamplesPerSec, wf->nAvgBytesPerSec, wf->nBlockAlign);
    printf("wBitsPerSample: %d\nchunk_data_id: %c%c%c%c\nchunkdatasize: %d\n", 
        wf->wBitsPerSample, wf->chunk_data_id[0], wf->chunk_data_id[1], 
        wf->chunk_data_id[2], wf->chunk_data_id[3], wf->chunkdatasize);
    
    void *invSound = malloc(wf->chunkdatasize);
    memcpy(invSound, (char *) sound + (wf->chunkdatasize / 2), wf->chunkdatasize / 2);
    memcpy((char *) invSound + (wf->chunkdatasize / 2), sound, wf->chunkdatasize / 2);
    
    WaveFile_Write("../la-fa-post.wav", wf, invSound);
    
    varFree(2, sound, invSound);
	
	return 0;
}

