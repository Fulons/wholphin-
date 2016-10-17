#include "Context.h"
#include "Utilities.h"
#include <iostream>

#ifndef USE_GLFW_
namespace {
	wholphin::Context* g_Context = nullptr;
}

namespace wholphin {

	Context* Context::currentContext;


	LRESULT CALLBACK mainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		if (g_Context) return g_Context->MsgProc(hWnd, msg, wParam, lParam);
		else return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	Context::Context(HINSTANCE hInstance) {
		if (hInstance) hAppInstance = hInstance;
		else		   hAppInstance = GetModuleHandle(NULL);
		clientWidth = 800;
		clientHeight = 600;
		appTitle = "OPENGL EXAMPLE";
		windowStyle = WS_OVERLAPPED | WS_SIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
		g_Context = this;
		currentContext = this;
	}

	LRESULT Context::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)	{
		switch (msg) {
		case WM_SIZE: {
			clientWidth = LOWORD(lParam);
			clientHeight = HIWORD(lParam);
			RECT r = {};
			GetClientRect(hWnd, &r);
			int w = r.right - r.left;
			int h = r.bottom - r.top;
			Resize(w, h);
		}return 0;

		default: DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	bool Context::Init() {
		if (!InitWindow()) return false;
		if (!InitGL()) return false;
		return true;
	}

	int Context::Run() {
		__int64 prevTime{ 0 };
		QueryPerformanceCounter((LARGE_INTEGER*)&prevTime);
		__int64 countsPerSec{ 0 };
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
		float secondsPerCount = 1.0f / countsPerSec;
		MSG msg = {};
		while (msg.message != WM_QUIT) {		///Game Loop
			if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				__int64 curTime = 0;
				QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
				float deltaTime = (curTime - prevTime) * secondsPerCount;
				currentTime = curTime * secondsPerCount;

				Update(deltaTime);
				Render();
				CalculateFPS(deltaTime);
				SwapBuffers(hDeviceContext);
				prevTime = curTime;
				frameCount++;
			}
		}
		Shutdown();
		return static_cast<int>(msg.wParam);
	}

	bool Context::InitWindow() {
		WNDCLASSEX wcex = {};	//
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = mainWindowProc;
		//wcex.cbClsExtra;
		//wcex.cbWndExtra;
		wcex.hInstance = hAppInstance;
		wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		//wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = "CONTEXTWINDOWCLASS";
		wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wcex)) return OutErrorMessage("Failed to register wnd class");

		RECT r = { 0,0,(LONG)clientWidth, (LONG)clientHeight };
		AdjustWindowRect(&r, windowStyle, FALSE);

		int w = r.right - r.left;
		int h = r.bottom - r.top;
		int x = GetSystemMetrics(SM_CXSCREEN) / 2 - w / 2;
		int y = GetSystemMetrics(SM_CYSCREEN) / 2 - h / 2;

		hAppWnd = CreateWindow("CONTEXTWINDOWCLASS", appTitle, windowStyle, x, y, w, h, NULL, NULL, hAppInstance, NULL);
		if (!hAppWnd) return OutErrorMessage("Failed to create window");
		ShowWindow(hAppWnd, SW_SHOW);
		//Might have to initialise the console here
		return true;
	}

	bool Context::InitGL() {
		hDeviceContext = GetDC(hAppWnd);

		PIXELFORMATDESCRIPTOR pfd = {};
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		//pfd.cRedBits;
		//pfd.cRedShift;
		//pfd.cGreenBits;
		//pfd.cGreenShift;
		//pfd.cBlueBits;
		//pfd.cBlueShift;
		//pfd.cAlphaBits;
		//pfd.cAlphaShift;
		//pfd.cAccumBits;
		//pfd.cAccumRedBits;
		//pfd.cAccumGreenBits;
		//pfd.cAccumBlueBits;
		//pfd.cAccumAlphaBits;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		//pfd.cAuxBuffers;
		//pfd.iLayerType;
		//pfd.bReserved;
		//pfd.dwLayerMask;
		//pfd.dwVisibleMask;
		//pfd.dwDamageMask;

		int format = ChoosePixelFormat(hDeviceContext, &pfd);
		if (!SetPixelFormat(hDeviceContext, format, &pfd)) return OutErrorMessage("Failed to set pixel format");

		hGLRenderContext = wglCreateContext(hDeviceContext);
		if (!wglMakeCurrent(hDeviceContext, hGLRenderContext)) return OutErrorMessage("Failed to create and activate render context");

		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK) return OutErrorMessage((std::string("GLEW INIT ERROR:") + (char*)glewGetErrorString(err)).c_str());

		return true;
	}

	void Context::Shutdown() {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hGLRenderContext);
		ReleaseDC(hAppWnd, hDeviceContext);
	}

	void Context::CalculateFPS(float dt) {
		static float elapsed = 0;
		static int frameCount = 0;
		elapsed += dt;
		frameCount++;
		if(elapsed >= 1.0f){
			FPS = (float)frameCount;
			elapsed = 0.0f;
			frameCount = 0;
		}
	}

}	//namespace wolphin

#else



namespace wholphin {

	//This might be bad, when will glfw call this?
	void windowSizeCallback(GLFWwindow* window, int width, int height) {
		Context* context = (Context*)glfwGetWindowUserPointer(window);
		context->Resize(width, height);
	}	

	bool Context::Init() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, /*GLFW_OPENGL_CORE_PROFILE*/GLFW_OPENGL_COMPAT_PROFILE);
		window = glfwCreateWindow(800, 600, "OPENGL_TEST", nullptr, nullptr);
		if (!window) OutErrorMessage("Could not create glfw window");
		glfwMakeContextCurrent(window);
		CheckGLError();
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK) return OutErrorMessage((std::string("GLEW INIT ERROR:") + (char*)glewGetErrorString(err)).c_str());
		glfwSwapInterval(1);
		glfwSetWindowSizeCallback(window, windowSizeCallback);
		glfwSetWindowUserPointer(window, (void*)this);
		return true;
	}

	int Context::Run() {
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		Resize(w, h);

		double prevTime = glfwGetTime();
		double currentTime;

		while (!glfwWindowShouldClose(window)) {	

			currentTime = glfwGetTime();
			double deltaTime = currentTime - prevTime;

			Update((float)deltaTime);
			Render();
			glfwSwapBuffers(window);
			glfwPollEvents();
			prevTime = currentTime;
			frameCount++;
		}
		glfwTerminate();
		return 0;
	}
}

#endif