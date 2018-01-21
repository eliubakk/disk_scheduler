/*
This code was written by Erik Liubakka (eliubakk)
for EECS 482 Project 1 Winter 2018 at UofM
*/
#include "RequesterT.h"
#include "disk.h"
#include <fstream>

using namespace std;

namespace DiskScheduler{

		RequesterT::RequesterT(unsigned int _id, unsigned int _max_disk_queue):
			id(_id), max_disk_queue(_max_disk_queue){}

		void RequesterT::start(void* file){
			read_requests(*((string*)file));

			while(!requests.empty()){
				//pop next request
				Request* current_request = requests.front(); 
				requests.pop();

				//try to put request into queue
				disk_queue_mutex.lock();
				while(disk_queue.size() == max_disk_queue 
					|| (requesters_alive < max_disk_queue
						&& disk_queue.size() == requesters_alive)){
					//signal servicer that queue is full
					queue_full_cv.signal();
					queue_not_full_cv.wait(disk_queue_mutex);
				}
				disk_queue.push_back(current_request);
				print_mutex.lock();
				print_request(id, current_request->track);
				print_mutex.unlock();
				disk_queue_mutex.unlock();

				//wait until request is handled
				current_request->mut.lock();
				while(!current_request->handled){
					serviced.wait(current_request->mut);
				}
				delete current_request;
			}

			//TODO: Signal that thread is dead...
			requester_finished.broadcast();
		}

		void RequesterT::read_requests(string f_name){
			ifstream disk = ifstream(f_name);
			unsigned short track;
			while(disk >> track){
				Request* request = new Request{id, track, mutex(), &serviced, false};
				/*request->requester_id = id;
				request->track = track;
				request->serviced = &serviced;
				request->handled = false;*/
				requests.push(request);
			}
			disk.close();
		}
}