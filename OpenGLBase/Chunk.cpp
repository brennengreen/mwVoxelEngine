#include "Chunk.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <FastNoise/FastNoise.h>

Chunk::Chunk()
{
	m_pVoxels = new Voxel **[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		m_pVoxels[i] = new Voxel * [CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; j++) {
			m_pVoxels[i][j] = new Voxel[CHUNK_SIZE];
		}
	}

	GenerateTerrain();
	CreateMesh();
}

Chunk::~Chunk()
{
	 for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			delete[] m_pVoxels[i][j];
		}
		delete[] m_pVoxels[i];
	 }
	 delete[] m_pVoxels;
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
		0.025f, 1340
	);

	fnGenerator->GenUniformGrid2D(
		noiseOutput050f.data(),
		0, 0,
		CHUNK_SIZE, CHUNK_SIZE,
		0.05f, 1340
	);

	fnGenerator->GenUniformGrid2D(
		noiseOutput100f.data(),
		0, 0,
		CHUNK_SIZE, CHUNK_SIZE,
		0.1f, 1340
	);

	int i = 0;
	for (GLuint x = 0; x < CHUNK_SIZE; x++) {
		for (GLuint z = 0; z < CHUNK_SIZE; z++) {
			float h = 1.f*noiseOutput025f[i] + 0.5f*noiseOutput050f[i] + 0.25f*noiseOutput100f[i];
			i++;
			h += 0.50f;
			h *= CHUNK_SIZE/2;
			h = (h <= 0) ? 1.f : h;
			h = (h > CHUNK_SIZE) ? CHUNK_SIZE : h;
			for (GLuint y = 0; y < (int)h; y++) {
				m_pVoxels[x][y][z].SetActive(true);
			}
		}
	}
}

void Chunk::CreateMesh()
{
	for (GLuint x = 0; x < CHUNK_SIZE; x++) {
		for (GLuint y = 0; y < CHUNK_SIZE; y++) {
			for (GLuint z = 0; z < CHUNK_SIZE; z++) {
				if (m_pVoxels[x][y][z].IsActive()) {
					AddVoxel(x, y, z);
				}
			}
		}
	}

	glGenVertexArrays(1, &m_chunkVAO);
    glGenBuffers(1, &m_chunkVertexBuffer);
    glGenBuffers(1, &m_chunkNormalBuffer);

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

	// normal attribute
	GLuint num_normals = static_cast<GLsizei>(m_normals.size());
	GLsizeiptr normalSize = num_normals * sizeof(unsigned int);
	glBindBuffer(GL_ARRAY_BUFFER, m_chunkNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normalSize, &m_normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(unsigned int), (void*)0);
	glEnableVertexAttribArray(1);

}

void Chunk::AddVoxel(GLuint x, GLuint y, GLuint z) {
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

	unsigned int normalBufferData[] = {
		0b0000, // Front (0, 0, 1)
		0b0000,
		0b0000,
		0b0000,
		0b0000,
		0b0000,

		0b0001, // Back (0, 0, -1)
		0b0001, 
		0b0001, 
		0b0001, 
		0b0001, 
		0b0001, 		 

		0b0010, // Right (1, 0, 0)
		0b0010,
		0b0010,
		0b0010,
		0b0010,
		0b0010,

		0b0011, // Left (-1, 0, 0)
		0b0011,
		0b0011,
		0b0011,
		0b0011,
		0b0011,

		0b0100, // Top (0, 1, 0)
		0b0100,
		0b0100,
		0b0100,
		0b0100,
		0b0100,

		0b0101, // Bottom (0, -1, 0)
		0b0101,
		0b0101,
		0b0101,
		0b0101,
		0b0101
	};

	unsigned normalsArraySize = sizeof(normalBufferData) / sizeof(unsigned int);
	m_normals.insert(m_normals.end(), &normalBufferData[0], &normalBufferData[normalsArraySize]);
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
	shader.setVec3("lightPos", glm::vec3(CHUNK_SIZE * 5, CHUNK_SIZE * 10, CHUNK_SIZE * 5));

	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size()/3);
	glBindVertexArray(0);
}
