#pragma once
#include <string>

namespace wholphin {

	std::string ReadFile(const char* path, bool* good);
	bool _OutErrorMessage(const char* errorMessage, int line = 0, char* file = nullptr);
#define OutErrorMessage(errorMessage) _OutErrorMessage((errorMessage), __LINE__, __FILE__)
}