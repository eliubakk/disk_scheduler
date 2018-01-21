/*
This code was written by Erik Liubakka (eliubakk)
for EECS 482 Project 1 Winter 2018 at UofM
*/
#include "DiskSchedulerShared.h"
#include <vector>
#include "Request.h"
#include "thread.h"

using namespace std;

namespace DiskScheduler{
	vector<Request*> disk_queue;
	mutex disk_queue_mutex;
	cv queue_full_cv;
	cv queue_not_full_cv;
	unsigned int requesters_alive;
	cv requester_finished;
	mutex print_mutex;
}