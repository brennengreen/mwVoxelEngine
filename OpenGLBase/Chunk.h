#pragma once

#include <glad/glad.h>

#include <vector>

#include "Voxel.h"
#include "Shader.h"
#include "MwTypes.h"

#include <thread>

class Chunk {
public:
	Chunk();
	~Chunk();
	void GenerateTerrain();
	void GenerateFromHeightMap();
	glm::vec3 SurfaceNormal(int i, int j);
	void ErodeTerrain();
	void CreateMesh();
	void AddVoxel(GLuint x, GLuint y, GLuint z, VoxelType type);
	void AsyncBuildMesh();
	void Update(float dt);
	void Draw(Shader &shader, GLuint x, GLuint y, GLuint z);
	static constexpr GLint CHUNK_SIZE = mw::CHUNK_SIZE;

private:
	Voxel ***m_pVoxelsRender;
	float ** m_pHeightMap;

	bool m_readyToRender = false;
	bool m_updating = true;
	int m_particlesToSimulate = 150000;

	std::thread worker;

	std::vector<GLfloat> m_vertices;
	std::vector<unsigned int> m_attributes;
	GLuint m_chunkVertexBuffer{0}, m_chunkAttributeBuffer{0}, m_chunkVAO{0};
};