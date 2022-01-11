#include "Voxel.h"

#include <iostream>

Voxel::Voxel()
{
	m_active = false;
	m_blockType = VoxelType_Default;
}

bool Voxel::IsActive() const
{
	return m_active;
}

void Voxel::SetActive(const bool active)
{
	m_active = active;
}

VoxelType Voxel::GetVoxelType() const
{
	return m_blockType;
}

void Voxel::SetVoxelType(const VoxelType type)
{
	m_blockType = type;
}
