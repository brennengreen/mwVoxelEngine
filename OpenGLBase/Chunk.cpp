#include "Chunk.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <FastNoise/FastNoise.h>

Chunk::Chunk()
{
	m_pVoxelsRender = new Voxel **[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		m_pVoxelsRender[i] = new Voxel * [CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; j++) {
			m_pVoxelsRender[i][j] = new Voxel[CHUNK_SIZE];
		}
	}

	m_pHeightMap = new float *[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		m_pHeightMap[i] = new float [CHUNK_SIZE];
	}

	GenerateTerrain();
	CreateMesh();
}

Chunk::~Chunk()
{
	 for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			delete[] m_pVoxelsRender[i][j];
		}
		delete[] m_pVoxelsRender[i];
	 }
	 delete[] m_pVoxelsRender;

	  for (int i = 0; i < CHUNK_SIZE; ++i) {
		delete[] m_pHeightMap[i];
	 }
	 delete[] m_pHeightMap;

	 glDeleteVertexArrays(1, &m_chunkVAO);
	 glDeleteBuffers(1, &m_chunkVertexBuffer);
	 glDeleteBuffers(1, &m_chunkAttributeBuffer);
}

void Chunk::GenerateTerrain() {
	FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree("DQAFAAAArkcBQAcAAD0K1z4AAAAAAA==");
	//FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree("EgAFAAAA9ihcPxAAKVwPvwcAAFyPwj4AcT0CQQApXI8+");
	//FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree("CwABAAAAAQAAAAAAAAACAAAAARIABQAAAPYoXD8QAClcD78HAABcj8I+AHE9AkEAKVyPPg==");

	std::vector<float> noiseOutput025f(CHUNK_SIZE * CHUNK_SIZE);
	std::vector<float> noiseOutput050f(CHUNK_SIZE * CHUNK_SIZE);
	std::vector<float> noiseOutput100f(CHUNK_SIZE * CHUNK_SIZE);

	fnGenerator->GenUniformGrid2D(
		noiseOutput025f.data(),
		0, 0,
		CHUNK_SIZE, CHUNK_SIZE,
		0.0125f, 758
	);

	fnGenerator->GenUniformGrid2D(
		noiseOutput050f.data(),
		0, 0,
		CHUNK_SIZE, CHUNK_SIZE,
		0.025f, 758
	);

	fnGenerator->GenUniformGrid2D(
		noiseOutput100f.data(),
		0, 0,
		CHUNK_SIZE, CHUNK_SIZE,
		0.05f, 758
	);

	int j = 0;
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			m_pHeightMap[x][z] = 1.f*noiseOutput025f[j] + 0.5f*noiseOutput050f[j] + 0.25f*noiseOutput100f[j];
			j++;
		}
	}

	ErodeTerrain();

	for (GLuint x = 0; x < CHUNK_SIZE; x++) {
		for (GLuint z = 0; z < CHUNK_SIZE; z++) {
			float h = m_pHeightMap[x][z];
			h += .80f;
			h *= CHUNK_SIZE/2;
			h = (h <= 0) ? 1.f : h;
			h = (h > CHUNK_SIZE) ? CHUNK_SIZE : h;
			for (GLuint y = 0; y < (int)h; y++) {
				m_pVoxelsRender[x][y][z].SetActive(true);
				double tolerance = (((double)rand() / RAND_MAX)*10.f);
				double snow_height = 0.6*CHUNK_SIZE;
				double stone_height = 0.015*CHUNK_SIZE;
				double stone_percentage = 0.94;

				if (y < stone_height + tolerance || y < stone_height - tolerance) {
					m_pVoxelsRender[x][y][z].SetVoxelType(VoxelType::VoxelType_Stone);
				} else if (y < snow_height + tolerance || y < snow_height - tolerance) {
					double r = (double)rand() / RAND_MAX;
					if (y >= h-2) {
						m_pVoxelsRender[x][y][z].SetVoxelType(VoxelType::VoxelType_Grass);
					} else {
						m_pVoxelsRender[x][y][z].SetVoxelType(r < stone_percentage ? VoxelType_Dirt : VoxelType_Stone);
					}
				}  else {
					m_pVoxelsRender[x][y][z].SetVoxelType(VoxelType::VoxelType_Snow);
				}
			}
		}
	}
}


