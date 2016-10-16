#include "Utilities.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace wholphin {

	std::string ReadFile(const char* path, bool* good)
	{
		std::ifstream file(path);
		if (!file.is_open()) {
			std::string str("Could not find file: ");
			str += path;
			OutErrorMessage(str.c_str());
			if (good) *good = false;
			return std::string();
		}

		std::stringstream ret;
		ret << file.rdbuf();
		file.close();
		if (good) *good = true;
		return ret.str();
	}

	bool _OutErrorMessage(const char* errorMessage, int line, char* file) {
		if (file) {
			if (line)	std::cout << "ERROR in: " << file << " @ line" << line << "\n";
			else			std::cout << "ERROR in: " << file << "\n";		
		}
		else if (line) std::cout << "ERROR @ line:" << line << "\n";
		std::cout << errorMessage << std::endl;
		//MessageBox(nullptr, )
		return false;
	}

}