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
std::string RoundRobin(std::vector<long long> arrival_time, std::vector<long long> burst_length, long long burst ){
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
		exit(-1);
	}
	
	long long time_quantum;

	if ( scheduler.compare("RR")==0 || scheduler.compare("rr")==0 ) {
		try{
			time_quantum = std::stoll(argv[3]);
		}
		catch(std::exception& e){
			printf("Must specify time slice as an integer for RR algorithm\n");
			exit(-1);
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
		printf("Couldn't find or open \"%s\"\n", config.c_str());
		exit(-1);
	}

	
	//Now we have all the arrival times and corresponding burst lengths.
	std::string result;
	if (scheduler.compare("RR")==0||scheduler.compare("rr")==0){
		time_quantum = std::stoll(argv[3]);
		result = RoundRobin(arrival_time, burst_length, time_quantum);
	}
	else{
		result = SJF(arrival_time, burst_length);
	}

	printf("Seq: %s\n", result.substr(0,result.length()-2).c_str());
	return 0;

}