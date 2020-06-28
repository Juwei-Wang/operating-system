//
// Created by jay on 2020-05-20.
//

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <cstdint>

using namespace std;

int main(int, char**)

{
    int word_count = 0;
    int line_count = 0;
    int char_count = 0;
    bool inside_word = false;
    char c;
    char buff[1024];
    int fd = STDIN_FILENO;


    while (1){
        int k = read( fd, buff, sizeof(buff));
        if (k<1024){
            for (int i = 0; i < k; ++i) {
                c = buff[i];
                char_count ++;
                if( c == '\n') {
                    line_count ++;
                }
                if( isspace(c)) {
                    inside_word = false;
                } else {
                    // update word count if starting a new word
                    if( ! inside_word) word_count ++;
                    inside_word = true;
                }
            }
            break;
        }
        for(int i = 0; i < 1024; i++){
            c = buff[i];
            char_count ++;
            if( c == '\n') {
                line_count ++;
            }
            if( isspace(c)) {
                inside_word = false;
            } else {
                // update word count if starting a new word
                if( ! inside_word) word_count ++;
                inside_word = true;
            }
        }

    }

    printf( "%7lu %7lu %7lu\n",
            line_count,
            word_count,
            char_count
    );


    return 0;
}