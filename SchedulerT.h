/*
This code was written by Erik Liubakka (eliubakk)
for EECS 482 Project 1 Winter 2018 at UofM
*/
#ifndef SCHEDULER_T_H
#define SCHEDULER_T_H

namespace DiskScheduler{
	struct Commands{
		unsigned int argc;
		char** argv;
	};

	void SchedulerStart(void* args);

	class SchedulerT{
		public:
			SchedulerT();
			void operator()(Commands commands);
	};

	struct SchedulerStartArgs{
		SchedulerT t;
		Commands args;
	};
}

#endif