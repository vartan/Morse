#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct morseChar {
    uint8_t length:3; /**< number of sounds in the word               */
    uint8_t sounds:5; /**< list of sounds, where 0th bit is the first */
}; // pack into one byte

struct morseChar morseValues[] = {
    {5, 0x1F}, //0: -----
    {5, 0x1E}, //1: .----
    {5, 0x1C}, //2: ..---
    {5, 0x18}, //3: ...--
    {5, 0x10}, //4: ....-
    {5, 0x00}, //5: .....
    {5, 0x01}, //6: -....
    {5, 0x03}, //7: --...
    {5, 0x07}, //8: ---..
    {5, 0x0F}, //9: ----.
    {2, 0x02}, //A: .-
    {4, 0x01}, //B: -...
    {4, 0x05}, //C: -.-.
    {3, 0x01}, //D: -..
    {1, 0x00}, //E: .
    {4, 0x04}, //F: ..-.
    {3, 0x03}, //G: --.
    {4, 0x00}, //H: ....
    {2, 0x00}, //I: ..
    {4, 0x0E}, //J: .---
    {3, 0x05}, //K: -.-
    {4, 0x02}, //L: .-..
    {2, 0x03}, //M: --
    {2, 0x01}, //N: -.
    {3, 0x07}, //O: ---
    {4, 0x06}, //P: .--.
    {4, 0x0B}, //Q: --.-
    {3, 0x02}, //R: .-.
    {3, 0x00}, //S: ...
    {1, 0x01}, //T: -
    {3, 0x04}, //U: ..-
    {4, 0x08}, //V: ...-
    {3, 0x06}, //W: .--
    {4, 0x09}, //X: -..-
    {4, 0x0D}, //Y: -.--
    {4, 0x03}  //Z: --..
};

struct morseChar *getMorseForChar(char c) {
        if(c >= 'a' && c <= 'z') {
            return &morseValues[10+c-'a'];
        } else if(c >= 'A' && c <= 'Z') {
            return &morseValues[10+c-'A'];
        } else if(c>='0' && c<='9') {
            return &morseValues[c-'0'];
        }

        return NULL;
}
void createMorseString(struct morseChar *morse, char *morseString) {
    int i;
    for(i = 0; i < morse->length; i++) {
        (*morseString++) = ((morse->sounds>>i)&1)?'-':'.';
    }
    (*morseString++) = 0;
}
int main(int argc, char *argv[]) {
    char morseString[6];
    printf("Size of struct: %lu\n", sizeof(struct morseChar));
    for(int i=1;i<argc;i++) {
        char *str = argv[i];
        while(*str!=0) {
            struct morseChar *a = getMorseForChar(*str);
            if(a!=NULL) {
                createMorseString(a, &morseString[0]);
                printf("%s ", morseString);
            } else {
                printf("\n\nCould not find %c\n\n", *str);
                exit(1);
            }
            str++;
        }
        printf("/ ");

    }
}
