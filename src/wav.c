#include "../inc/wav.h"

short WaveFile_Write(const char *filename, WaveFile *wf, void *data){
    FILE *wfile;
  
    if ((wfile = fopen(filename, "wb")) != NULL){
        fwrite(wf, sizeof(WaveFile), 1, wfile);
        fwrite(data, wf->chunkdatasize, 1, wfile);
        fclose(wfile);
      
        return(1);
    } else {
        printf("Error in opening the file!\n");
    
        return(0);
    }
}

short WaveFile_Read(const char *filename, WaveFile *wf, void **data){
    FILE *wfile;
    
    if ((wfile = fopen(filename, "rb")) != NULL){
        fread(wf, sizeof(WaveFile), 1, wfile);
        *data = (char*) malloc(wf->chunkdatasize);
        fread(*data, wf->chunkdatasize, 1, wfile);
        fclose(wfile);
        
        return(1);
    } else {
        printf("Error in opening the file!\n");
    
        return(0);
    }
}
