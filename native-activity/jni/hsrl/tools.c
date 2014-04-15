#include "tools.h"

#include <android/log.h>
#include<string.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

int logCount = 0;
void log_information(const char *text_format, const char *param) {

	char logCountText[200];
	sprintf(logCountText, "log:【%d】----", logCount++);

	LOGI(strcat(logCountText, text_format), param);
}
