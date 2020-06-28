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
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cinttypes>
#include <cmath>
#include <queue>

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
    std::vector<long long> arrival_time; //-----------
    for (int i = 0; i < processes.size() ; ++i) {
        arrival_time.push_back(std::get<0>(processes[i]));
    }

    std::vector<long long> burst_length;
    for (int i = 0; i < processes.size() ; ++i) {
        burst_length.push_back(std::get<1>(processes[i]));
    }

    long long burst = time_slices;
    long long curr_time = 0;
    long long burst_counter =0;
    long long jobs_remaining = arrival_time.size();
    long long job_counter = -1;
    long long job=-1;
    long long flag = 0;
    bool cpu_idle = true;
    std::vector<long long> executed_bursts (burst_length.size(), 0);
    std::queue<long long> RQ;
    std::string result = "";
    std::string lastProcess;

    while(1){
        if (jobs_remaining == 0) break;

        if (cpu_idle && jobs_remaining != 0 && RQ.empty()){
            if (lastProcess.compare("-, ")== 0){}
            else{
                result = result +"-, ";
                lastProcess = "-, ";
            }
        }

        if (!cpu_idle && executed_bursts[job] == burst_length[job]){
            //Job is done executing;
            jobs_remaining -= 1;
            cpu_idle = true;
            burst_counter = 0;
            //burst_length[job] = -1;
            continue;
        }

        if (cpu_idle && !RQ.empty()){

            //Move job from RQ onto CPU
            job = RQ.front();
            RQ.pop();
            cpu_idle = false;
            result = result +"P"+to_string(job)+", ";
            lastProcess = "P"+to_string(job);
            continue;
        }

        if (burst_counter == burst){
            //time slice expired for current job, put next job in RQ onto CPU.
            //printf("job %d time slice expired\n", job);
            RQ.push(job);
            job = RQ.front();
            RQ.pop();
            burst_counter = 0;
            if (lastProcess.compare("P"+to_string(job)) != 0){
                result = result +"P"+to_string(job)+", ";
                lastProcess = "P"+to_string(job);
            }

            continue;
        }

        std::vector<long long>::iterator it = std::find(arrival_time.begin(), arrival_time.end(), curr_time);

        if( it != arrival_time.end() && flag == 0 ){
            job_counter++;
            //arrival_time[job_counter] = -1;
            RQ.push(job_counter);
            flag = 1;
            continue;
        }

        //Might implement calculating and executing multiple rounds at a time

        long long time_to_add;
        if (job_counter!= (long long) arrival_time.size()-1){
            //We havent received every job
            if (arrival_time[job_counter+1]-curr_time < burst-burst_counter){

                //The next job arrives BEFORE this round ends
                if(arrival_time[job_counter+1]-curr_time< burst_length[job]-executed_bursts[job]){//The next job arrives before this job finishes
                    time_to_add = arrival_time[job_counter+1]-curr_time;

                }

                else //The current Job finishes before the next job arrives
                    time_to_add = burst_length[job]-executed_bursts[job];

                curr_time+= time_to_add;

            }
            else{

                //The next job arrives AFTER this round ends
                if (burst_length[job]- executed_bursts[job]<burst-burst_counter) //Current job ends before round ends
                    time_to_add = burst_length[job]- executed_bursts[job];
                else //Rounds ends before job ends and before next job arrives
                    time_to_add = burst-burst_counter;

                curr_time += time_to_add;
            }
        }
        else{
            //We've recieved every job
            if (burst_length[job]-executed_bursts[job] < burst-burst_counter) //Current job ends before round ends
                time_to_add = burst_length[job]-executed_bursts[job];
            else  //rounds ends before current job ends
                time_to_add = burst-burst_counter;
            curr_time+= time_to_add;
        }
        if (!cpu_idle){
            if(time_to_add <= 0){
                executed_bursts[job]++;
                burst_counter++;
            }
            else{
                executed_bursts[job] += time_to_add;
                burst_counter += time_to_add;
            }
        }

        if (time_to_add <= 0){
            if(job == -1){
                curr_time = arrival_time[0];
            }
            else
                curr_time++;
        }
        flag = 0;
    }
    return result;
}

bool sortcol( const std::tuple< long, long , std::string > & v1, const std::tuple< long, long , std::string > & v2 ) {
    return get<1>(v1) < get<1>(v2);
}

std::string SJF(std::vector<pair1 > processes){
    long waiting_job;
    bool cpu_free = true;
    bool process_change = false;
    long time = 0;
    int process_num = 0;
    long time_count = 0;
    std::vector<pair1> ready_queue;
    std::string outcome = std::string("");
    waiting_job = processes.size();

    while (1){
        if (waiting_job == 0) {
            break;
        }
        //std::vector<long>::iterator it = std::find(std::get<0>(processes.begin), std::get<0>(processes.end), time);

        printf("%i\n", waiting_job);

        //printf("%i\n", time);

        printf("ready queue size %i",ready_queue.size());
        if(ready_queue.size() == 0){
            process_change = false;
        }

        //时间到了，把任务放进去
        if (time == std::get<0>(processes[process_num]) && process_num <= processes.size()) {
            printf("loop 1\n");
            pair1 temp = processes[process_num];
            ready_queue.push_back(temp);
            process_num++;
        }

        //如果没有任何任务，cpu空置
        printf("ready queue size %i",ready_queue.size());
        if(ready_queue.size() == 1 && !process_change && cpu_free){
            process_change = true;
            printf("loop 2\n");
            outcome += "-, ";
        }

        //有任务 任务开始执行
        if (ready_queue.size() > 0) {
            printf("loop 3\n");
            cpu_free = false;
            std::get<1>(ready_queue[0]) --;
        }



        //有任务 检测任务是否完成 ------如果ready_queue的第一个元素已经消耗完成
        if (ready_queue.size() > 0){
            printf("loop 4\n");
            printf("jindu %i",std::get<1>(ready_queue[0]));
            if (std::get<1>(ready_queue[0]) == 0) {
                printf("loop 5\n");
                cpu_free = true;
                std::string p_temp = std::get<2>(ready_queue[0]);
                outcome = outcome + p_temp + ", ";
                ready_queue.erase(ready_queue.begin());
                waiting_job--;
                if(ready_queue.size() > 1){
                    sort(ready_queue.begin(), ready_queue.end(), sortcol);
                }
            }
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

    int p_num = 0;
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