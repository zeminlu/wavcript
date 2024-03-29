/**
 * \file main.c
 *
 *  \date Created on: 11-apr-2011
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */

#include "../inc/main.h"

int main (int argc, char* argv[]) {
	t_input *inputStruct = parseInput(argc, argv);

	//Preconditions check.
	if (inputStruct == NULL) {
		printf("Wrong input.\n");
		return EXIT_FAILURE;
	}
	
/*	if(inputStruct == NULL) {
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
*/	
    void *sound;
    
    WaveFile *wf = malloc(sizeof(WaveFile));
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
    
    void *data, *decryptData, *toCryptData, *cryptData, *stegData, *hiddenData;    
    unsigned int dataSize, hiddenDataSize, cryptSize, toCryptSize, filenamelength;
    char *extension;
         	
	//Without crypt:
	//embed: get data size, get carrier size, get null terminated extension, call steg algorithm, save file
	//extract: unsteg wav file, save data file
	
	//With crypt:
	//embed: get data size, get carrier size, get null terminated extension, encrypt (data size + data + extension), get encrypt size, steg, save file
	//extract: unsteg wav file, decrypt (data size + data + extension), save file     
		
	if (inputStruct->stegMode == EMB){
        if ((dataSize = (unsigned int) readFile(inputStruct->input, &data)) <= 0){
            return -1;
        }
        
        extension = getFileExtension(inputStruct->input);
        if (inputStruct->pass != NULL){
            toCryptSize = 4 + dataSize + strlen(extension) + 1;
            toCryptData = malloc (sizeof(char) * toCryptSize);      
            
            endian_swap(&dataSize); //ENDIANNN
            memcpy(toCryptData, (void *) &dataSize, sizeof(char) * 4);
            endian_swap(&dataSize); //NO ENDIANNN
            
            memcpy((char *)toCryptData + 4, data, dataSize);
            memcpy((char *)toCryptData + (4 + dataSize), extension, strlen(extension) + 1);    
    	    
    	    cryptData = malloc(sizeof(char) * (toCryptSize + 32)); //el 32 es el pulmoncito para el padding    	    
    	    if ((cryptSize = (unsigned int) cryptWithPass(toCryptData, toCryptSize, cryptData, inputStruct->operation, inputStruct->algorithm, inputStruct->mode, inputStruct->pass)) == -1){
				printf("Maybe bad pass\n");
				return -1;
			}
			if ((stegData = lsbNHideCrypted(sound, wf->chunkdatasize, wf->wBitsPerSample, cryptData, cryptSize, inputStruct->stegAlg, inputStruct->endianMode)) == NULL){
                return -1;
            }
                        
            varFree(2, toCryptData, cryptData);      
        } else {
            if ((stegData = lsbNHide(sound, wf->chunkdatasize, wf->wBitsPerSample, data, dataSize, extension, inputStruct->stegAlg, inputStruct->endianMode)) == NULL){
                return -1;
            }
        }
        
        WaveFile_Write(inputStruct->output, wf, stegData);
        
        varFree(3, data, extension, stegData);
	} else {    	    
	    if (inputStruct->pass != NULL){
	        if ((hiddenData = lsbNExtractCrypted(sound, wf->chunkdatasize, wf->wBitsPerSample, &hiddenDataSize, inputStruct->stegAlg, inputStruct->endianMode)) == NULL){
                return -1;
	        }
    	
	        decryptData = malloc(sizeof(char) * hiddenDataSize);
            if ((cryptSize = (unsigned int) cryptWithPass(hiddenData, hiddenDataSize, decryptData, inputStruct->operation, inputStruct->algorithm, inputStruct->mode, inputStruct->pass)) == -1){
				printf("Maybe bad pass?\n");
				return -1;
			}
        	memcpy(&dataSize, decryptData, sizeof(char) * 4);
        	endian_swap(&dataSize); //ENDIANNNNN
            data = malloc(sizeof(char) * dataSize);
            memcpy(data, (char *)decryptData + 4, dataSize);

            extension = malloc(sizeof(char) * (cryptSize - 4 - dataSize));
        	memcpy(extension, (char *)decryptData + (4 + dataSize), cryptSize - 4 - dataSize);
            
            varFree(2, decryptData, hiddenData);
	    } else {
	        if ((data = lsbNExtract(sound, wf->chunkdatasize, wf->wBitsPerSample, &dataSize, &extension, inputStruct->stegAlg, inputStruct->endianMode)) == NULL){
                return -1;
	        }
	    }
	    
        filenamelength = strlen(inputStruct->output) + (extension == NULL ? 0 : strlen(extension)) + 1;
        char *filename = calloc(1, sizeof(char) * filenamelength);
        
        strcpy(filename, inputStruct->output);
        if (extension != NULL){
			strcat(filename, extension);
		}
        if (writeFile(filename, data, (long) dataSize) <= 0){
            return -1;
        }

        varFree(2, data, filename);

		if (extension != NULL){
			free(extension);	
		}else{
			printf("No extension, probably bad steg alg\n");
		}
		
	}    	
    
    varFree(2, wf, sound);
	
	return 0;
}

