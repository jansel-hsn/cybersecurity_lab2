#include <stdio.h>
#include <stdlib.h>

int main() {
    unsigned char c1[] = {0xBB, 0xF3, 0x16, 0xE8, 0xD9, 0x40};
    unsigned char c2[] = {0x06, 0xA9, 0x21, 0xEA, 0xD9, 0x41};

    char m1_guess[] = "Attack";
    char m2_recovered[7];

    printf("Attempting to recover M2...\n");

    for (int i = 0; i < 6; i++) {
        m2_recovered[i] = c1[i] ^ c2[i] ^ m1_guess[i];
    }

    m2_recovered[6] = '\0';

    printf("Recovered Message 2: %s\n", m2_recovered);

    return 0;
}