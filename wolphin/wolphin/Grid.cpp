#include "Grid.h"
#include <glm\gtc\matrix_transform.hpp>
#include <stdlib.h>
#include <soil\SOIL.h>
#include "Utilities.h"
#include "Noise.h"

namespace wholphin {

	

	Texture LoadTexture(const char * file){
		Texture ret;
		unsigned char* image = SOIL_load_image(file, &ret.width, &ret.height, 0, SOIL_LOAD_RGB);
		if (!image) {
			OutErrorMessage((std::string("Could not load texture:") + file).c_str());
			return Texture();
		}
		glGenTextures(1, &ret.ID);
		glBindTexture(GL_TEXTURE_2D, ret.ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ret.width, ret.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		return ret;
	}

	glm::vec3 tileColours[] = {
		glm::vec3(1.0f, 0.0f, 0.0f), //red, duh
		glm::vec3(1.0f, 0.0f, 0.0f), //red, duh
		glm::vec3(1.0f, 0.0f, 0.0f), //red, duh
		glm::vec3(1.0f, 0.0f, 0.0f), //red, duh
		glm::vec3(0.0f, 1.0f, 0.0f), //green, duh
		glm::vec3(0.0f, 1.0f, 0.0f), //green, duh
		glm::vec3(0.0f, 1.0f, 0.0f), //green, duh
		glm::vec3(0.0f, 1.0f, 0.0f), //green, duh
		glm::vec3(0.0f, 0.0f, 1.0f), //blue, duh
		glm::vec3(0.0f, 0.0f, 1.0f), //blue, duh
		glm::vec3(0.0f, 0.0f, 1.0f), //blue, duh
		glm::vec3(0.0f, 0.0f, 1.0f), //blue, duh
		glm::vec3(1.0f, 1.0f, 0.0f), //yellow
		glm::vec3(1.0f, 1.0f, 0.0f), //yellow
		glm::vec3(1.0f, 1.0f, 0.0f), //yellow
		glm::vec3(1.0f, 1.0f, 0.0f), //yellow
		glm::vec3(1.0f, 0.0f, 1.0f), //purple
		glm::vec3(1.0f, 0.0f, 1.0f), //purple
		glm::vec3(1.0f, 0.0f, 1.0f), //purple
		glm::vec3(1.0f, 0.0f, 1.0f), //purple
		glm::vec3(0.0f, 1.0f, 1.0f), //cyan
		glm::vec3(0.0f, 1.0f, 1.0f), //cyan
		glm::vec3(0.0f, 1.0f, 1.0f), //cyan
		glm::vec3(0.0f, 1.0f, 1.0f), //cyan
		glm::vec3(1.0f, 1.0f, 1.0f), //white
		glm::vec3(1.0f, 1.0f, 1.0f), //white
		glm::vec3(1.0f, 1.0f, 1.0f), //white
		glm::vec3(1.0f, 1.0f, 1.0f), //white
		glm::vec3(0.0f, 0.0f, 0.0f), //black
		glm::vec3(0.0f, 0.0f, 0.0f), //black
		glm::vec3(0.0f, 0.0f, 0.0f), //black
		glm::vec3(0.0f, 0.0f, 0.0f), //black
		glm::vec3(0.5f, 0.5f, 0.5f), //grey
		glm::vec3(0.5f, 0.5f, 0.5f), //grey
		glm::vec3(0.5f, 0.5f, 0.5f), //grey
		glm::vec3(0.5f, 0.5f, 0.5f), //grey
		glm::vec3(1.0f, 0.5f, 0.0f), //orange
		glm::vec3(1.0f, 0.5f, 0.0f), //orange
		glm::vec3(1.0f, 0.5f, 0.0f), //orange
		glm::vec3(1.0f, 0.5f, 0.0f), //orange
		glm::vec3(0.5f, 1.0f, 0.0f), //greenish yellow
		glm::vec3(0.5f, 1.0f, 0.0f), //greenish yellow
		glm::vec3(0.5f, 1.0f, 0.0f), //greenish yellow
		glm::vec3(0.5f, 1.0f, 0.0f), //greenish yellow
		glm::vec3(0.5f, 1.0f, 0.5f), //light green
		glm::vec3(0.5f, 1.0f, 0.5f), //light green
		glm::vec3(0.5f, 1.0f, 0.5f), //light green
		glm::vec3(0.5f, 1.0f, 0.5f), //light green
		glm::vec3(0.0f, 0.5f, 0.0f), //dark green
		glm::vec3(0.0f, 0.5f, 0.0f), //dark green
		glm::vec3(0.0f, 0.5f, 0.0f), //dark green
		glm::vec3(0.0f, 0.5f, 0.0f), //dark green
	};

	

	struct Vertex {
		glm::vec2 pos;
	};

	void Grid::Init() {
		std::vector<glm::vec2> UVs;
		UVs.resize(4 * 4 * 4);
		for (unsigned x = 0; x < 4; x++) {
			for (unsigned y = 0; y < 4; y++) {
				UVs[(x + (y * 4)) * 4 + 0] = glm::vec2(x / 4.0f,		  y / 4.0f);
				UVs[(x + (y * 4)) * 4 + 1] = glm::vec2((x + 1) / 4.0f,  y / 4.0f);
				UVs[(x + (y * 4)) * 4 + 2] = glm::vec2((x + 1) / 4.0f, (y + 1) / 4.0f);
				UVs[(x + (y * 4)) * 4 + 3] = glm::vec2(x / 4.0f,		 (y + 1) / 4.0f);
			}
		}
		for (unsigned x = 0; x < 4; x++) {
			for (unsigned y = 0; y < 4; y++) {
				UVs[(x + (y * 4)) * 4 + 0] = glm::vec2(x / 4.0f,		  y / 4.0f);
				UVs[(x + (y * 4)) * 4 + 1] = glm::vec2((x + 1) / 4.0f,  y / 4.0f);
				UVs[(x + (y * 4)) * 4 + 2] = glm::vec2((x + 1) / 4.0f, (y + 1) / 4.0f);
				UVs[(x + (y * 4)) * 4 + 3] = glm::vec2(x / 4.0f,		 (y + 1) / 4.0f);
			}
		}

		textureMap = LoadTexture("tileMap.png");
		modelMatrix.resize(size.x * size.y);
		tiles.resize(size.x * size.y);
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {
				glm::vec2 perlinPos = glm::vec2(x, y) / (glm::vec2)size;
				float pn = Perlin(perlinPos);
				int tileType = (int)((pn + 1) * 6);
				tiles[x + (y * size.x)] = Tile(tileType, glm::vec2(x - size.x / 2, y - size.y / 2));
				modelMatrix[x + (y * size.x)] = glm::scale(glm::mat4(), glm::vec3(50.0f, 50.0f, 1.0f)) * glm::translate(glm::mat4(), glm::vec3(tiles[x + (y * size.x)].pos, 0.0f));
			}
		}

		Vertex vertices[] = {
			glm::vec2(-0.5f, -0.5f),
			glm::vec2(+0.5f, -0.5f),
			glm::vec2(+0.5f, +0.5f),
			glm::vec2(-0.5f, +0.5f)
		};

		GLuint indices[] = { 0, 1, 2,
			0, 2, 3};

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &IBO);
		glGenBuffers(1, &CBO);
		glGenBuffers(1, &TBO);


		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, CBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tileColours), tileColours, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, TBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * UVs.size(), UVs.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		CheckGLError();
	}

	void Grid::Draw(GLuint modelMatrixIndex) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureMap.ID);
		for (unsigned i = 0; i < tiles.size(); i++) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

			glBindBuffer(GL_ARRAY_BUFFER, CBO);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(glm::vec3) * tiles[i].type * 4));

			glBindBuffer(GL_ARRAY_BUFFER, TBO);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(glm::vec2) * tiles[i].type * 4));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

			glUniformMatrix4fv(modelMatrixIndex, 1, GL_FALSE, &modelMatrix[i][0][0]);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
		}
	}

	

}