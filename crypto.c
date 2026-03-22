#include "crypto.h"
#include <string.h>
#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/sha.h>


int aes_encrypt(const unsigned char *plaintext, int plaintext_len,
                unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;
    int len = 0;
    int ciphertext_len = 0;

    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fprintf(stderr, "aes_encrypt: failed to create context\n");
        return -1;
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, AES_KEY, AES_IV) != 1) {
        fprintf(stderr, "aes_encrypt: init failed\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len) != 1) {
        fprintf(stderr, "aes_encrypt: update failed\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    ciphertext_len = len;

    if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1) {
        fprintf(stderr, "aes_encrypt: final failed\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}


int aes_decrypt(const unsigned char *ciphertext, int ciphertext_len,
                unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;
    int len = 0;
    int plaintext_len = 0;

    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fprintf(stderr, "aes_decrypt: failed to create context\n");
        return -1;
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, AES_KEY, AES_IV) != 1) {
        fprintf(stderr, "aes_decrypt: init failed\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1) {
        fprintf(stderr, "aes_decrypt: update failed\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    plaintext_len = len;

    if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1) {
        fprintf(stderr, "aes_decrypt: final failed\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}


void sha256_hash(const char *input, char *output_hex)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX *ctx;
    unsigned int len = 0;
    int i;

    ctx = EVP_MD_CTX_new();
    if (!ctx) {
        fprintf(stderr, "sha256: context creation failed\n");
        output_hex[0] = '\0';
        return;
    }

    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(ctx, input, strlen(input));
    EVP_DigestFinal_ex(ctx, hash, &len);

    EVP_MD_CTX_free(ctx);

    /* convert raw bytes to hex string */
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output_hex + (i * 2), "%02x", hash[i]);
    }
    output_hex[64] = '\0';
}
