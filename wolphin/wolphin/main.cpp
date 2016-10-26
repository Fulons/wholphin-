
#define THIS_IS_THE_START_OF_OUR_GAME_GUYS 0x13333337
#include <iostream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <string>
#include "Context.h"
#include "Utilities.h"
#include "Grid.h"
#include "Shader.h"
#include <GL\wglew.h>

class TestApplication : public wholphin::Context {
public:
	TestApplication() : wholphin::Context(), grid(glm::ivec2(100, 100)){}
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
	wholphin::Shader multiTextureProg;

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

	GLuint multiTextureModelMatrixLocation;
	GLuint multiTextureViewMatrixLocation;
	GLuint multiTextureProjectionMatrixLocation;
	GLuint multiTexture1Location;
	GLuint multiTexture2Location;
	GLuint multiTexture3Location;
	GLuint multiTexture4Location;
	GLuint multiTexture5Location;

	glm::vec3 lookAtCenter;
	float zoom = 1.0f;
	int funkyness = 1;
};

struct Vertex {
	glm::vec2 pos;
	glm::vec3 colour;
};

bool TestApplication::Init() {
	Context::Init();
	modelMatrix = glm::mat4(100.0f);
	modelMatrix[3][3] = 1.0f;
	modelMatrix = glm::translate(modelMatrix, glm::vec3(30.0f, 0.0f, -1.0f));
	glm::vec3 tiltVector;
	//tiltVector = glm::normalize(glm::vec3(-1.0f, -1.0f, 0.0f));
	//tiltVector.z = 1.0f;
	tiltVector = glm::vec3(-1.0f, -1.0f, 1.0f) * zoom + lookAtCenter;
	viewMatrix = glm::lookAt(tiltVector, lookAtCenter,  glm::vec3(1.0f, 1.0f, 0.0f));
	shaderProg.Init("Shaders\\tilevs.glsl", "Shaders\\tilefs.glsl");
	//shaderProg.Use();
	modelMatrixLocation = shaderProg.GetLocation("m");
	viewMatrixLocation = shaderProg.GetLocation("v");
	projectionMatrixLocation = shaderProg.GetLocation("p");
	entityProg.Init("Shaders\\entityvs.glsl", "Shaders\\entityfs.glsl");
	//entityProg.Use();
	entityModelMatrixLocation = entityProg.GetLocation("m");
	entityViewMatrixLocation = entityProg.GetLocation("v");
	entityProjectionMatrixLocation = entityProg.GetLocation("p");
	multiTextureProg.Init("Shaders\\multitexturevs.glsl", "Shaders\\multitexturefs.glsl");
	//multiTextureProg.Use();
	multiTextureModelMatrixLocation = multiTextureProg.GetLocation("m");
	multiTextureViewMatrixLocation = multiTextureProg.GetLocation("v");
	multiTextureProjectionMatrixLocation = multiTextureProg.GetLocation("p");
	multiTexture1Location = multiTextureProg.GetLocation("tex1");
	multiTexture2Location = multiTextureProg.GetLocation("tex2");
	multiTexture3Location = multiTextureProg.GetLocation("tex3");
	multiTexture4Location = multiTextureProg.GetLocation("tex4");
	multiTexture5Location = multiTextureProg.GetLocation("textureMap");

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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	wglSwapIntervalEXT(1);

	return true;
}

bool TestApplication::Update(float dt) {
	//modelMatrix = glm::rotate(modelMatrix, dt, glm::vec3(0.0f, 0.0f, 1.0f));
	const static float speed = 1000.0f;
	bool zoomThisUpdate = false;
	static bool wireframe = false;
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
	if (inputHandler.WentDownAtFrame(VK_SPACE, frameCount)) {
		wireframe = !wireframe;
		if (wireframe) glPolygonMode(GL_FRONT, GL_LINE);
		else glPolygonMode(GL_FRONT, GL_FILL);
	}
	if (zoomThisUpdate) {
		int w = GetClientWidth();
		int h = GetClientHeight();
		projectionMatrix = glm::ortho(-w/2.0f * zoom, w/2.0f * zoom, -h/2.0f * zoom, h/2.0f * zoom, -10000.0f, 10000.0f);
	}

	grid.Update(frameCount / 200.0f, funkyness);

	glm::vec3 tiltVector;
	tiltVector = glm::vec3(-1.0f, -1.0f, 1.0f) * zoom + lookAtCenter;
	viewMatrix = glm::lookAt(tiltVector, lookAtCenter,  glm::vec3(1.0f, 1.0f, 0.0f));
	return true;
}

bool TestApplication::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	multiTextureProg.Use();
	//glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(multiTextureProjectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(multiTextureViewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniform1i(multiTexture1Location, 0);
	glUniform1i(multiTexture2Location, 1);
	glUniform1i(multiTexture3Location, 2);
	glUniform1i(multiTexture4Location, 3);
	glUniform1i(multiTexture5Location, 4);
	grid.Draw(multiTextureModelMatrixLocation);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	entityProg.Use();
	glUniformMatrix4fv(entityProjectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(entityViewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	//glUniformMatrix4fv(entityModelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	grid.DrawEntities(entityModelMatrixLocation);
	CheckGLError();
	return true;
}

void TestApplication::Resize(int w, int h){
	glViewport(0, 0, w, h);
	projectionMatrix = glm::ortho(-w/2.0f * zoom, w/2.0f * zoom, -h/2.0f * zoom, h/2.0f * zoom, -10000.0f, 10000.0f);
	//glm::vec3 tiltVector;
	//tiltVector = glm::vec3(-1.0f, -1.0f, 1.0f) * zoom + lookAtCenter;
	//viewMatrix = glm::lookAt(tiltVector, lookAtCenter,  glm::vec3(1.0f, 1.0f, 0.0f));

}

int main(char* argv[], int argc) {
	TestApplication test;
	test.Init();
	int result =  test.Run();
	return result;
}