glm::vec3 Chunk::SurfaceNormal(int i, int j){
	float scale = 60.f;
	glm::vec3 n = glm::vec3(0.15) * glm::normalize(glm::vec3(scale*(m_pHeightMap[i][j]-m_pHeightMap[i+1][j]), 1.0, 0.0));  //Positive X
	n += glm::vec3(0.15) * glm::normalize(glm::vec3(scale*(m_pHeightMap[i-1][j]-m_pHeightMap[i][j]), 1.0, 0.0));  //Negative X
	n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale*(m_pHeightMap[i][j]-m_pHeightMap[i][j+1])));    //Positive Y
	n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale*(m_pHeightMap[i][j-1]-m_pHeightMap[i][j])));  //Negative Y

	//Diagonals! (This removes the last spatial artifacts)
	n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale*(m_pHeightMap[i][j]-m_pHeightMap[i+1][j+1])/sqrt(2), sqrt(2), scale*(m_pHeightMap[i][j]-m_pHeightMap[i+1][j+1])/sqrt(2)));    //Positive Y
	n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale*(m_pHeightMap[i][j]-m_pHeightMap[i+1][j-1])/sqrt(2), sqrt(2), scale*(m_pHeightMap[i][j]-m_pHeightMap[i+1][j-1])/sqrt(2)));    //Positive Y
	n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale*(m_pHeightMap[i][j]-m_pHeightMap[i-1][j+1])/sqrt(2), sqrt(2), scale*(m_pHeightMap[i][j]-m_pHeightMap[i-1][j+1])/sqrt(2)));    //Positive Y
	n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale*(m_pHeightMap[i][j]-m_pHeightMap[i-1][j-1])/sqrt(2), sqrt(2), scale*(m_pHeightMap[i][j]-m_pHeightMap[i-1][j-1])/sqrt(2)));    //Positive Y

	return n;
}

void Chunk::ErodeTerrain() {
	float dt = 1.2f;
	int num_particles = 250000;
	float density = 1.0f;
	float evapRate = 0.001f;
	float friction = 0.05f;
	float depositionRate = 0.1f;
	float minVolume = 0.01f;
	for (int i = 0; i < num_particles; i++) {
		mw::Particle drop(glm::vec2(CHUNK_SIZE*((double)rand() / RAND_MAX), CHUNK_SIZE*((double)rand() / RAND_MAX)));
		while (drop.volume > minVolume) {
			glm::ivec2 ipos = drop.pos;
			if (ipos.x < 1 || ipos.x >= CHUNK_SIZE-1 || ipos.y < 1 || ipos.y >= CHUNK_SIZE-1) break;
			glm::vec3 n = SurfaceNormal(ipos.x, ipos.y); // surface normal
			
			drop.speed += dt*glm::vec2(n.x, n.z)/(drop.volume*density);
			drop.pos += dt*drop.speed;
			drop.speed *= (1.0-dt*friction);

			if (drop.pos.x < 1 || drop.pos.x >= CHUNK_SIZE-1 || drop.pos.y < 1 || drop.pos.y >= CHUNK_SIZE-1) break;
			float c_eq = drop.volume * glm::length(drop.speed)*(m_pHeightMap[ipos.x][ipos.y]-m_pHeightMap[(int)drop.pos.x][(int)drop.pos.y]);
			if (c_eq < 0.0) c_eq = 0.0;

			float cdiff = c_eq - drop.sediment;

			drop.sediment += dt * depositionRate * cdiff;
			m_pHeightMap[ipos.x][ipos.y] -= dt * drop.volume * depositionRate * cdiff;

			drop.volume *= (1.0-dt*evapRate);
		}
	}
}

