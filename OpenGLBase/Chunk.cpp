#include "Chunk.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Chunk::Chunk()
{
	m_pVoxels = new Voxel **[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		m_pVoxels[i] = new Voxel * [CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; j++) {
			m_pVoxels[i][j] = new Voxel[CHUNK_SIZE];
		}
	}

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
    glGenBuffers(1, &m_chunkVBO);

    glBindVertexArray(m_chunkVAO);

	/*for (int i = 2; i <= m_vertices.size(); i += 3)
		std::cout << m_vertices[i-2] << " " << m_vertices[i-1] << " " << m_vertices[i] << std::endl;*/

	GLuint num_vertices = static_cast<GLsizei>(m_vertices.size());
	GLsizeiptr vertexSize = num_vertices * sizeof(GLfloat);
    glBindBuffer(GL_ARRAY_BUFFER, m_chunkVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, &m_vertices.front(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

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
