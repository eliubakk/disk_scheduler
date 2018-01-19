/*
This code was written by Erik Liubakka (eliubakk)
for EECS 482 Project 1 Winter 2018 at UofM
*/
#ifndef REQUESTER_T_H
#define REQUESTER_T_H

#include <string>
#include <queue>
#include "thread.h"
#include "Request.h"
#include "DiskSchedulerShared.h"

namespace DiskScheduler{
	class RequesterT{
	public:
		RequesterT(int id, int max_disk_queue);

		void start(void* file);

	private:
		int id;
		int max_disk_queue;
		cv serviced;
		std::queue<Request> requests;

		void read_requests(string file);
	};
}


#endif