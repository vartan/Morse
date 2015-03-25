#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>

/**
 * Contains all data required to generate morse code
 */
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
/**
 * Gets a morseChar for an input char
 * @param  c char which you would like to get morse data on
 * @return   morseChar which contains data to use morse with that char
 */
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
/**
 * Create Morse String
 * @param morse       struct morseChar which contains the morse data
 * @param morseString buffer to put the morse string in, will use -. for dit/dah
 */
void createMorseString(struct morseChar *morse, char *morseString) {
    int i;
    for(i = 0; i < morse->length; i++) {
        // add next morse character
        (*morseString++) = ((morse->sounds >> i) & 1) ? '-' : '.';
    }
    // add null terminator
    *morseString = 0; 
}

/**
 * Plays morse on terminal in real time
 * @param str       word to play
 * @param inPlace   true if you want the function to use only one character.
 */
void playMorseWord(char *str, bool inPlace) {
    const int TIME_BETWEEN_WORDS = 7;
    const int TIME_BETWEEN_CHAR = 3;
    const int TIME_BETWEEN_ELEMENT = 1;
    const int TIME_DIT = 1;
    const int TIME_DAH = 3;

    int i;
    bool dah;
    float timeUnit = 0.100 * 1.0e6; // 100ms dit time

    while(*str != 0) {
        struct morseChar *morse = getMorseForChar(*str);
        if(morse != NULL) {
            for(i = 0; i < morse->length; i++) {
                // add next morse character
                dah = ((morse->sounds >> i) & 1) ? true : false;
                // print next character, if in place, stay on character by 
                // backspace
                printf("%c", dah ? '-' : '.');
                if(inPlace) printf("\b");
                fflush(stdout);

                // sleep appropriate time
                usleep(timeUnit * (dah ? TIME_DAH : TIME_DIT));

                // erase dit/dah when not active
                if(inPlace) {
                    printf(" \b");
                    fflush(stdout);
                }
                // sleep between elements
                usleep(timeUnit * (TIME_BETWEEN_ELEMENT));
            } // end of ditdah element 
            printf(" ");
            if(inPlace) printf("\b");
            fflush(stdout);
            usleep(timeUnit * (TIME_BETWEEN_CHAR - TIME_BETWEEN_ELEMENT));
        } // end of letter
        str++;
    } // end of word
    if(!inPlace) {
        printf("/ ");
        fflush(stdout);
    }
    usleep(timeUnit * 
        (TIME_BETWEEN_WORDS - TIME_BETWEEN_CHAR - TIME_BETWEEN_ELEMENT));


}


int main(int argc, char *argv[]) {

    char morseString[6];                /**< 5 character string + null 
                                        terminator to contain morse letter */

    struct morseChar *currentMorseChar; /**< current morse character       */
    int start = 1;
    bool inPlace = false;

    // debugging information: show how large each morse char takes
    printf("Size of struct: %lu\n", sizeof(struct morseChar));
    if(atoi(argv[1]) > 1) {
        inPlace = true;
        start++;
        printf("Morse in place: [ ]\b\b");
    }
    // loop through each word in args
    for(int i = start; i < argc; i++) {
        // point to next word
        char *str = argv[i];
        playMorseWord(str, inPlace);
    }
}