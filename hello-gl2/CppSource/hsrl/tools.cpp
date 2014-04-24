#include "tools.h"

#include <android/log.h>
//#include <string>
//#include <map>

namespace hsrl {

void test() {
	SpaceHolder* spaceHolder = new SpaceHolder();
}


#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

int logCount = 0;
void log_information(const char *text_format,  int param) {

	char logCountText[200];
//	sprintf(logCountText, "log 16:【%d】----", logCount++);

	LOGI("log 16:【%d】----", param);
	if(logCount==50){
		test();
	}
}

}
