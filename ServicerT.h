/*
This code was written by Erik Liubakka (eliubakk)
for EECS 482 Project 1 Winter 2018 at UofM
*/
#ifndef SERVICER_T_H
#define SERVICER_T_H

#include "DiskSchedulerShared.h"
#include "Request.h"

namespace DiskScheduler{
	class ServicerT{
		public:
			ServicerT();

			void start(void* _commands);

		private:
			unsigned int curr_track;
			Request* closest_request();

	};
}

#endif