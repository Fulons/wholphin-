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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

	bool MeshData::Init(const char* filename) {		
		Assimp::Importer import;
		std::string file(filename);
		const aiScene* scene = import.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			return OutErrorMessage((std::string("Assimp error") + import.GetErrorString()).c_str());
		}
		directory = file.substr(0, file.find_last_of('\\'));
		ProcessNode(scene->mRootNode, scene);
		MakeBuffer();
	}

	void MeshData::Draw(){
		glBindVertexArray(buffer.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.IBO);
		glDrawElements(GL_TRIANGLES, subMeshes[0].indices.size(), GL_UNSIGNED_INT, NULL);
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
			aiString str;
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * subMeshes[0].vertices.size(), subMeshes[0].vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * subMeshes[0].indices.size(), subMeshes[0].indices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (GLvoid*)(sizeof(glm::vec2)));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Grid::Init() {
		palmMesh.Init("Assets\\palmScaleTest.obj");
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(4950.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		entities.push_back(Entity{ model, &palmMesh });

		std::vector<glm::vec2> UVs;
		UVs.resize(4 * 4 * 4);
		float factor = 0.001f;
		for (unsigned x = 0; x < 4; x++) {
			for (unsigned y = 0; y < 4; y++) {
				UVs[(x + (y * 4)) * 4 + 0] = glm::vec2( x / 4.0f + factor,	 y / 4.0f + factor);
				UVs[(x + (y * 4)) * 4 + 1] = glm::vec2((x + 1) / 4.0f - factor,		 y / 4.0f + factor);
				UVs[(x + (y * 4)) * 4 + 2] = glm::vec2((x + 1) / 4.0f - factor,		(y + 1) / 4.0f - factor);
				UVs[(x + (y * 4)) * 4 + 3] = glm::vec2( x / 4.0f + factor,			(y + 1) / 4.0f - factor);
			}
		}		

		textureMap = LoadTexture("tileMap.png");
		modelMatrix.resize(size.x * size.y);
		tiles.resize(size.x * size.y);
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {
				glm::vec2 perlinPos = glm::vec2(x - size.x / 2, y - size.y / 2) / (glm::vec2)size;
				float pn = Perlin(perlinPos * 20.0f) * 0.5f;
				pn += Perlin(perlinPos * 3.0f) * 1.5f;
				pn += sin((float)x / size.x * glm::pi<float>() * 10);
				pn /= 3.0f;
				int tileType = (int)((pn + 1) * 6);
				tiles[x + (y * size.x)] = Tile(tileType, glm::vec2(x - size.x / 2, y - size.y / 2));
				modelMatrix[x + (y * size.x)] = glm::scale(glm::mat4(), glm::vec3(50.0f, 50.0f, 50.0f)) * glm::translate(glm::mat4(), glm::vec3(tiles[x + (y * size.x)].pos, 0.0f));
			}
		}

		Vertex2D vertices[] = {
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
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)0);

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

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		CheckGLError();
	}

	void Grid::Update(float frameNumberFraction, int funkyness)	{
		
		float average;
		switch (funkyness) {
		case 1: average = 0.5f; break;
		case 2: average = 2.0f; break;
		case 3: average = 2.5f; break;
		case 4: average = 3.5f; break;
		case 5: average = 3.5f; break;
		case 6: average = 4.5f; break;
		case 7: average = 6.5f; break;
		}
		entities.clear();
		float time = frameNumberFraction / 100.0f;
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {
				glm::vec3 perlinPos = glm::vec3(glm::vec2(x - size.x / 2, y - size.y / 2) / (glm::vec2)size, time);
				float pn = Perlin(perlinPos * 20.0f) * 0.5f;
				if (funkyness > 1) pn += Perlin(perlinPos * 3.0f) * 1.5f;
				if (funkyness > 2) pn += Perlin(perlinPos * 50.0f) * 0.5f;
				if (funkyness == 4) pn += sinf(sqrt(x*x + y*y) / size.length() * glm::pi<float>() * 0.1f);
				if (funkyness > 4) pn += sinf((float)x / size.x * glm::pi<float>() * 10);//sin((float)y / size.y * glm::pi<float>() * 17);
				if (funkyness > 5) pn += sinf((float)y / size.y * glm::pi<float>() * 10);
				if (funkyness > 6) pn += sinf(sqrt(x*x + y*y) / size.length() * glm::pi<float>() * 0.1f) * 2;
				pn /= average;
				int tileType = (int)((pn + 1) * 9);
				tiles[x + (y * size.x)].type = tileType;
				if (tileType == 4 && (rand() % 7) == 5) {
					glm::mat4 model(1);
					model = glm::translate(model, glm::vec3(tiles[x + (y * size.x)].pos * 50.0f, 0.0f));
					model = glm::rotate(model, (float)((rand() % 100) / 100.0f * glm::pi<float>()), glm::vec3(0.0f, 0.0f, 1.0f));
					model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
					entities.push_back(Entity{ model, &palmMesh });
				}
				//modelMatrix[x + (y * size.x)] = glm::scale(glm::mat4(), glm::vec3(50.0f, 50.0f, 1.0f)) * glm::translate(glm::mat4(), glm::vec3(tiles[x + (y * size.x)].pos, 0.0f));
			}
		}
	}

	void Grid::Draw(GLuint modelMatrixIndex) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureMap.ID);
		for (unsigned i = 0; i < tiles.size(); i++) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)0);

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

	void Grid::DrawEntities(GLuint modelMatrixIndex){
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, palmMesh.GetTextureID());
		//glm::mat4 model;
		//model = glm::translate(model, glm::vec3(4950.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		//glUniformMatrix4fv(modelMatrixIndex, 1, GL_FALSE, &model[0][0]);
		//palmMesh.Draw();
		for (unsigned i = 0; i < entities.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, entities[i].what->GetTextureID());
			glUniformMatrix4fv(modelMatrixIndex, 1, GL_FALSE, &entities[i].where[0][0]);
			entities[i].what->Draw();
		}
		
	}

}