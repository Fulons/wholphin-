#pragma once
#include <glm\glm.hpp>
#include <vector>
#include <GL\glew.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

namespace wholphin {

	struct Texture {
		int width;
		int height;
		GLuint ID;
	};

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
	};	

	class MeshData {
	public:
		bool Init(const char* filename);
		void InitBillboard(glm::vec2 size, Texture texture);
		void Draw();
		GLuint GetTextureID() {
			return subMeshes[0].texture.ID;
		}
	private:
		void MakeBuffer();
		bool isInitialized = false;
		std::vector<SubMeshData> subMeshes;
		Buffer buffer;

	private:
		aiString directory;
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(SubMeshData& subMesh, aiMesh* mesh, const aiScene* scene);
	};

	struct Entity {
		glm::mat4 where;
		MeshData* what;
	};

	Texture LoadTexture(const char* file, int forceChannels = 3/*RGB*/);

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
		void Update(float frameNumberFraction, int funkyness);
		void Draw(GLuint modelMatrixIndex);
		void DrawEntities(GLuint modelMatrixIndex);
	private:
		glm::ivec2 size;
		std::vector<Tile> tiles;
		std::vector<glm::mat4> modelMatrix;
	private:
		MeshData meshData;
		MeshData palmMesh;
		MeshData bushMesh;
		
		Texture textureMap;
		std::vector<glm::ivec2> texturePos;
		std::vector<Entity> entities;
		GLuint VBO;
		GLuint CBO;
		GLuint IBO;
		GLuint TBO;
		GLuint VAO;
	};

}