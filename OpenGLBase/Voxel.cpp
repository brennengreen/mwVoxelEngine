#include "Voxel.h"

#include <iostream>

Voxel::Voxel()
{
	m_active = false;
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
