/*
This code was written by Erik Liubakka (eliubakk)
for EECS 482 Project 1 Winter 2018 at UofM
*/
#include "ServicerT.h"
#include "disk.h"
#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace std;

namespace DiskScheduler{
	void ServicerStart(void* args){
		((ServicerStartArgs*)args)->t(((ServicerStartArgs*)args)->max_disk_queue);
	}

	ServicerT::ServicerT() : curr_track(0) {}

	void ServicerT::operator()(unsigned int max_disk_queue){

		while(true){
			disk_queue_mutex.lock();
			while(!(disk_queue.size() == requesters_alive ||
					disk_queue.size() == max_disk_queue)){
				//wait until queue is full
				queue_full_cv.wait(disk_queue_mutex);
			}
			//thread exit condition
			if(requesters_alive == 0)
				return;

			//get request off queue
			Request* request = closest_request();
			queue_not_full_cv.signal();

			//service request
			request->mut.lock();
			curr_track = request->track;
			request->handled = true;
			print_mutex.lock();
			print_service(request->requester_id, request->track);
			print_mutex.unlock();
			request->serviced->signal();
			request->mut.unlock();
			disk_queue_mutex.unlock();
		}
	}

	Request* ServicerT::closest_request(){
		//assume first is closest
		auto closest = disk_queue.begin();
		(*closest)->mut.lock();

		//compare to each request
		for(auto it = disk_queue.begin(); it != disk_queue.end(); ++it){
			if(closest == it){
				continue;
			}
			(*it)->mut.lock();

			//check if it is closer than closest
			if(abs((signed int)(*it)->track - (signed int)curr_track) <
			   abs((signed int)(*closest)->track - (signed int)curr_track)){
				//unlock closest, update
				(*closest)->mut.unlock();
				closest = it;
			}
			else{
				(*it)->mut.unlock();
			}
		}
		Request* next = (*closest);
		disk_queue.erase(closest);
		next->mut.unlock();
		return next;
	}
}