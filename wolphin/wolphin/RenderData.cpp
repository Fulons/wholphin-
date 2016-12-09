#include "RenderData.h"
#include "Utilities.h"
#include <soil\SOIL.h>

namespace wholphin {

	Texture LoadTexture(const char * file, int forceChannels){
		Texture ret;
		int channels = 0;
		unsigned char* image = SOIL_load_image(file, &ret.width, &ret.height, &channels, forceChannels);
		if (!image) {
			OutErrorMessage((std::string("Could not load texture:") + file).c_str());
			return Texture();
		}
		glGenTextures(1, &ret.ID);
		glBindTexture(GL_TEXTURE_2D, ret.ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		GLint format = GL_RGB;
		switch (channels) {
		case SOIL_LOAD_RGB: format = GL_RGB; break;
		case SOIL_LOAD_RGBA: format = GL_RGBA; break;
		}


		glTexImage2D(GL_TEXTURE_2D, 0, format, ret.width, ret.height, 0, format, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		return ret;
	}

	bool MeshData::Init(const char* filename) {		
		Assimp::Importer import;
		std::string file(filename);
		const aiScene* scene = import.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals );

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			return OutErrorMessage((std::string("Assimp error") + import.GetErrorString()).c_str());
		}
		directory = file.substr(0, file.find_last_of('\\'));
		ProcessNode(scene->mRootNode, scene);
		MakeBuffer();
	}

	void MeshData::InitBillboard(glm::vec2 size, Texture texture) {
		subMeshes.push_back(SubMeshData());
		subMeshes[0].vertices = { Vertex3D{glm::vec2(1.0f, 1.0f), glm::vec3(-size.x / 2.0f, size.y / 2.0f, 0)},
			Vertex3D{glm::vec2(0.0f, 1.0f), glm::vec3(size.x / 2.0f, -size.y / 2.0f, 0)},
			Vertex3D{glm::vec2(0.0f, 0.0f), glm::vec3(size.x / 2.0f, -size.y / 2.0f, size.y)},
			Vertex3D{glm::vec2(1.0f, 0.0f), glm::vec3(-size.x / 2.0f, size.y / 2.0f, size.y)} };
		subMeshes[0].texture = texture;
		subMeshes[0].indices = { 0, 1, 2, 0, 2, 3 };
		MakeBuffer();
	}

	void MeshData::Draw(){
		BindBuffers();
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
		UnbindBuffers();
	}

	void MeshData::DrawInitialized() {
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
	}

	void MeshData::BindBuffers(){
		glBindVertexArray(buffer.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.IBO);
	}

	void MeshData::UnbindBuffers(){
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void MeshData::ProcessNode(aiNode* node, const aiScene* scene) {
		for (GLuint i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			subMeshes.push_back(SubMeshData());
			ProcessMesh(subMeshes.back(), mesh, scene);
		}
		for (GLuint i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene);
		}
	}

	void MeshData::ProcessMesh(SubMeshData& subMesh, aiMesh* mesh, const aiScene * scene)	{
		for (GLuint i = 0; i < mesh->mNumVertices; i++) {
			Vertex3D vertex;
			vertex.pos = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].z, mesh->mVertices[i].y);
			vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			subMesh.vertices.push_back(vertex);
		}

		for (GLuint i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (GLuint j = face.mNumIndices - 1; j < face.mNumIndices; j--)
				subMesh.indices.push_back(face.mIndices[j]);
		}

		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			GLuint textureCount = material->GetTextureCount(aiTextureType_AMBIENT);
			material->GetTexture(aiTextureType_DIFFUSE, 0, &directory);
			subMesh.texture = LoadTexture(directory.C_Str());
		}
	}

	void MeshData::MakeBuffer() {
		glGenBuffers(1, &buffer.VBO);
		glGenBuffers(1, &buffer.IBO);
		glGenVertexArrays(1, &buffer.VAO);
		glBindVertexArray(buffer.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.IBO);
		GLsizeiptr VBOBufferSize = 0;
		GLsizeiptr IBOBufferSize = 0;
		for (auto mesh : subMeshes) {
			VBOBufferSize += mesh.vertices.size();
			IBOBufferSize += mesh.indices.size();
		}
		numIndices = IBOBufferSize;
		VBOBufferSize *= sizeof(Vertex3D);
		IBOBufferSize *= sizeof(GLuint);
		glBufferData(GL_ARRAY_BUFFER, VBOBufferSize, nullptr, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IBOBufferSize, nullptr, GL_STATIC_DRAW);

		GLintptr vboRunner = 0;
		GLintptr iboRunner = 0;
		for (unsigned i = 0; i < subMeshes.size(); i++) {
			if (i != 0) {
				GLuint indexOffset = vboRunner / sizeof(Vertex3D);
				for (unsigned j = 0; j < subMeshes[i].indices.size(); j++)
					subMeshes[i].indices[j] += indexOffset;
			}
			GLsizeiptr vertexSize = subMeshes[i].vertices.size() * sizeof(Vertex3D);
			GLsizeiptr indexSize = subMeshes[i].indices.size() * sizeof(GLuint);
			glBufferSubData(GL_ARRAY_BUFFER, vboRunner, vertexSize, subMeshes[i].vertices.data());
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, iboRunner, indexSize, subMeshes[i].indices.data());
			vboRunner += vertexSize;
			iboRunner += indexSize;
		}

		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (GLvoid*)(sizeof(glm::vec2)));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}