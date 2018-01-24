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
			rThreads.emplace_back(
				(thread_startfunc_t)RequesterStart,
				(void*)&requesters.back()
			);
		}
		ServicerStartArgs servicerArgs{ServicerT(), max_disk_queue};
		thread sThread((thread_startfunc_t)ServicerStart, 
						(void*)&servicerArgs);
		disk_queue_mutex.lock();
		while(requesters_alive != 0){
			requester_finished.wait(disk_queue_mutex);
			if(disk_queue.size() == requesters_alive ||
			   disk_queue.size() == max_disk_queue) {
				queue_full_cv.signal();
			}	
		}
		queue_full_cv.broadcast();
		disk_queue_mutex.unlock();
		sThread.join();
	}
};