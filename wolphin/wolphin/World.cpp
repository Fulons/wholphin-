#include "World.h"
#include "Noise.h"
#include <glm\gtc\matrix_transform.hpp>

namespace wholphin{
	//////////////////////////////
	void RendererSystem::Submit(MeshData * mesh, const glm::mat4 & mat){
		entityRenderables[mesh].push_back(mat);
	}

	void RendererSystem::SubmitChunk(const glm::mat4& mat){

	}

	void RendererSystem::Present(GLuint modelMatrixIndex) {
		for (auto renderable : entityRenderables) {
			renderable.first->BindBuffers();
			for (auto instance : renderable.second) {
				glUniformMatrix4fv(modelMatrixIndex, 1, GL_FALSE, &instance[0][0]);
				renderable.first->DrawInitialized();
			}
			renderable.first->UnbindBuffers();
			renderable.second.clear();
		}
	}

	////////////////////////////
	bool ChunkSystem::renderableInitialized = false;
	MeshData ChunkSystem::renderable;

	void ChunkSystem::Init(RendererSystem* renderer, glm::vec2 pos){
		if (!renderableInitialized) {
			renderable.InitRect(glm::vec2(CHUNK_SIZE, CHUNK_SIZE));
			renderableInitialized = true;
		}
		renderer->SetChunkMesh(&renderable);
		renderSystem = renderer;

		///////Temporary test/////////
		glm::vec2 currentChunkPos = pos - glm::vec2(CHUNK_SIZE * 1, CHUNK_SIZE * 1);
		for (unsigned i = 0; i < 9; i++) {			
			chunks.push_back(Chunk());
			Chunk& chunk = chunks.back();
			chunk.pos = currentChunkPos;
			for (unsigned y = 0; y < CHUNK_SIZE; y++) {
				for (unsigned x = 0; x < CHUNK_SIZE; x++) {
					glm::vec2 perlinPos = (chunk.pos + glm::vec2(x + 0.5f, y + 0.5f)) / glm::vec2(CHUNK_SIZE, CHUNK_SIZE);
					float pn = Perlin(perlinPos);
					chunk.tiles[x + y * CHUNK_SIZE].type = (unsigned short)((pn + 1) * 4 / 2);
					if (chunk.tiles[x + y * CHUNK_SIZE].type == 4) {
						chunk.tiles[x + y * CHUNK_SIZE].type = 3;
					}
				}
			}
			std::vector<unsigned char> textureData;
			textureData.resize(CHUNK_SIZE * TILE_TYPE_TEXTURE_RESOLUTION * CHUNK_SIZE * TILE_TYPE_TEXTURE_RESOLUTION);
			for (unsigned y = 0; y < CHUNK_SIZE * TILE_TYPE_TEXTURE_RESOLUTION; y++) {
				for (unsigned x = 0; x < CHUNK_SIZE * TILE_TYPE_TEXTURE_RESOLUTION; x++) {
					//Check if this access the right tile....
					textureData[x + y * CHUNK_SIZE * TILE_TYPE_TEXTURE_RESOLUTION] = chunk.tiles[x + (y / TILE_TYPE_TEXTURE_RESOLUTION) * CHUNK_SIZE].type;
				}
			}
			chunk.tileTypeTexture = MakeTexture(textureData.data(), glm::ivec2(CHUNK_SIZE * TILE_TYPE_TEXTURE_RESOLUTION, CHUNK_SIZE * TILE_TYPE_TEXTURE_RESOLUTION), 1);
			switch (i) {
			case 0: currentChunkPos = pos - glm::vec2(CHUNK_SIZE * 0, CHUNK_SIZE * 1);
			case 1: currentChunkPos = pos - glm::vec2(-CHUNK_SIZE * 1, CHUNK_SIZE * 1);
			case 2: currentChunkPos = pos - glm::vec2(CHUNK_SIZE * 1, CHUNK_SIZE * 0);
			case 3: currentChunkPos = pos - glm::vec2(CHUNK_SIZE * 0, CHUNK_SIZE * 0);
			case 4: currentChunkPos = pos - glm::vec2(-CHUNK_SIZE * 1, CHUNK_SIZE * 0);
			case 5: currentChunkPos = pos - glm::vec2(CHUNK_SIZE * 1, -CHUNK_SIZE * 1);
			case 6: currentChunkPos = pos - glm::vec2(CHUNK_SIZE * 0, -CHUNK_SIZE * 1);
			case 7: currentChunkPos = pos - glm::vec2(-CHUNK_SIZE * 1, -CHUNK_SIZE * 1);
			}

			renderSystem->SubmitChunk(glm::translate(glm::mat4(), glm::vec3(chunk.pos, 0.0f)));
		}
		//////////////////////////////
	}

	void ChunkSystem::Update(glm::vec2 pos) {

	}

	/////////////////////////////
	void World::Init() {
		chunkSystem.Init(&renderer, pos);		
	}

	void World::Update() {
		chunkSystem.Update(pos);
	}

	void World::Render(GLuint modelMatrixIndex) {
		renderer.Present(modelMatrixIndex);
	}

}

