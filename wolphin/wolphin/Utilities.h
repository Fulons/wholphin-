#pragma once
#include <string>

namespace wholphin {

	std::string ReadFile(const char* path, bool* good);
	bool _OutErrorMessage(const char* errorMessage, int line = 0, char* file = nullptr);
	void _CheckGLError(int line, const char* file);
}
#define OutErrorMessage(errorMessage) wholphin::_OutErrorMessage((errorMessage), __LINE__, __FILE__)
#define CheckGLError() wholphin::_CheckGLError(__LINE__, __FILE__)