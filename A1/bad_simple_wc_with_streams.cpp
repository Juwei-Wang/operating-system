//
// DO NOT USE THIS FILE TO IMPLEMENT YOUR ASSIGNMENT !!!
//
// this is a rewrite of simple_wc.cpp to make it run much faster
// 
// it uses the getchar() function from C streams library, which you
// are not allowed to use in your solution
//
// your assignment solution must use read() and nothing else
//
// DO NOT USE THIS FILE TO IMPLEMENT YOUR ASSIGNMENT !!!
//

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <cstdint>

char buff[1024];


int my_getchar(){
    int fd = STDIN_FILENO;
    bool inside_word = false;
    int words = read(fd, buff, sizeof(buff));




    return 0;

    // using read
    // do i still have stuff in buff
    // if yes , take one thing out of the buffer
    //otherwise used read to replenish buffer


}


int main( int, char **)
{
  // our boolean mini-state, true = we are in a middle of a word
  bool inside_word = false;
  // setup 3 counters
  int word_count = 0;
  int line_count = 0;
  int char_count = 0;
  // read file character by character and count words
  while(1) {
    // read in a single character
    int c = getchar();
    std::cout << c << std::endl;
    if( c == EOF) break; // quit loop on EOF
    // count every character
    char_count ++;
    // count new lines
    if( c == '\n') {
      line_count ++;
    }
    // update our state
    if( isspace(c)) {
      inside_word = false;
    }
    else {
      // update word count if starting a new word
      if( ! inside_word) word_count ++;
      inside_word = true;
    }
  }

  // print results
  printf( "%7d %7d %7d\n",
	  line_count,
	  word_count,
	  char_count
	  );

  return 0;
}
