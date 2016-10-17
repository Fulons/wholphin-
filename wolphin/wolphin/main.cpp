
#define THIS_IS_THE_START_OF_OUR_GAME_GUYS 0x13333337
#include <iostream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <string>
#include "Context.h"
#include "Utilities.h"
#include "Grid.h"
#include "Shader.h"

class TestApplication : public wholphin::Context {
public:
	TestApplication() : wholphin::Context(), grid(glm::ivec2(20, 20)){}
	virtual bool Init() override;
	virtual bool Update(float dt) override;
	virtual bool Render() override;
	virtual void Resize(int w, int h) override;
private:
	GLuint VBO;
	GLuint IBO;
	GLuint VAO;
	wholphin::Shader shaderProg;

	glm::mat4 modelMatrix;	
	GLuint modelMatrixLocation;
	glm::mat4 viewMatrix;
	GLuint viewMatrixLocation;
	glm::mat4 projectionMatrix;
	GLuint projectionMatrixLocation;
	wholphin::Grid grid;
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
	glBindVertexArray(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	CheckGLError();
	grid.Init();
	return true;
}

bool TestApplication::Update(float dt) {
	modelMatrix = glm::rotate(modelMatrix, dt, glm::vec3(0.0f, 0.0f, 1.0f));
	return true;
}

bool TestApplication::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	shaderProg.Use();
	grid.Draw(modelMatrixLocation);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	return true;
}

void TestApplication::Resize(int w, int h){
	glViewport(0, 0, w, h);
	projectionMatrix = glm::ortho(-w/2.0f, w/2.0f, -h/2.0f, h/2.0f, -1000.0f, 1000.0f);
	//glm::perspective(glm::radians(45.0f), (float)w / h, 0.1f, 100.0f);
}

int main(char* argv[], int argc) {
	TestApplication test;
	test.Init();
	int result =  test.Run();
	system("PAUSE");
	return result;
}