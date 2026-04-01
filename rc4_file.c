#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s input_file output_file password\n", argv[0]);
        return 1;
    }

    FILE *fin = fopen(argv[1], "rb");
    if (fin == NULL) {
        printf("Error: Cannot open input file.\n");
        return 1;
    }

    fseek(fin, 0, SEEK_END);
    long filesize = ftell(fin);
    rewind(fin);

    unsigned char *buffer = (unsigned char*)malloc(filesize);
    unsigned char *output = (unsigned char*)malloc(filesize);

    if (buffer == NULL || output == NULL) {
        printf("Memory allocation error.\n");
        fclose(fin);
        free(buffer);
        free(output);
        return 1;
    }

    fread(buffer, 1, filesize, fin);
    fclose(fin);

    rc4_crypt((unsigned char*)argv[3], strlen(argv[3]), buffer, filesize, output);

    FILE *fout = fopen(argv[2], "wb");
    if (fout == NULL) {
        printf("Error: Cannot open output file.\n");
        free(buffer);
        free(output);
        return 1;
    }

    fwrite(output, 1, filesize, fout);
    fclose(fout);

    printf("Done. Output written to %s\n", argv[2]);

    free(buffer);
    free(output);

    return 0;
}