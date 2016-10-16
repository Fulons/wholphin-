#pragma once
#include <glm\glm.hpp>
#include <vector>
#include <GL\glew.h>

namespace wholphin {

	struct Texture {
		int width;
		int height;
		GLuint ID;
	};

	Texture LoadTexture(const char* file);

	class Tile {
	public:
		Tile(short t, glm::vec2 p) : type(t), pos(p){}
		Tile(){}
		short type;
		glm::vec2 pos;
	};

	class Grid {
	public:
		Grid(glm::ivec2 size) : size(size){}
		void Init();
		void Draw(GLuint modelMatrixIndex);
	private:
		glm::ivec2 size;
		std::vector<Tile> tiles;
		std::vector<glm::mat4> modelMatrix;
	private:
		Texture textureMap;
		std::vector<glm::ivec2> texturePos;
		GLuint VBO;
		GLuint CBO;
		GLuint IBO;
		GLuint TBO;
		GLuint VAO;
	};

}