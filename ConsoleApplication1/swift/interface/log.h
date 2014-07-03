#ifndef LOG_H
#define LOG_H
#include "Import.h"


#include "../../data_core/data_core.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

void log(JSObject* object);

int add(int x, int y);

JSON*  frame_log(JSON* parameter);
JSON* frame_add(JSON* parameter);
#endif /* LOG_H */

