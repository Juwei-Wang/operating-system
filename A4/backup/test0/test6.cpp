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

using namespace std;

typedef std::vector<std::string> vs_t;
typedef std::tuple< long long, long long , long long > pair1;

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



std::string RoundRobin(std::vector<pair1 > processes, long long time_slices ){
    long long waiting_job;
    bool cpu_free = true;
    bool process_change = false;
    long long time = 0;
    int process_num = 0;
    long long time_count = 0;
    long long changed_time = 0;
    std::vector<pair1> ready_queue;
    std::vector<pair1> job_finished;
    std::string outcome = std::string("");
    waiting_job = processes.size();
    long long cpu_busy_waiting_time;
    long long cpu_busy_incoming_time;
    long long queue_add = false;
    long long temp_change = 0;
    bool job_done = false;


    while (1) {
        if (waiting_job == 0) {
            break;
        }
        //printf("\n");
        //printf("\n");
        //std::vector<pair1>::iterator it = std::find(processes.start(), processes.end(), time);
        // ??????????????????????????????
        //if(std::get<1>(processes[i]) <= 0){
        //    waiting_job --;
        //  1 9 P1      0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
        //  3 5 P2       -
        //  5 3 P3       RQ P1
        //printf("%i\n", waiting_job);

        //printf("%i\n", time);

        //printf("ready queue size %i",ready_queue.size());
        if(ready_queue.size() == 0){
            //printf("node add \n");
            process_change = false;
        }

        //时间到了，把任务放进去
        if (time == std::get<0>(processes[process_num]) && process_num <= processes.size() && queue_add) {
            //printf("loop 1\n");
            pair1 temp = processes[process_num];
            ready_queue.push_back(temp);
            process_num++;
        }

        //如果没有任何任务，cpu空置
        //printf("ready queue size %i",ready_queue.size());
        if(ready_queue.size() == 1 && !process_change && cpu_free){
            process_change = true;
            //printf("loop 2\n");
            outcome += "-, ";
        }

        //有任务 任务开始执行
        if(temp_change > 0){
            changed_time = changed_time + temp_change;
        }

        printf("%llu ,\n", changed_time);
        if (ready_queue.size() > 0 && !queue_add) {
            //printf("loop 3\n");
            cpu_free = false;
            std::get<1>(ready_queue[0]) -= changed_time;
            time_count += changed_time;;
            temp_change = 0;
        }

        //有任务 检测任务是否完成 ------如果ready_queue的第一个元素已经消耗完成
        if (ready_queue.size() > 0){
            //printf("loop 4\n");
            printf("jindu %llu \n",std::get<1>(ready_queue[0]));
            if (std::get<1>(ready_queue[0]) <= 0) {
                //printf("loop 5\n");
                cpu_free = true;
                long long p_temp = std::get<2>(ready_queue[0]);
                outcome = outcome + "P" + std::to_string(p_temp) + ", ";
                job_finished.push_back(ready_queue[0]);
                ready_queue.erase(ready_queue.begin());
                waiting_job--;
                time_count = 0;
                job_done = true;
            }
        }

        if(time_count == time_slices){
            pair1 temp = ready_queue[0];
            long long p_temp = std::get<2>(ready_queue[0]);
            if (ready_queue.size() > 1) {
                outcome = outcome + "P" + std::to_string(p_temp) + ", ";
            }
            ready_queue.erase(ready_queue.begin());
            ready_queue.push_back(temp);
            time_count = 0;
        }

        queue_add = false;
        long long cpu_free_waiting_time;
        long long test_time = time;
        printf("%llu , % llu \n", time , job_finished.size());
        printf("%llu , % llu \n", time , ready_queue.size());
        printf("--------------------------------------------\n");
        if(cpu_free && ready_queue.size() == 0){
            if(job_finished.size() == 0){
                //printf("loop1\n");
                cpu_free_waiting_time = std::get<0>(processes[0]);
                time = cpu_free_waiting_time;
                printf("I am here %llu \n",  cpu_free_waiting_time);
            }
            if(job_finished.size() > 0){
                //printf("loop2\n");
                cpu_free_waiting_time = std::get<0>(processes[job_finished.size()]);
                time = cpu_free_waiting_time;
                printf("I am here%llu \n",  cpu_free_waiting_time);
            }
            changed_time = time - test_time;
            queue_add = true;
            if(ready_queue.size() == 0  &&  job_finished.size() >= 0){
                changed_time = 0;
            }
        }


        
        if((!cpu_free && ready_queue.size() > 0) || (job_done && ready_queue.size() > 0)){
            //printf("I get into this loop\n");
            //printf("time_slices %llu \n",  time_slices);
            printf("current p work %llu \n", std::get<1>(ready_queue[0]));
            if(std::get<1>(ready_queue[0]) < time_slices){
                printf("loop1\n");
                if (cpu_busy_waiting_time <= cpu_busy_incoming_time) {
                    cpu_busy_waiting_time = time + std::get<1>(ready_queue[0]);
                    printf("cpu busy time %llu \n", cpu_busy_waiting_time);
                }
            } else{
                printf("loop2 -- ------- \n");
                if (cpu_busy_waiting_time <= cpu_busy_incoming_time) {
                    cpu_busy_waiting_time = time + time_slices;
                    printf("cpu busy time %llu \n", cpu_busy_waiting_time);
                }
            }
            printf("change_time1 %llu ,\n", changed_time);
            if(job_finished.size() + ready_queue.size() <= processes.size()){
                printf("loop3\n");
                if (job_finished.size() + ready_queue.size() < processes.size()){
                    cpu_busy_incoming_time = std::get<0>(processes[job_finished.size() + ready_queue.size()]);
                }
                else{
                    cpu_busy_incoming_time = 9223372036854775807;
                }
                printf("cpu incoming time %llu \n",  cpu_busy_incoming_time);
                printf("cpu busy waiting time %llu \n",  cpu_busy_waiting_time);
                if(cpu_busy_waiting_time > cpu_busy_incoming_time){
                   time = cpu_busy_incoming_time;
                   queue_add = true;
                } else{
                    time = cpu_busy_waiting_time;
                }
                changed_time = time - test_time;
                if(queue_add){
                    temp_change = changed_time;
                }
                printf("change_time2 %llu ,\n", changed_time);
            }
            job_done = false;
        }


/*        //现在开始计算应该增长的时间

        flag = 0; */
        //time ++;
    }
    return outcome;
}

