#include <iostream>
#include <set>
#include <list>
#include <fstream>
#include <cassert>
#include <unordered_map>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
using namespace std;

typedef std::vector<std::string> vs_t;
typedef std::tuple< long, long , std::string > pair1;

// split string p_line into vector of strings (words)
// the delimiters are 1 or more whitespaces
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



std::string RoundRobin(std::vector<pair1 > processes, long time_slices ){
    long waiting_job;
    bool cpu_free = true;
    long time = 0;
    int process_num = 0;
    std::vector<pair1> ready_queue;
    std::string outcome;
    waiting_job = processes.size();



    while (1){
        if(waiting_job == 0) {
            break;
        }

        // ??????????????????????????????
        //if(std::get<1>(processes[i]) <= 0){
        //    waiting_job --;
        //}

        if(time == std::get<0>(processes[process_num]) && process_num <= processes.size()){
            pair1 temp = processes[process_num];
            ready_queue.push_back(temp);
        }



        time++;
    }



    return outcome;
}

std::string SJF(std::vector<pair1 > processes){
    long waiting_job;
    bool cpu_free = true;
    long time = 0;
    int process_num = 0;
    std::vector<pair1> ready_queue;
    std::string outcome;
    waiting_job = processes.size();

    while (1){
        if(waiting_job == 0) {
            break;
        }

        // ??????????????????????????????
        //if(std::get<1>(processes[i]) <= 0){
        //    waiting_job --;
        //}

        if(time == std::get<0>(processes[process_num]) && process_num <= processes.size()){
            pair1 temp = processes[process_num];
            ready_queue.push_back(temp);
        }



        time++;
    }

    return outcome;
}




int main( int argc, char ** argv) {
    ifstream my_file;
    std::string line;
    vs_t words;

    std::vector< pair1 > shaA;
/*    std::string a  = std::string("a") + "b";
    pair1 k = std::make_tuple(1,2,"a");
    pair1 b = std::make_tuple(2,3,a);
    shaA.push_back(k);
    shaA.push_back(b);
    long c = std::get<0>(shaA[0]);    
    printf("%i \n", c);
    long d = std::get<0>(shaA[1]);    
    printf("%i \n", d);
    std::string e = std::get<2>(shaA[0]);
    std::string f = std::get<2>(shaA[1]);
    printf("%s \n", e.c_str());
    printf("%s \n", f.c_str());
*/
    long time_slices;

    if (argc < 3) {
        printf("At least 3 command line arguments expected\n ");
        exit(-1);
    }
    // add file_name to variable
    std::string file_name = argv[1];     // file name
    std::string Sche_al = argv[2];   // schedule algorithm

    //check if numbers of file_name match
    if ((Sche_al.compare("sjf") == 0 || Sche_al.compare("SJF")) == 0 && argc == 4) {
        printf("please don't add time slices to SJF\n");
        exit(-1);
    }

    //check if the file_name name is correct
    if (Sche_al.compare("sjf") != 0 && Sche_al.compare("SJF") != 0 && Sche_al.compare("rr") != 0 &&
        Sche_al.compare("RR") != 0) {
        printf("Scheduling algorithm not recognized. \n ");
        exit(-1);
    }



    // add time_slices to variable
    if (Sche_al.compare("RR") == 0 || Sche_al.compare("rr") == 0) {
        try {
            time_slices = std::stoll(argv[3]);
        }
        catch (std::exception &e) {
            printf("time slice for RR algorithm must be an integer\n");
            exit(-1);
        }
    }

    int p_num = 1;
    // read all the line and store the element into shaA
    my_file.open(file_name, ios::in);
    if(my_file.is_open()){
        while ( getline (my_file,line) )
        {
            // split string p_line into vector of strings (words)
            // the delimiters are 1 or more whitespaces
            words = split(line);
            std::string p_name  = std::string("P") + std::to_string(p_num);
            pair1 k = std::make_tuple(std::stoll(words[0]),std::stoll(words[1]),p_name);
            shaA.push_back(k);
            p_num++;

        }
        my_file.close();
    }
    else{
        printf("Couldn't find or open \"%s\"\n", file_name.c_str());
        exit(-1);
    }

    std::string outcome;
    if (Sche_al.compare("RR")==0||Sche_al.compare("rr")==0){
        time_slices = std::stoll(argv[3]);
        outcome = RoundRobin(shaA, time_slices);
    }
    else{
        outcome = SJF(shaA);
    }
    printf("%s\n",outcome.c_str());
    //printf("Seq: %s\n", outcome.substr(0,outcome.length()-2).c_str());
    return 0;

}
