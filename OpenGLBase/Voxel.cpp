#include "Voxel.h"

#include <iostream>

Voxel::Voxel()
{
	double r = ((double) rand() / (RAND_MAX));
	m_active = r > 0.99 ? true : false;
	m_blockType = VoxelType_Default;
}

bool Voxel::IsActive()
{
	return m_active;
}

void Voxel::SetActive(const bool active)
{
	m_active = active;
}
