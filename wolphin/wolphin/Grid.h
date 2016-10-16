#pragma once
#include <glm\glm.hpp>
#include <vector>
#include <GL\glew.h>

namespace wholphin {

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
		GLuint VBO;
		GLuint CBO;
		GLuint IBO;
		GLuint VAO;
	};

}