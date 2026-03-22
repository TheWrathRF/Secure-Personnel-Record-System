#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/evp.h>
#include <openssl/rand.h>

/* 
 * AES-256-CBC needs a 32-byte key and 16-byte IV.
 * Hardcoded for now.
 * (yes i know hardcoding keys is bad, will be fixed  )
 */
static const unsigned char AES_KEY[32] = {
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
    0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E,
    0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76
};

static const unsigned char AES_IV[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

/*
 * Encrypts plaintext with AES-256-CBC.
 * Returns ciphertext length on success, -1 on failure.
 * Caller must ensure `ciphertext` buffer is large enough
 * (plaintext_len + EVP_MAX_BLOCK_LENGTH should be safe).
 */
int aes_encrypt(const unsigned char *plaintext, int plaintext_len,
                unsigned char *ciphertext);

/*
 * Decrypts ciphertext with AES-256-CBC.
 * Returns plaintext length on success, -1 on failure.
 */
int aes_decrypt(const unsigned char *ciphertext, int ciphertext_len,
                unsigned char *plaintext);

/*
 * Hashes `input` string with SHA-256.
 * Writes the result as a 64-char lowercase hex string + null terminator
 * into `output_hex` (must be at least 65 bytes).
 */
void sha256_hash(const char *input, char *output_hex);

#endif /* CRYPTO_H */
