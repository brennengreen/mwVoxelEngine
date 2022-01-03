#include "Voxel.h"

#include <iostream>

Voxel::Voxel()
{
	m_active = true;
	m_blockType = VoxelType_Default;
	std::cout << "init" << std::endl;
}

bool Voxel::IsActive()
{
	return m_active;
}

void Voxel::SetActive(const bool active)
{
	m_active = active;
}
