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

	void RequesterStart(void* args);

	class RequesterT{
		public:
			RequesterT(unsigned int id, unsigned int max_disk_queue);

			void operator()(std::string file);

		private:
			unsigned int id;
			unsigned int max_disk_queue;
			cv serviced;
			std::queue<Request*> requests;

			void read_requests(std::string f_name);
	};

	struct RequesterStartArgs{
		RequesterT t;
		std::string file;

		RequesterStartArgs(unsigned int _id, unsigned int _max_disk_queue, std::string _file);
	};
}


#endif