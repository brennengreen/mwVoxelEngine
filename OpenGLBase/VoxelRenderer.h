#pragma once

#include <glad/glad.h>

#include "Chunk.h"
#include "Voxel.h"
#include "Shader.h"
#include "Camera.h"

#include <vector>
#include <memory>
#include <cstdint>

class VoxelRenderer {
public:
	VoxelRenderer();
	void Init();
	void ProcessMousePosition();
	~VoxelRenderer();

	static constexpr GLint MAX_CHUNKS = 4;

	Camera cam{};

	void Draw();
private:
	Chunk ***m_pChunks;

	Shader m_voxelShader;
	//GLuint m_voxelVBO, m_voxelVAO;
};