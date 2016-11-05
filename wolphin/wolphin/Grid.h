#pragma once
#include <glm\glm.hpp>
#include <vector>
#include <GL\glew.h>

#include "RenderData.h"

namespace wholphin {

	struct Renderable {
		glm::mat4 where;
		MeshData* what;
	};	

	class Tile {
	public:
		Tile(short t, glm::vec2 p) : type(t), pos(p){}
		Tile(){}
		short type;
		glm::vec2 pos;
	};

	struct MapEntity {
		MeshData* what;
		float mapValueStart;
		float mapValueEnd;
		float probability;
	};

	class Grid {
	public:
		Grid(glm::ivec2 size) : size(size) { textureMixTextureData.resize(textureMixTextureSize.x * textureMixTextureSize.y); }
		void Init();
		void Init(std::vector<MapEntity>* mapEntities, std::vector<Texture>* tileTextures);
		void Update(float frameNumberFraction, int funkyness);
		void Draw(GLuint modelMatrixIndex);
		void Draw2(GLuint modelMatrixIndex);
		void DrawEntities(GLuint modelMatrixIndex);
	private:
		glm::ivec2 size;
		std::vector<Tile> tiles;
		std::vector<glm::mat4> modelMatrix;

		std::vector<MapEntity> mapEntities;
		std::vector<Texture> tileTextures;

		void InitBuffers();
		void Populate();
	private:
		MeshData palmMesh;
		MeshData bushMesh[6];
		
		Texture textureMap;
		Texture textureMixTexture;
		union {
			struct {
				Texture desGround00;
				Texture desGround01;
				Texture desGround02;
				Texture desGround03;
			};
			Texture desTextures[4];
		};
		union {
			struct {
				Texture desShrub00;
				Texture desShrub01;
				Texture desShrub02;
				Texture desShrub03;
				Texture desShrub04;
			};
			Texture desShruberyTextures[5];
		};

		std::vector<unsigned char> textureMixTextureData;
		glm::ivec2 textureMixTextureSize = glm::ivec2(2048, 2048);

		std::vector<glm::ivec2> texturePos;
		std::vector<Renderable> renderables;
		GLuint VBO;
		GLuint CBO;
		GLuint IBO;
		GLuint TBO;
		GLuint TBO2;
		GLuint VAO;
	};

}