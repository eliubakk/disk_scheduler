/*
This code was written by Erik Liubakka (eliubakk)
for EECS 482 Project 1 Winter 2018 at UofM
*/
#include "SchedulerT.h"
#include "DiskSchedulerShared.h"
#include "ServicerT.h"
#include "RequesterT.h"
#include <vector>
#include <iostream>

using namespace std;

namespace DiskScheduler{

	void SchedulerStart(void* args){
		((SchedulerStartArgs*)args)->t(((SchedulerStartArgs*)args)->args);
	}

	SchedulerT::SchedulerT(){}

	void SchedulerT::operator()(Commands commands){
		//print_mutex.lock();
		//cout << "SchedulerT started" << endl;
		//print_mutex.unlock();

		if(commands.argc < 3)
			return;
		unsigned int max_disk_queue = (unsigned int)atoi(commands.argv[1]);
		vector<RequesterStartArgs> requesters;
		vector<thread> rThreads;
		requesters.reserve(commands.argc - 2);
		rThreads.reserve(commands.argc - 2);
		for(unsigned int i = 2; i < commands.argc; ++i){
			requesters.emplace_back(i - 2, max_disk_queue, commands.argv[i]);
			disk_queue_mutex.lock();
			++requesters_alive;
			disk_queue_mutex.unlock();
			//print_mutex.lock();
			//cout << "Starting requester " + (i - 2) << endl;
			//print_mutex.unlock();
			rThreads.emplace_back(
				(thread_startfunc_t)RequesterStart,
				(void*)&requesters.back()
			);
		}
		ServicerStartArgs servicerArgs{ServicerT(), max_disk_queue};
		//print_mutex.lock();
		//cout << "Starting servicer thread" << endl;
		//print_mutex.unlock();
		thread sThread((thread_startfunc_t)ServicerStart, 
						(void*)&servicerArgs);
		disk_queue_mutex.lock();
		while(requesters_alive != 0){
			requester_finished.wait(disk_queue_mutex);
			if(requesters_alive < max_disk_queue
					&& disk_queue.size() == requesters_alive){
				queue_full_cv.signal();
			}	
		}
		queue_full_cv.broadcast();
		disk_queue_mutex.unlock();
		sThread.join();
	}
};