void Chunk::CreateMesh()
{
	for (GLuint x = 0; x < CHUNK_SIZE; x++) {
		for (GLuint y = 0; y < CHUNK_SIZE; y++) {
			for (GLuint z = 0; z < CHUNK_SIZE; z++) {
				if (m_pVoxelsRender[x][y][z].IsActive()) {
					AddVoxel(x, y, z, m_pVoxelsRender[x][y][z].GetVoxelType());
				}
			}
		}
	}

	glGenVertexArrays(1, &m_chunkVAO);
    glGenBuffers(1, &m_chunkVertexBuffer);
    glGenBuffers(1, &m_chunkAttributeBuffer);

    glBindVertexArray(m_chunkVAO);

	/*for (int i = 2; i <= m_vertices.size(); i += 3)
		std::cout << m_vertices[i-2] << " " << m_vertices[i-1] << " " << m_vertices[i] << std::endl;*/

	// position attribute
	GLuint num_vertices = static_cast<GLsizei>(m_vertices.size());
	GLsizeiptr vertexSize = num_vertices * sizeof(GLfloat);
    glBindBuffer(GL_ARRAY_BUFFER, m_chunkVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, &m_vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

	GLuint num_attributes = static_cast<GLsizei>(m_attributes.size());
	GLsizeiptr attributeSize = num_attributes * sizeof(unsigned int);
	glBindBuffer(GL_ARRAY_BUFFER, m_chunkAttributeBuffer);
    glBufferData(GL_ARRAY_BUFFER, attributeSize, &m_attributes.front(), GL_STATIC_DRAW);

	// normal attribute
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(unsigned int), (void*)0);
	glEnableVertexAttribArray(1);
	
	// color attribute
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(unsigned int), (void*)sizeof(unsigned int));
	glEnableVertexAttribArray(2);

}

void Chunk::AddVoxel(GLuint x, GLuint y, GLuint z, VoxelType type) {
	GLfloat VoxelSize = 1.0f;
	GLfloat VoxelHalfSize = VoxelSize / 2.0f;
	GLfloat vertexBufferData[] = {
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize, // front
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,

		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize, // rear
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,

		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize, // right
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,

		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize, // left
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,

		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize, // top
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,

		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize, // bottom
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize
	};

	unsigned verticesArraySize = sizeof(vertexBufferData) / sizeof(GLfloat);
	m_vertices.insert(m_vertices.end(), &vertexBufferData[0], &vertexBufferData[verticesArraySize]);


	unsigned int color;
	switch (type) {
	case VoxelType::VoxelType_Grass:
		color = 0b0001;
		break;
	case VoxelType::VoxelType_Stone:
		color = 0b0010;
		break;
	case VoxelType::VoxelType_Snow:
		color = 0b0011;
		break;
	case VoxelType::VoxelType_Dirt:
		color = 0b0100;
		break;
	default:
		color = 0b0000;
		break;
	}

	unsigned int attrBufferData[] = {
		0b0000, color, // Front (0, 0, 1)
		0b0000, color,
		0b0000, color,
		0b0000, color,
		0b0000, color,
		0b0000, color,
			    
		0b0001, color, // Back (0, 0, -1)
		0b0001, color, 
		0b0001, color, 
		0b0001, color, 
		0b0001, color, 
		0b0001, color, 		 
			    
		0b0010, color, // Right (1, 0, 0)
		0b0010, color,
		0b0010, color,
		0b0010, color,
		0b0010, color,
		0b0010, color,
			    
		0b0011, color, // Left (-1, 0, 0)
		0b0011, color,
		0b0011, color,
		0b0011, color,
		0b0011, color,
		0b0011, color,
			    
		0b0100, color, // Top (0, 1, 0)
		0b0100, color,
		0b0100, color,
		0b0100, color,
		0b0100, color,
		0b0100, color,
			    
		0b0101, color, // Bottom (0, -1, 0)
		0b0101, color,
		0b0101, color,
		0b0101, color,
		0b0101, color,
		0b0101, color
	};

	unsigned attrArraySize = sizeof(attrBufferData) / sizeof(unsigned int);
	m_attributes.insert(m_attributes.end(), &attrBufferData[0], &attrBufferData[attrArraySize]);
}


void Chunk::Update(float dt)
{
}

void Chunk::Draw(Shader &shader, GLuint x, GLuint y, GLuint z)
{
	//std::cout << x << " " << y << " " << z << std::endl;
	shader.use();
	glBindVertexArray(m_chunkVAO);

	glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x*CHUNK_SIZE, y*CHUNK_SIZE, z*CHUNK_SIZE));
    shader.setMat4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size()/3);
	glBindVertexArray(0);
}
