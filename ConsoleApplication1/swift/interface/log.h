#ifndef LOG_H
#define LOG_H
#include "Import.h"


#include "../../data_core/data_core.h"
#include "../../thrift_server/Thrift_server.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

void log(JSObject* object);

int add(int x, int y);

JSObject*  frame_log(JSON* parameter);
JSObject* frame_add(JSON* parameter);
JSObject* frame_add3(JSON* parameter);
#endif /* LOG_H */

