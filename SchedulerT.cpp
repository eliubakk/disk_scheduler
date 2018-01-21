/*
This code was written by Erik Liubakka (eliubakk)
for EECS 482 Project 1 Winter 2018 at UofM
*/
#include "SchedulerT.h"
#include "DiskSchedulerShared.h"
#include "ServicerT.h"
#include "RequesterT.h"
#include <vector>

using namespace std;

namespace DiskScheduler{
	SchedulerT::SchedulerT(){}

	void SchedulerT::operator()(void* _commands){
		Commands commands = *((Commands*)_commands);
		if(commands.argc < 3)
			return;
		unsigned int max_disk_queue = (unsigned int)atoi(commands.argv[1]);
		vector<RequesterT> requesters;
		vector<thread> rThreads;
		requesters.reserve(commands.argc - 2);
		rThreads.reserve(commands.argc - 2);
		for(unsigned int i = 2; i < commands.argc; ++i){
			requesters.emplace_back(i - 2, max_disk_queue);
			disk_queue_mutex.lock();
			++requesters_alive;
			disk_queue_mutex.unlock();
			rThreads.emplace_back(
				(thread_startfunc_t)&(requesters.back()),
				(void*)commands.argv[i]
			);
		}
		ServicerT servicer;
		thread sThread((thread_startfunc_t)&servicer, 
						(void*)&max_disk_queue);
		disk_queue_mutex.lock();
		while(requesters_alive != 0){
			requester_finished.wait(disk_queue_mutex);
		}
		queue_full_cv.broadcast();
		disk_queue_mutex.unlock();
		sThread.join();
	}
};