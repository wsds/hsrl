#include "tools.h"

#include <android/log.h>
//#include <string>
//#include <map>

namespace hsrl {

	void test() {
		SpaceHolder* spaceHolder = new SpaceHolder();
		spaceHolder->test();
	}


#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "tools", __VA_ARGS__))

	int logCount = 0;
	void log_information(const char *text_format, int param) {

		char logCountText[200];
		//	sprintf(logCountText, "log 16:�?d�?---", logCount++);

		LOGI("log 15:Hello%d", logCount++);
		if (logCount == 10){
			test();
		}
	}

}
