#include "tools.h"

#include <android/log.h>

#include "./../MainEngine.h"

//#include <string>
//#include <map>

namespace hsrl {


	class A{
	private:
		int w, h;
		int m;
	public:
		static int area;//静态数据成员的定义形式
		int area1;
		void setVal(int x)
		{
			area = x;
		}
		int getArea()
		{
			return area;
		}
		A(int x, int y)
		{
			w = x, h = y;
		}
		A(){}
	};
	int A::area = 0;
	void test2(){
		A const a(3, 4);
		A c(2, 6);
		A f;
		c.area1 = 4;
		c.setVal(4);
		c.area = 8;//初始化后可以改变其值，不初始化程序编译都通不过
		f.area = 16;
	}
	//int MainEngine::test1 = 100;
	void test() {
		SpaceHolder* spaceHolder = new SpaceHolder();
		spaceHolder->test();
		/*	MainEngine* m = new MainEngine();
			m->test1 = 1;*/
		//MainEngine m();
		//m.test1 = 1; 
		//hsrl::MainEngine* mMainEngine = hsrl::MainEngine::getInstance();
		MainEngine::test1 = 120;


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
