/*
curr_time= 0
jobs_remaining= size of job queue

while(1) {
 if jobs_remaining== 0 break
 if process in cpu is done
 mark process done
 set CPU idle
 jobs_remaining--continue

 if a new process arriving
 add new process to RQ
 continue

 if cpu is idle and RQ not empty
 move process from RQ to CPU
 continue

 execute one burst of job on CPU
 curr_time++

 }
*/
#include <stdio.h>
#include <ctype.h>
#include <vector>
#include <string>
#include <cassert>
#include <unordered_map>
#include <set>
#include <list>

typedef std::vector<std::string> vs_t;

vs_t
split( const std::string & p_line)
{
    auto line = p_line + " ";
    vs_t res;
    bool in_str = false;
    std::string curr_word = "";
    for( auto c : line) {
        if( isspace(c)) {
            if( in_str)
                res.push_back(curr_word);
            in_str = false;
            curr_word = "";
        }
        else {
            curr_word.push_back(c);
            in_str = true;
        }
    }
    return res;
}

bool
parseLine( std::string & fdigit, std::string & sdigit)
{
    char buff[4096];
    vs_t words;

    // read in a line and parse it into words
    while(1) { // loop to skip empty lines
        if( ! fgets( buff, sizeof(buff), stdin))
            return false;
        words = split(buff);
        if( words.size() != 0) break;
    }

    if( words.size() != 2) {
        printf("Bad line: %s\n", buff);
        exit( -1); // exception would be nicer
    }

    fdigit = words[0];
    sdigit = words[1];

    printf("%s", fdigit);
    printf("%s", sdigit);



    printf("Bad line: %s\n", buff);
    exit( -1); // exception would be nicer
};



int
main()
{

    G

    while(true) {
        std::string fdigit, sdigit;

        if( ! parseLine(fdigit, sdigit)) break;

    }


    return 0;
}