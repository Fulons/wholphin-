#pragma once
#include <GL\glew.h>

namespace wholphin {

	class Shader {
	public: 
		void Init(const char* vsPath, const char* fsPath);
		void Use() { glUseProgram(programID); }
		GLuint GetLocation(const char* name) { return glGetUniformLocation(programID, name); }
	private:
		GLuint programID;
	};

	
}