bool sortcol( const std::tuple< long long, long long , long long > & v1, const std::tuple< long long, long long , long long > & v2 ) {
    return get<1>(v1) < get<1>(v2);
}

std::string SJF(std::vector<pair1 > processes){
    long long waiting_job;
    bool cpu_free = true;
    bool process_change = false;
    long long time = 0;
    int process_num = 0;
    long long time_count = 0;
    std::vector<pair1> ready_queue;
    std::string outcome = std::string("");
    waiting_job = processes.size();

    while (1){
        if (waiting_job == 0) {
            break;
        }
        //std::vector<long long>::iterator it = std::find(std::get<0>(processes.begin), std::get<0>(processes.end), time);

        //printf("%i\n", waiting_job);

        //printf("%i\n", time);

        //printf("ready queue size %i",ready_queue.size());
        if(ready_queue.size() == 0){
            process_change = false;
        }

        //时间到了，把任务放进去
        if (time == std::get<0>(processes[process_num]) && process_num <= processes.size()) {
            //printf("loop 1\n");
            pair1 temp = processes[process_num];
            ready_queue.push_back(temp);
            process_num++;
        }

        //如果没有任何任务，cpu空置
        //printf("ready queue size %i",ready_queue.size());
        if(ready_queue.size() == 1 && !process_change && cpu_free){
            process_change = true;
            //printf("loop 2\n");
            outcome += "-, ";
        }

        //有任务 任务开始执行
        if (ready_queue.size() > 0) {
            //printf("loop 3\n");
            cpu_free = false;
            std::get<1>(ready_queue[0]) --;
        }


        //有任务 检测任务是否完成 ------如果ready_queue的第一个元素已经消耗完成
        if (ready_queue.size() > 0){
            //printf("loop 4\n");
            //printf("jindu %i",std::get<1>(ready_queue[0]));
            if (std::get<1>(ready_queue[0]) == 0) {
                //printf("loop 5\n");
                cpu_free = true;
                long long p_temp = std::get<2>(ready_queue[0]);
                outcome = outcome + "P" + std::to_string(p_temp) + ", ";
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
    long long c = std::get<0>(shaA[0]);
    printf("%i \n", c);
    long long d = std::get<0>(shaA[1]);
    printf("%i \n", d);
    std::string e = std::--------------------------------------------(shaA[0]);
    std::string f = std::--------------------------------------------(shaA[1]);
    printf("%s \n", e.c_str());
    printf("%s \n", f.c_str());
*/
    long long time_slices;

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

    long long p_num = 0;
    // read all the line and store the element into shaA
    my_file.open(file_name, ios::in);
    if(my_file.is_open()){
        while ( getline (my_file,line) )
        {
            // split string p_line into vector of strings (words)
            // the delimiters are 1 or more whitespaces
            words = split(line);
            std::string p_name  = std::string("P") + std::to_string(p_num);
            pair1 k = std::make_tuple(std::stoll(words[0]),std::stoll(words[1]),p_num);
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
    printf("seq: %s\n",outcome.c_str());
    //printf("Seq: %s\n", outcome.substr(0,outcome.length()-2).c_str());
    return 0;

}
