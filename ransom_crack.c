#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void rc4_crypt(unsigned char *key, int keylen, unsigned char *data, int datalen, unsigned char *output) {
    unsigned char S[256];
    int i, j = 0;
    unsigned char temp;

    for (i = 0; i < 256; i++) {
        S[i] = (unsigned char)i;
    }

    for (i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % keylen]) % 256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }

    i = 0;
    j = 0;

    for (int k = 0; k < datalen; k++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;

        temp = S[i];
        S[i] = S[j];
        S[j] = temp;

        unsigned char keystreamByte = S[(S[i] + S[j]) % 256];
        output[k] = data[k] ^ keystreamByte;
    }
}

int main() {
    unsigned char encrypted[] = {
        0x95, 0x56, 0x93, 0x54, 0x1B, 0x22, 0xCF, 0x6E,
        0xC5, 0x8C, 0x52, 0x34, 0xCC, 0x86, 0x3A
    };

    int len = sizeof(encrypted);
    unsigned char attempt_buffer[100];
    char key_guess[5];

    printf("--- Starting Ransomware Decryptor ---\n");

    for (int i = 0; i <= 9999; i++) {
        sprintf(key_guess, "%04d", i);

        rc4_crypt((unsigned char*)key_guess, 4, encrypted, len, attempt_buffer);

        attempt_buffer[len] = '\0';

        if (strstr((char*)attempt_buffer, "Secret") != NULL) {
            printf("[SUCCESS] Key found: %s\n", key_guess);
            printf("Decrypted message: %s\n", attempt_buffer);
            break;
        }
    }

    return EXIT_SUCCESS;
}