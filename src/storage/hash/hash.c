#include <stdlib.h>
#include <stdio.h>
#include <openssl/evp.h>
#include "hash.h"


/*
 * uses sha-256 hash of a file 
 * returns a dynamically allocated 65-character hex string (64 hex chars + '\0')
 * caller calls free() on the returned pointer
 * 
 * 
 * ZAHAHAHA IM POLARIS(THIS IS USELESS)
 * 
 * 
 */


 char *ComputeFile_sha256(const char *filepath)
 {
     FILE *file = fopen(filepath, "rb");

     if (!file)
     {
        // the file didnt open -> bad so error
        perror("Error : failed to open the file\n");
        return NULL;
     }

     //allocate on openssl digest context
     EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
     if (!mdctx)
     {
         fclose(file);
         return NULL;
     }


    //  initialise sha256 context
    if(EVP_DigestInit_ex(mdctx,EVP_sha256(),NULL)!=1)
    {
        EVP_MD_CTX_free(mdctx);
        fclose(file);
        return NULL;
    }

    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;

    // stream the file in chuncks to keep memory usage low

    while( (bytes_read= fread(buffer,1,BUFFER_SIZE,file)) > 0 )
    {
        if(EVP_DigestUpdate(mdctx,buffer,bytes_read)!=1)
        {
            EVP_MD_CTX_free(mdctx);
            fclose(file);
            return NULL;
        }
    }

    // final digest execution
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;

    if(EVP_DigestFinal_ex(mdctx,hash,&hash_len)!=1)
    {
        EVP_MD_CTX_free(mdctx);
        fclose(file);
        return NULL;
    }

    //cleaning after myself (get it hehe ok my bad)

    EVP_MD_CTX_free(mdctx);
    fclose(file); // best prcatise to alawys close  :)

    //change raw byte into hex string ending with "\0"
    //32 bytes * 2 chars/byte +1 null terminator = 65 bytes or something
    char *hexstr = malloc(hash_len * 2 + 1);
    if(!hexstr)
        return NULL;

    for (unsigned int i = 0; i < hash_len;i++)
    {
        sprintf(hexstr+(i*2),"%02x",hash[i]);
    }
    hexstr[hash_len * 2] = '\0';

    return hexstr;
 }
