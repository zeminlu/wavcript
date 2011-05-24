/**
 * \file main.c
 *
 *  \date Created on: 11-apr-2011
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */

#include "../inc/main.h"

int main (int argc, char* argv[]) {
    long res;
	t_input *inputStruct = parseInput(argc, argv);

	//Preconditions check.
	if (inputStruct == NULL) {
		printf("Wrong input.\n");
		return EXIT_FAILURE;
	}
	
	if(inputStruct == NULL) {
		printf("TODO MAL \n");
		printf("input = %s\n", inputStruct->input);
		printf("output = %s\n", inputStruct->output);
		printf("operation = %d\n", inputStruct->operation);
		printf("pass = %s\n", inputStruct->pass);
		printf("key = %s\n", inputStruct->key);
		printf("iv = %s\n", inputStruct->iv);
		printf("algorithm = %d\n", inputStruct->algorithm);
		printf("mode = %d\n", inputStruct->mode);
		printf("\n");
//		return 0;
	} else {
		printf("TODO BIEN!!!! \n");
		printf("input = %s\n", inputStruct->input);
		printf("output = %s\n", inputStruct->output);
		printf("operation = %d\n", inputStruct->operation);
		printf("pass = %s\n", inputStruct->pass);
		printf("key = %s\n", inputStruct->key);
		printf("iv = %s\n", inputStruct->iv);
		printf("algorithm = %d\n", inputStruct->algorithm);
		printf("mode = %d\n", inputStruct->mode);
		printf("\n");
//		return 0;
	}
	
    void *sound, *cryptSound;
    WaveFile *wf = malloc(sizeof(WaveFile));
    //WaveFile_Read("../la-fa.wav", wf, &sound);
	//Levantamos el wav.
    WaveFile_Read(inputStruct->carrier, wf, &sound);
    printf("riff: %c%c%c%c\nfilesize: %d\nrifftype: %c%c%c%c\nchunk_format_id: %c%c%c%c\nchunkformatsize: %d\n", 
        wf->riff[0], wf->riff[1], wf->riff[2], wf->riff[3], wf->filesize, wf->rifftype[0], wf->rifftype[1], 
        wf->rifftype[2], wf->rifftype[3], wf->chunk_format_id[0], wf->chunk_format_id[1], wf->chunk_format_id[2], 
        wf->chunk_format_id[3], wf->chunkformatsize);
    printf("wFormatTag: %d\nnChannels: %d\nnSamplesPerSec: %d\nnAvgBytesPerSec: %d\nnBlockAlign: %d\n",
        wf->wFormatTag, wf->nChannels, wf->nSamplesPerSec, wf->nAvgBytesPerSec, wf->nBlockAlign);
    printf("wBitsPerSample: %d\nchunk_data_id: %c%c%c%c\nchunkdatasize: %d\n", 
        wf->wBitsPerSample, wf->chunk_data_id[0], wf->chunk_data_id[1], 
        wf->chunk_data_id[2], wf->chunk_data_id[3], wf->chunkdatasize);
    
    cryptSound = malloc(wf->chunkdatasize);
    
    //res = cryptWithPass(sound, wf->chunkdatasize, cryptSound, wf->operation, wf->algorithm, wf->mode, char *pass);
    //res = cryptWithPass(sound, wf->chunkdatasize, cryptSound, 1, 1, 0, "12345678");
       
	if (inputStruct->pass == NULL) {
    	//no password, steg without encription
    	
    	//embed: get data size, get carrier size, get null terminated extension, call steg algorithm, save file
    	//extract: unsteg wav file, save data file
    	
        void *data;
        long dataSize, carrierSize = wf->chinkdatasize;
        
    	if (inputStruct->stegMode == EMB){
            if ((dataSize = readFile(inputStruct->input, &data)) <= 0){
                return -1;
            }
            
            char *extension = getFileExtension(inputStruct->input);
            void *stegData = lsbNHide(sound, carrierSize, wf->wBitsPerSample, data, dataSize, extension, LSBN);
            WaveFile_Write(inputStruct->output, wf, stegData);

            varFree(2, data, extension);
    	} else {
            long dataSize;
            char *extension;
    	    void *data = lsbNExtract(sound, carrierSize, wf->wBitsPerSample, &dataSize, &extension, LSBN);
            int filenamelength = strlen(inputStruct->output) + strlen(extension);
            char filename[filenamelength + 1] = {0};
            strcopy(filename, inputStruct->output);
            strcopy(filename, ".");
            strcat(filename, extension);
            writeFile(filename, data, dataSize);
    	    
            varFree(2, data, extension);
    	}    	
	} else {
	    //password provided, steg with encription
	    
	    //embed: get data size, get carrier size, get null terminated extension, encrypt (data size + data + extension), get encrypt size, steg, save file
	    //extract: unsteg wav file, decrypt (data size + data + extension), save file
	    
	    if (inputStruct->stegMode == EMB){
    	    res = cryptWithPass(sound, wf->chunkdatasize, cryptSound, inputStruct->operation, inputStruct->algorithm, inputStruct->mode, inputStruct->pass);
        	
    	} else {
    	    long dataSize;    	    
            long decDataSize;
    	    
    	    void *data = lsbNExtractCrypted(sound, carrierSize, wf->wBitsPerSample, &dataSize, LSBN);
            void *decryptData = malloc(sizeof(char *) * dataSize);
            res = cryptWithPass(data, wf->chunkdatasize, decryptData, inputStruct->operation, inputStruct->algorithm, inputStruct->mode, inputStruct->pass);
        	
            char charSize[4];
            
            size[0] = decryptData[0];
            size[1] = decryptData[1];
            size[2] = decryptData[2];
            size[3] = decryptData[3];

            decDataSize = (long) charSize;
            
            void *hiddenData = malloc(sizeof(char *) * decDatasize);
            memcpy(hiddenData, (char *)decryptData + 4, decDataSize);
            
            int extSize = res - 4 - decDataSize;
        	char *extension = (char *)decryptData + (4 + decDataSize);
        	
            int filenamelength = strlen(inputStruct->output) + strlen(extension);
            char filename[filenamelength + 1] = {0};
            strcopy(filename, inputStruct->output);
            strcopy(filename, ".");
            strcat(filename, extension);
            writeFile(filename, data, dataSize);
    	    
            varFree(3, data, decryptData, hiddenData);
    	}
	}
    //WaveFile_Write("../la-fa-post.wav", wf, cryptSound);
    WaveFile_Write(inputStruct->output, wf, cryptSound);
    
    varFree(2, sound, cryptSound);
	
	return 0;
}

