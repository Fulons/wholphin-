#pragma once
#include <array>
#include <vector>
#include <unordered_map>
#include "RenderData.h"
#define CHUNK_SIZE 16
#define TILE_TYPE_TEXTURE_RESOLUTION 10

namespace wholphin {

	class Entity;

	class Tile {
		//4 different tiles...0-3
		unsigned short type;
		std::vector<Entity*> entities;
	};

	//A square of tiles and its tiletype lookup texture
	class Chunk {
		std::array<Tile, CHUNK_SIZE * CHUNK_SIZE> tiles;
		Texture tileTypeTexture;
		glm::vec2 pos;
	};

	//
	class World {
	public:
		std::vector<Chunk> chunks;
	private:

	};

	class Renderer {
		std::unordered_map<MeshData*, std::vector<glm::mat4>> renderables;
	public:
		void Submit(MeshData* mesh, const glm::mat4& mat) {
			renderables[mesh].push_back(mat);
		}
		void Present(GLuint modelMatrixIndex);
	};

}