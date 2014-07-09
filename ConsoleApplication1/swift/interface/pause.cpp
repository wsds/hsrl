#include "pause.h"

std::mutex mutex_pause;
std::condition_variable condition_variable_pause;

void pause(){
	std::unique_lock<std::mutex> ul(mutex_pause);
	condition_variable_pause.wait(ul);
}

void continueRun(){
	condition_variable_pause.notify_all();
}


JSON* frame_pause(JSON* parameter){
	pause();
	return NULL;
}
