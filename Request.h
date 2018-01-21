/*
This code was written by Erik Liubakka (eliubakk)
for EECS 482 Project 1 Winter 2018 at UofM
*/
#ifndef REQUEST_H
#define REQUEST_H

#include "thread.h"

struct Request{
	unsigned int requester_id;
	unsigned short track;
	mutex mut; 
	cv* serviced;
	bool handled;
};

#endif