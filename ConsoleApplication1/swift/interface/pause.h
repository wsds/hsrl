#ifndef PAUSE_H
#define PAUSE_H

#include <thread>
#include <condition_variable>
#include "../../data_core/JSON.h"

void pause();
void continueRun();

JSON* frame_pause(JSON* parameter);

#endif /* PAUSE_H */

