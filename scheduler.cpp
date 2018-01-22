#include <iostream>
#include "thread.h"
#include "SchedulerT.h"

using namespace std;

int main(int argc, char* argv[])
{
	DiskScheduler::Commands commands{(unsigned int)argc, argv};
	DiskScheduler::SchedulerStartArgs args{DiskScheduler::SchedulerT(), commands};
    cpu::boot((thread_startfunc_t) DiskScheduler::SchedulerStart, (void *) &args, 0);
}