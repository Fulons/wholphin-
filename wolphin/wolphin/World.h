#pragma once
#include <array>
#include <vector>
#include <unordered_map>
#include "RenderData.h"
#define CHUNK_SIZE 16
#define TILE_TYPE_TEXTURE_RESOLUTION 10
#define VIEW_RADIUS 256

namespace wholphin {

	class RendererSystem {
		MeshData* chunkMesh;

		std::unordered_map<MeshData*, std::vector<glm::mat4>> entityRenderables;
	public:
		void Submit(MeshData* mesh, const glm::mat4& mat);
		void SubmitChunk(const glm::mat4& mat);
		void SetChunkMesh(MeshData* mesh) { chunkMesh = mesh; }
		void Present(GLuint modelMatrixIndex);
	};

	struct Tile {
		//4 different tiles...0-3
		unsigned short type;
		//std::vector<Entity*> entities;
	};

	//A square of tiles and its tiletype lookup texture
	//IDEA: Calculate perlin noise for each tile then interpolate between tiles
	//IDEA: Data oriented design. Entities contain a single unique identifier
	//		components are stored in "componentName"-System classes.
	class ChunkSystem {
	public:
		void Init(RendererSystem* renderer, glm::vec2 pos);
		void Update(glm::vec2 pos);
	private:
		struct Chunk {
			std::array<Tile, CHUNK_SIZE * CHUNK_SIZE> tiles;
			Texture tileTypeTexture;	//IDEA: Can eventually be rendered from the tiles at initialisation
			glm::vec2 pos;
		};
		std::vector<Chunk> chunks;
		RendererSystem* renderSystem;
		static MeshData renderable;
		static bool renderableInitialized;
	};	

	//
	class World {
	public:
		void Init();
		void Update();
		void Render(GLuint modelMatrixIndex);
	private:
		std::vector<MeshData> renderables;
		RendererSystem renderer;
		ChunkSystem chunkSystem;
		glm::vec2 pos;				//Camera center position
	};

	

}