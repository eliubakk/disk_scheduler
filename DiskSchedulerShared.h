/*
This code was written by Erik Liubakka (eliubakk)
for EECS 482 Project 1 Winter 2018 at UofM
*/
#ifndef DISK_SCHEDULER_SHARED_H
#define DISK_SCHEDULER_SHARED_H

#include <vector>
#include "Request.h"
#include "thread.h"

namespace DiskScheduler{
	extern std::vector<Request*> disk_queue;
	extern mutex disk_queue_mutex;
	extern cv queue_full_cv;
	extern cv queue_not_full_cv;
	extern int requesters_alive;
	extern mutex requesters_alive_mutex;
	extern cv requester_finished;
}
#endif