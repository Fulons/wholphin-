#include "Shader.h"
#include "Utilities.h"
#include <iostream>

namespace wholphin {

	void Shader::Init(const char * vsPath, const char * fsPath){
		bool good = false;
		std::string vs = ReadFile(vsPath, &good);
		if (!good) std::cout << "Could not read vertex shader: " << vsPath << std::endl;
		std::string fs = ReadFile(fsPath, &good);
		if (!good) std::cout << "Could not read fragment shader: " << fsPath << std::endl;

		GLint success;

		const GLchar* vsFile = vs.c_str();
		GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vsID, 1, &vsFile, nullptr);
		glCompileShader(vsID);
		glGetShaderiv(vsID, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar infoLog[512];
			glGetShaderInfoLog(vsID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILES\n" << infoLog << std::endl;
			glDeleteShader(vsID);
			return;
		}

		const GLchar* fsFile = fs.c_str();
		GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fsID, 1, &fsFile, nullptr);
		glCompileShader(fsID);
		glGetShaderiv(fsID, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar infoLog[512];
			glGetShaderInfoLog(fsID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILES\n" << infoLog << std::endl;
			glDeleteShader(vsID);
			glDeleteShader(fsID);
			return;
		}

		programID = glCreateProgram();
		glAttachShader(programID, fsID);
		glAttachShader(programID, vsID);
		glLinkProgram(programID);
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar infoLog[512];
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			glDetachShader(programID, fsID);
			glDetachShader(programID, vsID);
			glDeleteShader(vsID);
			glDeleteShader(fsID);
			glDeleteProgram(programID);
			programID = 0;
			return;
		}
		glDeleteShader(vsID);
		glDeleteShader(fsID);
	}

}