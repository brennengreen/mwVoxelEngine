#pragma once

#include <glad/glad.h>

#include "Voxel.h"
#include "Shader.h"
#include "Camera.h"

#include <vector>
#include <memory>
#include <cstdint>

constexpr int MAX_VOXELS = 16;

class VoxelRenderer {
public:
	VoxelRenderer();
	void Init();
	void ProcessMousePosition();
	~VoxelRenderer();

	Camera cam{};

	void Draw();
private:
	typedef std::vector<std::vector<std::vector<Voxel>>> Grid;
	std::unique_ptr<Grid> m_pVoxels;

	void m_drawVoxelAt(uint64_t x, uint64_t y, uint64_t z);

	Shader m_voxelShader;
	GLuint m_voxelVBO, m_voxelVAO;
};