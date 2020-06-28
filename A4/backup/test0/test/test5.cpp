#include <stdio.h>
#include <ctype.h>
#include <vector>
#include <string>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cinttypes>
#include <cmath>
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
using namespace std;

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
std::string RoundRobin(std::vector<long long> arrival_time, std::vector<long long> burst_length, long long burst ){
	long long curr_time = 0;
	long long burst_counter =0;
	long long jobs_remaining = arrival_time.size();
	long long job_counter = 0;
	long long job=0;;
	bool cpu_idle = true;
	std::vector<long long> executed_bursts (burst_length.size(), 0);
	std::queue<long long> RQ;
	std::string result = "";
	std::string lastProcess;
	

	while(1){
		//printf("Curr time = %lld\n", curr_time);
		//printf("executed bursts on job %lld = %d\n", job, executed_bursts[job]);
		//printf("Job on CPU: %d\n", job);
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
			burst_length[job] = -1;
			
			continue;
		}
		
		std::vector<long long>::iterator it = std::find(arrival_time.begin(), arrival_time.end(), curr_time);
	 
		if( it != arrival_time.end() ){
			
			arrival_time[job_counter] = -1;
			RQ.push(job_counter);
			job_counter++;
		}

		if (cpu_idle && !RQ.empty()){
			
			//Move job from RQ onto CPU
			job = RQ.front();
			RQ.pop();
			cpu_idle = false;\
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
			//Redo this check
			std::string s1(1, result.at(result.length()-3));

			if (lastProcess.compare("P"+to_string(job)) != 0){ 
				result = result +"P"+to_string(job)+", ";
				lastProcess = "P"+to_string(job);
			}
			 
			continue;
		}

		//Need to figure out how to increase what we increment curr_time by
		curr_time++;
		
		if(!cpu_idle){
			burst_counter++;
			executed_bursts[job]++;
		}
	}
	return result;
}

int main( int argc, char ** argv){
	if (argc<3){
		printf("Expected at least 3 command line arguments\n ");
		exit(-1);
	}
	std::string config = argv[1];
	std::string scheduler = argv[2];

	if (scheduler.compare("sjf")!=0  && scheduler.compare("SJF")!=0 && scheduler.compare("rr")!=0 && scheduler.compare("RR")!=0){
		printf("Scheduling algorithm not recognized. \n ");
		exit(-1);
	}
 
	if ((scheduler.compare("sjf") == 0 || scheduler.compare("SJF")) == 0 && argc == 4){
		printf("SJF does not accept time slices\n");
	}
	
	long long time_quantum;

	if ( scheduler.compare("RR")==0 || scheduler.compare("rr")==0 ) {
		try{
			time_quantum = std::stoll(argv[3]);
		}
		catch(std::exception& e){
			printf("Must specify time slice as an integer for RR algorithm\n");
		}
	}

	ifstream myfile;
	std::string line;
	vs_t words;

	std::vector<long long> arrival_time;
	std::vector<long long> burst_length;

	myfile.open(config, ios::in);
	if(myfile.is_open()){ 
		while ( getline (myfile,line) )
		{
			words = split(line);
			arrival_time.push_back(std::stoll(words[0]));
			burst_length.push_back(std::stoll(words[1]));

		}
		myfile.close();
	}
	else{
		printf("Couldn't open %s\n", config.c_str());
	}

	//Now we have all the arrival times and corresponding burst lengths.
	//Do the loop
	std::string result;
	if (scheduler.compare("RR")==0||scheduler.compare("rr")==0){
		time_quantum = std::stoll(argv[3]);
		result = RoundRobin(arrival_time, burst_length, time_quantum);
	}
	else{
		//result = SJF();
	}

	printf("Seq: %s\n", result.c_str());

	return 0;

}