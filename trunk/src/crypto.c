/**
 * \file crypto.c
 *
 *  \date Created on: 11-apr-2011
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */
 
#include "../inc/crypto.h"

evpCipherFunc evpCipherFuncs[4][4] = {
    {EVP_aes_128_ecb, EVP_aes_192_ecb, EVP_aes_256_ecb, EVP_des_ecb},
    {EVP_aes_128_cfb8, EVP_aes_192_cfb8, EVP_aes_256_cfb8, EVP_des_cfb8},
    {EVP_aes_128_ofb, EVP_aes_192_ofb, EVP_aes_256_ofb, EVP_des_ofb},
    {EVP_aes_128_cbc, EVP_aes_192_cbc, EVP_aes_256_cbc, EVP_des_cbc}
};

long cryptWithPass(void *inData, long data_len, void *outData, t_opt crypt_mode, t_alg algorithm, t_mode mode, char *pass){
    evpCipherFunc type = evpCipherFuncs[mode][algorithm];
    int keySize = (algorithm == AES256) ? 32 : (algorithm == AES192 ? 24 : 16);
    int derivedKeySize, cryptSize;
    unsigned char *key = malloc(keySize);
    unsigned char *iv = malloc(keySize);    
    
    if ((derivedKeySize =  EVP_BytesToKey(type(), EVP_md5(), NULL, (unsigned char *)pass, strlen(pass), 1, key, iv)) != keySize){
        printf("keySize & derivedKeySize differ, keySize = %d, derivedKeySize = %d\n", keySize, derivedKeySize);
        return -1;
    }
        
    cryptSize = cryptMe(inData, data_len, outData, type, crypt_mode, key, iv);
    
    varFree(2, key, iv);
    
    return cryptSize;
}

long cryptWithKey(void *inData, long data_len, void *outData, t_opt crypt_mode, t_alg algorithm, t_mode mode, char *key, char *iv){
    int cryptSize;
    evpCipherFunc type = evpCipherFuncs[mode][algorithm];
    
    cryptSize = cryptMe(inData, data_len, outData, type, crypt_mode, (unsigned char *)key, (unsigned char *)iv);
    
    return cryptSize;
}

int cryptMe(void *inData, long data_len, void *outData, evpCipherFunc type, t_opt crypt_mode, unsigned char *key, unsigned char *iv){
    int outl, templ;
    
    EVP_CIPHER_CTX ctx;
    
    EVP_CIPHER_CTX_init(&ctx); 
        
    if (EVP_CipherInit_ex(&ctx, type(), NULL, key, iv, crypt_mode) == 0){
        printf("Error en cipherInit\n");
        return -1;
    }
    
    if (EVP_CipherUpdate(&ctx, outData, &outl, inData, data_len) == 0){
        printf("Error en cipherUpdate\n");
        return -1;
    }
    
    if (EVP_CipherFinal_ex(&ctx, (unsigned char *) outData + outl, &templ) == 0){
        printf("Error en cipherFinal\n");
        return -1;
    }
    
    if (EVP_CIPHER_CTX_cleanup(&ctx) == 0){
        printf("Error en cipherCleanup\n");
        return -1;
    }
        
    if (outl + templ != data_len){
        printf("El tamaño de los datos encriptados es distinto al de los datos desencriptados, outl = %d, templ = %d\n", outl, templ);
        return -1;
    }
    
    return outl + templ;
}