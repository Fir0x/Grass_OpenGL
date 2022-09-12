#pragma once

#if _DEBUG
	#define ASSERT(x) if (!(x)) __debugbreak();
	#define GL_CALL(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x, __FILE__, __LINE__));
	#define GL_AFTER_CHECK() GLClearError();\
		ASSERT(GLLogCall(__FILE__, __LINE__));;
#else
	#define ASSERT(x) x;
	#define GL_CALL(x) x;
	#define GL_AFTER_CHECK();
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
bool GLLogCall(const char* file, int line);