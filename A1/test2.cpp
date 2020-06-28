#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <cstdint>

using namespace std;

int main(int, char**) {
    int word_count = 0;
    int line_count = 0;
    int char_count = 0;
    bool inside_word = false;
    int c;
    char buff[1024];
    int fd = STDIN_FILENO;

    int k = read( fd, buff, sizeof(buff));

    for(int i = 0; i < 1024; i++){
        char a = getchar();
        std:: cout << a << endl;
    }







}
