#ifndef MAIN_OPENGL
#define MAIN_OPENGL

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
namespace hsrl {
	static void printGLString(const char *name, GLenum s);

	static void checkGlError(const char* op);

	GLuint loadShader(GLenum shaderType, const char* pSource);

	GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

	bool setupGraphics(int w, int h);

	void test1();

	void renderFrame();
}

#endif // MAIN_OPENGL