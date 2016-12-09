#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

namespace wholphin {


	struct Texture {
		int width;
		int height;
		GLuint ID;
	};
	Texture LoadTexture(const char* file, int forceChannels = 3/*RGB*/);

	struct Vertex2D {
		glm::vec2 pos;
	};

	struct Vertex3D {
		glm::vec2 texCoords;
		glm::vec3 pos;
	};

	struct Buffer {
		GLuint VBO;
		GLuint IBO;
		GLuint VAO;
	};

	struct SubMeshData {
		std::vector<Vertex3D> vertices;
		std::vector<GLuint> indices;
		Texture texture;
		GLuint indexOffset;
	};

	class MeshData {
	public:
		bool Init(const char* filename);
		void InitBillboard(glm::vec2 size, Texture texture);
		void Draw();
		void DrawInitialized();
		void BindBuffers();
		void UnbindBuffers();
		GLuint GetTextureID() {
			return subMeshes[0].texture.ID;
		}
		GLuint GetNumIndices() { return numIndices; }
	private:
		GLuint numIndices;
		void MakeBuffer();
		bool isInitialized = false;
		std::vector<SubMeshData> subMeshes;
		Buffer buffer;

	private:
		aiString directory;
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(SubMeshData& subMesh, aiMesh* mesh, const aiScene* scene);
	};

}