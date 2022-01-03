#include "VoxelRenderer.h"

#include <iostream>

VoxelRenderer::VoxelRenderer()
{
	m_pVoxels = std::make_unique<Grid>(MAX_VOXELS, std::vector<std::vector<Voxel>>(MAX_VOXELS, std::vector<Voxel>(MAX_VOXELS, Voxel())));
}

VoxelRenderer::~VoxelRenderer()
{
}

void VoxelRenderer::Draw()
{
	for (int x = 0; x < MAX_VOXELS; x++)
	for (int y = 0; y < MAX_VOXELS; y++)
	for (int z = 0; z < MAX_VOXELS; z++)
		m_drawVoxelAt(x,y,z);
}

void VoxelRenderer::m_drawVoxelAt(uint64_t x, uint64_t y, uint64_t z)
{
	if (m_pVoxels->at(x).at(y).at(z).IsActive());
	else;
}
