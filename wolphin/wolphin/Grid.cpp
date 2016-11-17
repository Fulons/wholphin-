#include "Grid.h"
#include <glm\gtc\matrix_transform.hpp>
#include <stdlib.h>
#include <soil\SOIL.h>
#include "Utilities.h"
#include "Noise.h"
#include <algorithm>

namespace wholphin {

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

	Texture MakeTexture(unsigned char* data, glm::ivec2 size, unsigned channels = 3) {
		Texture ret;
		ret.width = size.x;
		ret.height = size.y;
		glGenTextures(1, &ret.ID);
		glBindTexture(GL_TEXTURE_2D, ret.ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		GLint format = GL_RGB;
		switch (channels) {
		case 1: format = GL_RED; break;
		case 2: format = GL_RG; break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		}


		glTexImage2D(GL_TEXTURE_2D, 0, format, ret.width, ret.height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return ret;
	}	

	void Grid::Init() {
		palmMesh.Init("Assets\\palmScaleTest.obj");
		desGround00 = LoadTexture("Assets\\des_ground00.png");
		desGround01 = LoadTexture("Assets\\des_ground01.png");
		desGround02 = LoadTexture("Assets\\des_ground02.png");
		desGround03 = LoadTexture("Assets\\des_ground03.png");
		Texture bushTexture = LoadTexture("Assets\\shrub_00.png", SOIL_LOAD_RGBA);
		desShrub00 = LoadTexture("Assets\\des_shrub00.dds", SOIL_LOAD_RGBA);
		desShrub01 = LoadTexture("Assets\\des_shrub01.dds", SOIL_LOAD_RGBA);
		desShrub02 = LoadTexture("Assets\\des_shrub02.dds", SOIL_LOAD_RGBA);
		desShrub03 = LoadTexture("Assets\\des_shrub03.dds", SOIL_LOAD_RGBA);
		desShrub04 = LoadTexture("Assets\\des_shrub04.dds", SOIL_LOAD_RGBA);
		bushMesh[0].InitBillboard(glm::vec2(25.0f, 25.0f), bushTexture);
		bushMesh[1].InitBillboard(glm::vec2(25.0f, 25.0f), desShrub00);
		bushMesh[2].InitBillboard(glm::vec2(25.0f, 25.0f), desShrub01);
		bushMesh[3].InitBillboard(glm::vec2(25.0f, 25.0f), desShrub02);
		bushMesh[4].InitBillboard(glm::vec2(20.0f, 20.0f), desShrub03);
		bushMesh[5].InitBillboard(glm::vec2(15.0f, 15.0f), desShrub04);

		std::vector<glm::vec2> UVs = {
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f)};

		float factor = 0.0f;


		std::vector<glm::vec2> UV2s;
		UV2s.resize(size.x * size.y * 4);
		for (unsigned x = 0; x < size.x; x++) {
			for (unsigned y = 0; y < size.y; y++) {
				UV2s[(x + (y * size.x)) * 4 + 0] = glm::vec2( x / (float)size.x + factor,		 y / (float)size.y + factor);
				UV2s[(x + (y * size.x)) * 4 + 1] = glm::vec2((x + 1) / (float)size.x - factor,	 y / (float)size.y + factor);
				UV2s[(x + (y * size.x)) * 4 + 2] = glm::vec2((x + 1) / (float)size.x - factor,	(y + 1) / (float)size.y - factor);
				UV2s[(x + (y * size.x)) * 4 + 3] = glm::vec2( x / (float)size.x + factor,		(y + 1) / (float)size.y - factor);
			}
		}		
		
		tiles.resize(size.x * size.y);
		modelMatrix.resize(size.x * size.y);

		unsigned int dataMAX = 0;
		unsigned int dataMIN = 255;
		float flMAX = -10;
		float flMIN = 10;
		glm::vec2 sizeDiff = (glm::vec2)size / (glm::vec2)textureMixTextureSize;
		for (unsigned x = 0; x < textureMixTextureSize.x; x++) {
			for (unsigned y = 0; y < textureMixTextureSize.y; y++) {
				glm::vec2 perlinPos = glm::vec2((x - textureMixTextureSize.x / 2.0f), (y - textureMixTextureSize.y / 2.0f)) / (glm::vec2)textureMixTextureSize;
				float pn = (Perlin(perlinPos * 20.0f) + 1) * 0.5f;
				pn += (Perlin(perlinPos * 3.0f) + 1) * 1.5f;
				pn += (Perlin(perlinPos * 50.0f) + 1) * 0.5f;
				pn /= 2.5f;
				if (pn > flMAX) flMAX = pn;	//0.92410982
				if (pn < flMIN) flMIN = pn;	//0.122145057				
				//if (pn < 1.0f / 12.0f) pn = 0;
				//else if (pn < 1.0f / 12.0f) pn = 0;
				//else if (pn < 1.0f / 12.0f) pn = 0;
				//else if (pn < 1.0f / 12.0f) pn = 0;
				pn -= 0.5f;
				tiles[((int)(x * sizeDiff.x) + ((int)(y * sizeDiff.y) * size.x))] = Tile((int)(pn * 8), glm::vec2((int)(x * sizeDiff.x) - size.x / 2, (int)(y * sizeDiff.y) - size.y / 2));
				modelMatrix[((int)(x * sizeDiff.x) + ((int)(y * sizeDiff.y) * size.x))] = glm::scale(glm::mat4(), glm::vec3(200.0f, 200.0f, 200.0f)) * glm::translate(glm::mat4(), glm::vec3(tiles[((int)(x * sizeDiff.x) + ((int)(y * sizeDiff.y) * size.x))].pos, 0.0f));
				unsigned int data = (unsigned int)(pn * 255.0f);
				if (data > dataMAX) dataMAX = data;	//235
				if (data < dataMIN) dataMIN = data;	//31
				
				textureMixTextureData[x + (y * textureMixTextureSize.x)] = data;
			}
		}

		textureMixTexture = MakeTexture(textureMixTextureData.data(), textureMixTextureSize, 1);		


		
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {
				//glm::vec2 perlinPos = glm::vec2(x - size.x / 2.0f, y - size.y / 2.0f) / (glm::vec2)size;
				//float pn = (Perlin(perlinPos * 10.0f) + 1) * 0.5f;
				//pn += (Perlin(perlinPos * 1.5f) + 1) * 1.5f;
				//pn += (Perlin(perlinPos * 25.0f) + 1) * 0.5f;
				//pn /= 2.5f;
				int tileType = tiles[x + (y * size.x)].type;//(int)(pn * 8);
				std::vector<MapEntity*> possibleEntities;
				for (auto& entity : mapEntities){
					if ((entity.mapValueStart > tileType) &&
						(entity.mapValueEnd < tileType)){
						possibleEntities.push_back(&entity);
					}
				}

				//tiles[x + (y * size.x)] = Tile(tileType, glm::vec2(x - size.x / 2, y - size.y / 2));
				//modelMatrix[x + (y * size.x)] = glm::scale(glm::mat4(), glm::vec3(200.0f, 200.0f, 200.0f)) * glm::translate(glm::mat4(), glm::vec3(tiles[x + (y * size.x)].pos, 0.0f));
				int treeAndBushRand = rand() % 6;
				if ((tileType == 4 || tileType == 5 || tileType == 6) && (treeAndBushRand == 5 || treeAndBushRand == 3 || treeAndBushRand == 2)) {
					float randScale = rand() / (float)RAND_MAX + 0.5f;
					glm::mat4 model(1);
					glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(tiles[x + (y * size.x)].pos * 200.0f, 0.0f));
					glm::mat4 rotate = glm::rotate(glm::mat4(), (float)((rand() % 100) / 100.0f * glm::pi<float>()), glm::vec3(0.0f, 0.0f, 1.0f));
					glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(10.0f * randScale, 10.0f * randScale, 10.0f * randScale));
					model = translate * rotate * scale;
					if(treeAndBushRand == 5)
						renderables.push_back(Renderable{ model, &palmMesh });
					scale = glm::scale(glm::mat4(), glm::vec3(6.0f * randScale, 6.0f * randScale, 6.0f * randScale));
					model = translate * scale;
					if (treeAndBushRand != 5) {						
						renderables.push_back(Renderable{ model, &bushMesh[rand() % 6] });
					}
				}
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
		glGenBuffers(1, &TBO2);


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

		glBindBuffer(GL_ARRAY_BUFFER, TBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * UV2s.size(), UV2s.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		CheckGLError();
	}

	void Grid::Init(std::vector<MapEntity>* mapEntities, std::vector<Texture>* tileTextures) {
		
		this->mapEntities.swap(*mapEntities);
		this->tileTextures.swap(*tileTextures);

		InitBuffers();
		Populate();
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
		//entities.clear();
		float time = frameNumberFraction / 10.0f;
		//time = 10;
		//for (int x = 0; x < size.x; x++) {
		//	for (int y = 0; y < size.y; y++) {
		//		glm::vec3 perlinPos = glm::vec3(glm::vec2(x - size.x / 2, y - size.y / 2) / (glm::vec2)size, time);
		//		float pn = Perlin(perlinPos * 20.0f) * 0.5f;
		//		if (funkyness > 1) pn += Perlin(perlinPos * 3.0f) * 1.5f;
		//		if (funkyness > 2) pn += Perlin(perlinPos * 50.0f) * 0.5f;
		//		if (funkyness == 4) pn += sinf(sqrtf(x*x + y*y) / size.length() * glm::pi<float>() * 0.1f);
		//		if (funkyness > 4) pn += sinf((float)x / size.x * glm::pi<float>() * 10);//sin((float)y / size.y * glm::pi<float>() * 17);
		//		if (funkyness > 5) pn += sinf((float)y / size.y * glm::pi<float>() * 10);
		//		if (funkyness > 6) pn += sinf(sqrt(x*x + y*y) / size.length() * glm::pi<float>() * 0.1f) * 2;
		//		pn /= average;
		//		int tileType = (int)((pn + 1) * 9);
		//		tiles[x + (y * size.x)].type = ((pn + 1) * 9);
		//		int treeAndBushRand = rand() % 7;
		//		if ((tileType == 4 || tileType == 5) && (treeAndBushRand == 5 || treeAndBushRand == 3 || treeAndBushRand == 2)) {
		//			float randScale = (rand() % 100) / 100.0f + 0.5f;
		//			glm::mat4 model(1);
		//			glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(tiles[x + (y * size.x)].pos * 50.0f, 0.0f));
		//			glm::mat4 rotate = glm::rotate(glm::mat4(), (float)((rand() % 100) / 100.0f * glm::pi<float>()), glm::vec3(0.0f, 0.0f, 1.0f));
		//			glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(10.0f * randScale, 10.0f * randScale, 10.0f * randScale));
		//			model = translate * rotate * scale;
		//			//if(treeAndBushRand == 5)
		//			//	entities.push_back(Entity{ model, &palmMesh });
		//			scale = glm::scale(glm::mat4(), glm::vec3(6.0f * randScale, 6.0f * randScale, 6.0f * randScale));
		//			model = translate * scale;
		//			//if(treeAndBushRand != 5)
		//			//	entities.push_back(Entity{ model, &bushMesh });
		//		}
		//		//tiles[x + (y * size.x)].type = 4;
		//		//modelMatrix[x + (y * size.x)] = glm::scale(glm::mat4(), glm::vec3(50.0f, 50.0f, 1.0f)) * glm::translate(glm::mat4(), glm::vec3(tiles[x + (y * size.x)].pos, 0.0f));
		//	}
		//}
	}

	void Grid::Draw(GLuint modelMatrixIndex) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, desGround00.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, desGround01.ID);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, desGround02.ID);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, desGround03.ID);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, textureMixTexture.ID);
		CheckGLError();
		for (unsigned i = 0; i < tiles.size(); i++) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)0);

			glBindBuffer(GL_ARRAY_BUFFER, TBO);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

			glBindBuffer(GL_ARRAY_BUFFER, TBO2);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(glm::vec2) * i * 4));			

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

			glUniformMatrix4fv(modelMatrixIndex, 1, GL_FALSE, &modelMatrix[i][0][0]);			

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);			
		}
		CheckGLError();
	}

	void Grid::Draw2(GLuint modelMatrixIndex) {
		unsigned i = 0;
		for ( ; i < tileTextures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, tileTextures[i].ID);
		}
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureMixTexture.ID);

		CheckGLError();
		for (unsigned i = 0; i < tiles.size(); i++) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)0);

			glBindBuffer(GL_ARRAY_BUFFER, TBO);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

			glBindBuffer(GL_ARRAY_BUFFER, TBO2);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(glm::vec2) * i * 4));			

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

			glUniformMatrix4fv(modelMatrixIndex, 1, GL_FALSE, &modelMatrix[i][0][0]);			

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);			
		}
		CheckGLError();
	}

	void Grid::DrawEntities(GLuint modelMatrixIndex){
		glActiveTexture(GL_TEXTURE0);
		for (int i = renderables.size() - 1; i > -1; i--) {
			glBindTexture(GL_TEXTURE_2D, renderables[i].what->GetTextureID());
			glUniformMatrix4fv(modelMatrixIndex, 1, GL_FALSE, &renderables[i].where[0][0]);
			renderables[i].what->Draw();
		}
		
	}


	//Inittialises buffers and loads vertex data to the graphics card
	void Grid::InitBuffers(){
		std::vector<glm::vec2> UVs = {
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f)};

		float factor = 0.0f;
		std::vector<glm::vec2> UV2s;
		UV2s.resize(size.x * size.y * 4);
		for (unsigned x = 0; x < size.x; x++) {
			for (unsigned y = 0; y < size.y; y++) {
				UV2s[(x + (y * size.x)) * 4 + 0] = glm::vec2( x / (float)size.x + factor,		 y / (float)size.y + factor);
				UV2s[(x + (y * size.x)) * 4 + 1] = glm::vec2((x + 1) / (float)size.x - factor,	 y / (float)size.y + factor);
				UV2s[(x + (y * size.x)) * 4 + 2] = glm::vec2((x + 1) / (float)size.x - factor,	(y + 1) / (float)size.y - factor);
				UV2s[(x + (y * size.x)) * 4 + 3] = glm::vec2( x / (float)size.x + factor,		(y + 1) / (float)size.y - factor);
			}
		}		

		unsigned int dataMAX = 0;
		unsigned int dataMIN = 255;
		float flMAX = -10;
		float flMIN = 10;
		tiles.resize(size.x * size.y);
		modelMatrix.resize(size.x * size.y);
		glm::vec2 sizeDiff = (glm::vec2)size / (glm::vec2)textureMixTextureSize;
		for (unsigned x = 0; x < textureMixTextureSize.x; x++) {
			for (unsigned y = 0; y < textureMixTextureSize.y; y++) {
				glm::vec2 perlinPos = glm::vec2((x - textureMixTextureSize.x / 2.0f), (y - textureMixTextureSize.y / 2.0f)) / (glm::vec2)textureMixTextureSize;
				float pn = (Perlin(perlinPos * 20.0f) + 1) * 0.5f;
				pn += (Perlin(perlinPos * 3.0f) + 1) * 1.5f;
				pn += (Perlin(perlinPos * 50.0f) + 1) * 0.5f;
				pn /= 2.5f;
				pn -= 0.5f;
				//TODO: setup tiles in the populate loop
				tiles[((int)(x * sizeDiff.x) + ((int)(y * sizeDiff.y) * size.x))] = Tile((int)(pn * 8), glm::vec2((int)(x * sizeDiff.x) - size.x / 2, (int)(y * sizeDiff.y) - size.y / 2));
				unsigned int data = (unsigned int)(pn * 255.0f);
				textureMixTextureData[x + (y * textureMixTextureSize.x)] = data;
			}
		}

		textureMixTexture = MakeTexture(textureMixTextureData.data(), textureMixTextureSize, 1);	

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
		glGenBuffers(1, &TBO2);


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

		glBindBuffer(GL_ARRAY_BUFFER, TBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * UV2s.size(), UV2s.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		CheckGLError();
	}

	void Grid::Populate(){

		//glm::vec2 sizeDiff =  (glm::vec2)textureMixTextureSize / (glm::vec2)size;
		
		std::vector<MapEntity*> possibleEntities;

		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {		  //this might break...	  + 0.5f						   + 0.5f
				//tiles[x + (y * size.x)] = Tile(textureMixTextureData[(int)((x + 0.5f) * sizeDiff.x) + (int)((y + 0.5f) * sizeDiff.y) * textureMixTextureSize.y], glm::vec2(x - size.x / 2, y - size.y / 2));
				modelMatrix[x + (y * size.x)] = glm::scale(glm::mat4(), glm::vec3(200.0f, 200.0f, 200.0f)) * glm::translate(glm::mat4(), glm::vec3(tiles[x + (y * size.x)].pos, 0.0f));

				int tileType = tiles[x + (y * size.x)].type;
				int treeAndBushRand = rand() % 6;
				if ((tileType == 4 || tileType == 5 || tileType == 6) && (treeAndBushRand == 5 || treeAndBushRand == 3 || treeAndBushRand == 2)) {
					float randScale = rand() / (float)RAND_MAX + 0.5f;
					glm::mat4 model(1);
					glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(tiles[x + (y * size.x)].pos * 200.0f, 0.0f));
					glm::mat4 rotate = glm::rotate(glm::mat4(), (float)((rand() % 100) / 100.0f * glm::pi<float>() * 2), glm::vec3(0.0f, 0.0f, 1.0f));
					glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(10.0f * randScale, 10.0f * randScale, 10.0f * randScale));
					model = translate * rotate * scale;
					if(treeAndBushRand == 5)
						renderables.push_back(Renderable{ model, &palmMesh });
					scale = glm::scale(glm::mat4(), glm::vec3(6.0f * randScale, 6.0f * randScale, 6.0f * randScale));
					model = translate * scale;
					if (treeAndBushRand != 5) {						
						renderables.push_back(Renderable{ model, &bushMesh[rand() % 6] });
					}
				}
			}
		}
	}

}