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

//The program is held together by the coding equivalent of ducktape and wacky glue
//Please bear with me on this as I've not coded in c++ before this class.

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




//
std::string RoundRobin(std::vector<pair1 > processes, long time_slices){
    std::vector<long long> arrival_time; //-----------
    for (int i = 0; i < processes.size() ; ++i) {
        print("%s \n", std::get<0>(processes[i]);
        arrival_time.push_back(std::get<0>(processes[i]));
    }

    std::vector<long long> burst_length;
    for (int i = 0; i < processes.size() ; ++i) {
        print("%s \n", std::get<0>(processes[i]);
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


long long
optimalSJFinc(long long nextArrival, long long remBursts){
    if (remBursts<=0 && nextArrival<=0)
        return 1;
    if(remBursts<=0)
        return nextArrival;
    if(nextArrival<=0)
        return remBursts;
    if (nextArrival < remBursts && nextArrival != 0){
        return nextArrival;
    }
    else{
        return remBursts;
    }
}

std::string
SJF(std::vector<long long>arrival_time, std::vector<long long> burst_length){
    long long curr_time = 0;
    long long jobs_remaining = arrival_time.size();
    long long job_counter = -1;
    long long job=-1;
    bool cpu_idle = true;
    std::priority_queue<std::pair<long long, int>, std::vector<std::pair<long long, int>>, greater<std::pair<long long, int>>> RQ;
    std::vector<long long> executed_bursts (burst_length.size(), 0);
    std::string result = "";
    std::string lastProcess;

    while(1){

        if (jobs_remaining == 0) break;

        std::vector<long long>::iterator it = std::find(arrival_time.begin(), arrival_time.end(), curr_time);

        if( it != arrival_time.end() ){
            //New job arrives
            job_counter++;
            arrival_time[job_counter] = -1;
            RQ.push({burst_length[job_counter], job_counter});

        }

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
            burst_length[job] = -1;
            continue;
        }

        if (cpu_idle && !RQ.empty()){
            //Move job from RQ onto CPU
            job = RQ.top().second;
            RQ.pop();
            cpu_idle = false;
            result = result +"P"+to_string(job)+", ";
            lastProcess = "P"+to_string(job);
            continue;
        }

        long long prev_time = curr_time;

        if (job_counter+1<(long long)arrival_time.size())
            curr_time+= optimalSJFinc(arrival_time[job_counter+1]-curr_time, burst_length[job]-executed_bursts[job]);
        else
            curr_time+= 1;//this is a special case for the last job, in which curr_time is not applicable and no longer matters.


        if(!cpu_idle){
            if (job_counter+1<(long long)arrival_time.size())
                executed_bursts[job]+=optimalSJFinc(arrival_time[job_counter+1]-prev_time, burst_length[job]-executed_bursts[job]);
            else
                executed_bursts[job]=burst_length[job];
        }

    }

    return result;
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
    printf("Seq: %s\n", outcome.substr(0,outcome.length()-2).c_str());
    return 0;

}