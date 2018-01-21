/*
This code was written by Erik Liubakka (eliubakk)
for EECS 482 Project 1 Winter 2018 at UofM
*/
#ifndef DISK_SCHEDULER_H
#define DISK_SCHEDULER_H

#include "DiskSchedulerShared.h"
#include "ServicerT.h"
#include "RequesterT.h"

namespace DiskScheduler{
	class DiskScheduler{
		public:
			DiskScheduler();
			start(void* _commands);
	}
}

#endif