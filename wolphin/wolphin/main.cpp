
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
	TestApplication() : wholphin::Context(), grid(glm::ivec2(200, 200)){}
	virtual bool Init() override;
	virtual bool Update(float dt) override;
	virtual bool Render() override;
	virtual void Resize(int w, int h) override;
private:
	GLuint VBO;
	GLuint IBO;
	GLuint VAO;
	wholphin::Shader shaderProg;
	wholphin::Shader entityProg;

	glm::mat4 modelMatrix;	
	GLuint modelMatrixLocation;
	glm::mat4 viewMatrix;
	GLuint viewMatrixLocation;
	glm::mat4 projectionMatrix;
	GLuint projectionMatrixLocation;
	wholphin::Grid grid;

	GLuint entityModelMatrixLocation;
	GLuint entityViewMatrixLocation;
	GLuint entityProjectionMatrixLocation;

	glm::vec3 lookAtCenter;
	float zoom = 1.0f;
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
	tiltVector = glm::vec3(1.0f, 1.0f, 1.0f) * zoom + lookAtCenter;
	viewMatrix = glm::lookAt(tiltVector, lookAtCenter,  glm::vec3(1.0f, 1.0f, 0.0f));
	shaderProg.Init("Shaders\\tilevs.glsl", "Shaders\\tilefs.glsl");
	shaderProg.Use();
	modelMatrixLocation = shaderProg.GetLocation("m");
	viewMatrixLocation = shaderProg.GetLocation("v");
	projectionMatrixLocation = shaderProg.GetLocation("p");
	entityProg.Init("Shaders\\entityvs.glsl", "Shaders\\entityfs.glsl");
	entityProg.Use();
	entityModelMatrixLocation = entityProg.GetLocation("m");
	entityViewMatrixLocation = entityProg.GetLocation("v");
	entityProjectionMatrixLocation = entityProg.GetLocation("p");
	CheckGLError();
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

	glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	return true;
}

bool TestApplication::Update(float dt) {
	modelMatrix = glm::rotate(modelMatrix, dt, glm::vec3(0.0f, 0.0f, 1.0f));
	const static float speed = 1000.0f;
	bool zoomThisUpdate = false;
	if (inputHandler.IsDown('A')) {
		lookAtCenter += glm::vec3(-1.0f, 1.0f, 0.0f) * dt * speed;
	}
	if (inputHandler.IsDown('D')) {
		lookAtCenter += glm::vec3(1.0f, -1.0f, 0.0f) * dt * speed;
	}
	if (inputHandler.IsDown('W')) {
		lookAtCenter += glm::vec3(1.0f, 1.0f, 0.0f) * dt * speed;
	}
	if (inputHandler.IsDown('S')) {
		lookAtCenter += glm::vec3(-1.0f, -1.0f, 0.0f) * dt * speed;
	}
	if (inputHandler.IsDown('Q')) {
		zoom += dt * 10;
		zoomThisUpdate = true;
	}
	if (inputHandler.IsDown('E')) {
		zoom -= dt * 10;
		if (zoom <= 0) zoom = 0.1f;
		zoomThisUpdate = true;
	}
	if (zoomThisUpdate) {
		int w = GetClientWidth();
		int h = GetClientHeight();
		projectionMatrix = glm::ortho(-w/2.0f * zoom, w/2.0f * zoom, -h/2.0f * zoom, h/2.0f * zoom, -10000.0f, 10000.0f);
	}

	grid.Update(frameCount);

	glm::vec3 tiltVector;
	tiltVector = glm::vec3(1.0f, 1.0f, 1.0f) * zoom + lookAtCenter;
	viewMatrix = glm::lookAt(tiltVector, lookAtCenter,  glm::vec3(1.0f, 1.0f, 0.0f));
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

	entityProg.Use();
	glUniformMatrix4fv(entityProjectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(entityViewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	grid.DrawEntities(entityModelMatrixLocation);
	CheckGLError();
	return true;
}

void TestApplication::Resize(int w, int h){
	glViewport(0, 0, w, h);
	projectionMatrix = glm::ortho(-w/2.0f * zoom, w/2.0f * zoom, -h/2.0f * zoom, h/2.0f * zoom, -10000.0f, 10000.0f);
	//glm::perspective(glm::radians(45.0f), (float)w / h, 0.1f, 100.0f);
}

int main(char* argv[], int argc) {
	TestApplication test;
	test.Init();
	int result =  test.Run();
	return result;
}