#pragma once

#include <glad/glad.h>

#include "Voxel.h"

#include <vector>
#include <memory>
#include <cstdint>

constexpr int MAX_VOXELS = 32;

class VoxelRenderer {
public:
	VoxelRenderer();
	~VoxelRenderer();

	void Draw();
private:
	typedef std::vector<std::vector<std::vector<Voxel>>> Grid;
	std::unique_ptr<Grid> m_pVoxels;

	void m_drawVoxelAt(uint64_t x, uint64_t y, uint64_t z);
};