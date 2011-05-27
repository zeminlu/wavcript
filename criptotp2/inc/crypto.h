/**
 * \file crypto.h
 *
 *  \date Created on: 11-apr-2011
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */
 
#ifndef _CRYPTO_
#define _CRYPTO_

#include "definitions.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/evp.h>

typedef struct evp_cipher_st EVP_CIPHER; 
  
typedef struct evp_cipher_ctx_st EVP_CIPHER_CTX; 

typedef const EVP_CIPHER * (*evpCipherFunc) ();

unsigned int cryptWithPass(void *inData, long data_len, void *outData, t_opt crypt_mode, t_alg algorithm, t_mode mode, char *pass);

unsigned int cryptWithKey(void *inData, long data_len, void *outData, t_opt crypt_mode, t_alg algorithm, t_mode mode, char *key, char *iv);

unsigned int cryptMe(void *inData, long data_len, void *outData, evpCipherFunc type, t_opt crypt_mode, unsigned char *key, unsigned char *iv);

#endif