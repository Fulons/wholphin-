
#define THIS_IS_THE_START_OF_OUR_GAME_GUYS 0x13333337
#include <iostream>
#include <glm\glm.hpp>
#include <fstream>
#include <sstream>
#include "Context.h"


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

class Shader {
public: 
	void Init(const char* vsPath, const char* fsPath);
	void Use() { glUseProgram(programID); }

private:
	GLuint programID;
};

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

class TestApplication : public wholphin::Context {
public:
	TestApplication() : wholphin::Context(){}
	virtual bool Init();
	virtual bool Update(float dt);
	virtual bool Render();

private:
	GLuint VBO;
	GLuint IBO;
	GLuint VAO;
	Shader shaderProg;
};

struct Vertex {
	glm::vec2 pos;
	glm::vec3 colour;
};

bool TestApplication::Init() {
	Context::Init();
	shaderProg.Init("Shaders\\vs.glsl", "Shaders\\fs.glsl");
	shaderProg.Use();
	Vertex vertices[] = {
		Vertex{ glm::vec2(-0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f) },
		Vertex{ glm::vec2(+0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f) },
		Vertex{ glm::vec2(+0.5f, +0.5f), glm::vec3(0.0f, 0.0f, 1.0f) },
		Vertex{ glm::vec2(-0.5f, +0.5f), glm::vec3(0.0f, 1.0f, 1.0f) }
	};

	GLuint indices[] = { 0, 1, 2,
						 0, 2, 3};

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (GLvoid*)vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), (GLvoid*)indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(glm::vec2));


	return true;
}

bool TestApplication::Update(float dt) {


	return true;
}

bool TestApplication::Render() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	return true;
}

int main(char* argv[], int argc) {
	TestApplication test;
	test.Init();
	int result =  test.Run();
	system("PAUSE");
	return result;
}


