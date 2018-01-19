#include "RequesterT.h"
#include <fstream>

namespace DiskScheduler{

		RequesterT::RequesterT(int _id, int _max_disk_queue):
			id(_id), max_disk_queue(_max_disk_queue), requests(0){}

		void RequesterT::start(void* file){
			read_requests(*((string*)file));

			while(!requests.empty()){
				//pop next request
				Request* current_request = requests.pop();

				//try to put request into queue
				disk_queue_mutex.lock();
				while(disk_queue.size() == max_disk_queue 
					|| (requesters_alive < max_disk_queue
						&& disk_queue.size() == requesters_alive)){
					//signal servicer that queue is full
					queue_not_full_cv.signal();
					queue_full_cv.wait(disk_queue_mutex);
				}
				disk_queue.push_back(current_request);
				print_request(id, current_request->track);
				disk_queue_mutex.unlock();

				//wait until request is handled
				current_request->mut.lock();
				while(!current_request->handled){
					serviced.wait(current_request->mut)
				}
				delete current_request;
			}

			//TODO: Signal that thread is dead...
		}

		void RequesterT::read_requests(string file);
}