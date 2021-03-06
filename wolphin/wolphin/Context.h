#pragma once
//Test change
//#define WIN32_LEAN_AND_MEAN
#include <GL\glew.h>

#ifndef USE_GLFW_
#include <Windows.h>
#endif

#include "InputHandler.h"

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
		virtual void Resize(int w, int h) = 0;

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
		float currentTime;

	private:
		bool InitWindow();
		bool InitGL();
		void Shutdown();

	private:
		void CalculateFPS(float dt);

		static Context* currentContext;

	protected:
		unsigned frameCount = 0;
		InputHandler inputHandler;
	};
}
#else

#include <GLFW\glfw3.h>
namespace wholphin {
	class Context {
	public:
		virtual bool Init();
		int Run();
		virtual bool Update(float dt) = 0;
		virtual bool Render() = 0;
		virtual void Resize(int w, int h) = 0;
		unsigned GetClientWidth() { return clientWidth; }
		unsigned GetClientHeight() { return clientHeight; }
	private:
		GLFWwindow* window;
	protected:
		unsigned clientWidth;
		unsigned clientHeight;
		unsigned frameCount = 0;
		InputHandler inputHandler;
		friend void windowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend void windowSizeCallback(GLFWwindow* window, int width, int height);
	};
}
#endif