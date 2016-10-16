
#define THIS_IS_THE_START_OF_OUR_GAME_GUYS 0x13333337
#include <iostream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
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
	GLuint GetLocation(const char* name) { return glGetUniformLocation(programID, name); }
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
	virtual bool Init() override;
	virtual bool Update(float dt) override;
	virtual bool Render() override;
	virtual void Resize(int w, int h) override;
private:
	GLuint VBO;
	GLuint IBO;
	GLuint VAO;
	Shader shaderProg;

	glm::mat4 modelMatrix;	
	GLuint modelMatrixLocation;
	glm::mat4 viewMatrix;
	GLuint viewMatrixLocation;
	glm::mat4 projectionMatrix;
	GLuint projectionMatrixLocation;
};

struct Vertex {
	glm::vec2 pos;
	glm::vec3 colour;
};

bool TestApplication::Init() {
	Context::Init();
	modelMatrix = glm::mat4(100.0f);
	modelMatrix[3][3] = 1.0f;
	glm::vec3 tiltVector;
	//tiltVector = glm::normalize(glm::vec3(-1.0f, -1.0f, 0.0f));
	//tiltVector.z = 1.0f;
	tiltVector = glm::vec3(1.0f, 1.0f, 1.0f);
	viewMatrix = glm::lookAt(tiltVector, glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 1.0f, 0.0f));
	shaderProg.Init("Shaders\\vs.glsl", "Shaders\\fs.glsl");
	shaderProg.Use();
	modelMatrixLocation = shaderProg.GetLocation("m");
	viewMatrixLocation = shaderProg.GetLocation("v");
	projectionMatrixLocation = shaderProg.GetLocation("p");
	Vertex vertices[] = {
		Vertex{ glm::vec2(-0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f) },
		Vertex{ glm::vec2(+0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f) },
		Vertex{ glm::vec2(+0.5f, +0.5f), glm::vec3(0.0f, 0.0f, 1.0f) },
		Vertex{ glm::vec2(-0.5f, +0.5f), glm::vec3(0.0f, 1.0f, 1.0f) }
	};

	GLuint indices[] = { 0, 1, 2,
						 0, 2, 3};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
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


	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	return true;
}

bool TestApplication::Update(float dt) {
	modelMatrix = glm::rotate(modelMatrix, dt, glm::vec3(0.0f, 0.0f, 1.0f));
	return true;
}

bool TestApplication::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	shaderProg.Use();
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	return true;
}

void TestApplication::Resize(int w, int h){
	glViewport(0, 0, w, h);
	projectionMatrix = glm::ortho(-w/2.0f, w/2.0f, -h/2.0f, h/2.0f, -100.0f, 100.0f);
	//glm::perspective(glm::radians(45.0f), (float)w / h, 0.1f, 100.0f);
}

int main(char* argv[], int argc) {
	TestApplication test;
	test.Init();
	int result =  test.Run();
	system("PAUSE");
	return result;
}