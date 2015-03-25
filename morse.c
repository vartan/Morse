#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
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
    }
    else if(c >= 'A' && c <= 'Z') {
        return &morseValues[10+c-'A'];
    }
    else if(c>='0' && c<='9') {
        return &morseValues[c-'0'];
    }
    return NULL;
}
void createMorseString(struct morseChar *morse, char *morseString) {
    int i;
    for(i = 0; i < morse->length; i++) {
        // add next morse character
        (*morseString++) = ((morse->sounds >> i) & 1) ? '-' : '.';
    }
    // add null terminator
    *morseString = 0; 
}
void playMorseWord(char *str);
int main(int argc, char *argv[]) {

    char morseString[6];                /**< 5 character string + null 
                                        terminator to contain morse letter */

    struct morseChar *currentMorseChar; /**< current morse character       */
    
    // debugging information: show how large each morse char takes
    printf("Size of struct: %lu\n", sizeof(struct morseChar));

    // loop through each word in args
    for(int i = 1; i < argc; i++) {
        // point to next word
        char *str = argv[i];
        playMorseWord(str);
        // loop through each character
        /*while(*str!=0) { 
            // get morseChar pointer from character
            currentMorseChar = getMorseForChar(*str);
            if(currentMorseChar != NULL) {
                // generate morse string from morse char
                //createMorseString(currentMorseChar, &morseString[0]);
                // print morse char
                //printf("%s ", morseString);
            } else {
                printf("\n\nERROR: Could not find %c\a\n\n", *str);

                exit(1);
            }
            str++;
        }*/
        // print / between words
    }
}
/**
 * Plays morse on terminal
 * @param str word to play
 */
void playMorseWord(char *str) {
    int i;
    bool dah;
    float timeUnit = 0.05*1e6;
    char backspace[3] = {0, 0, 0};

    printf(" ");
    while(*str != 0) {
        struct morseChar *morse = getMorseForChar(*str);
        if(morse != NULL) {
            for(i = 0; i < morse->length; i++) {
                // add next morse character
                dah = ((morse->sounds >> i) & 1) ? true : false;
                printf("%s%c", backspace, dah?'_':'.');
                fflush(stdout);
                usleep(timeUnit * 3);
                if(dah) usleep(timeUnit * 3);

                printf("%s", backspace);
                fflush(stdout);
                usleep(timeUnit * 1);
            }
            printf(" %s", backspace);
            fflush(stdout);
            usleep(timeUnit * 3);

        }
        str++;
    }
    printf(" / %s", backspace);
    fflush(stdout);
    usleep(timeUnit * 27);



}