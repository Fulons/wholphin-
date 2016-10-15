#pragma once
//#define WIN32_LEAN_AND_MEAN
#include <GL\glew.h>

#ifndef USE_GLFW_
#include <Windows.h>
#endif

bool _OutErrorMessage(const char* errorMessage, int line = 0, char* file = nullptr);
#define OutErrorMessage(errorMessage) _OutErrorMessage((errorMessage), __LINE__, __FILE__)

#ifndef USE_GLFW_
namespace wholphin {

	class Context {
	public:
		Context(HINSTANCE hInstance = nullptr);
		virtual ~Context() {}
		virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		virtual bool Init();
		virtual bool Update(float dt) = 0;
		virtual bool Render() = 0;

		int Run();

		UINT GetClientWidth() { return clientWidth; }
		UINT GetClientHeight() { return clientHeight; }

		static Context* GetContext() { return currentContext; }


	private:
		HWND hAppWnd = nullptr;
		HINSTANCE hAppInstance = nullptr;
		HDC hDeviceContext = nullptr;
		HGLRC hGLRenderContext = nullptr;
		UINT clientWidth;
		UINT clientHeight;
		DWORD windowStyle;
		char* appTitle;
		float FPS;
		unsigned frameCount = 0;
		float currentTime;

	private:
		bool InitWindow();
		bool InitGL();
		void Shutdown();

	private:
		void CalculateFPS(float dt);

		static Context* currentContext;
	};
}
#else

#include <GLFW\glfw3.h>
namespace wholphin {
	class Context {
	public:
		virtual bool Init() {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			window = glfwCreateWindow(800, 600, "OPENGL_TEST", nullptr, nullptr);
			glfwMakeContextCurrent(window);
			glewInit();
			glfwSwapInterval(1);
			return true;
		}
		int Run() {
			while (!glfwWindowShouldClose(window)) {
				Update(0);
				Render();
				glfwSwapBuffers(window);
				glfwPollEvents();
			}
			return 0;
		}
		virtual bool Update(float dt) = 0;
		virtual bool Render() = 0;
	private:

		GLFWwindow* window;
	};
}
#endif