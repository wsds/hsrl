#ifndef ShellIface_H
#define ShellIface_H
extern "C"{
	namespace shell{
		//Must implement it to receive the shell
		void shell(const char*);

		//Call it the log output to the client
		void log(const char*);
	}
}
#endif
