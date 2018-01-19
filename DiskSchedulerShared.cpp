#include "DiskSchedulerShared.h"

using namespace std;

namespace DiskScheduler{
	vector<Request> requests;
	mutex requests_mutex;
	cv queue_full_cv;
	cv queue_not_full_cv;
}