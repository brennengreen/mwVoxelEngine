#pragma once


enum VoxelType {
	 VoxelType_Default = 0,
	 VoxelType_Grass,
	 VoxelType_Dirt,
	 VoxelType_Water,
	 VoxelType_Stone,
	 VoxelType_Wood,
	 VoxelType_Sand,
	 VoxelType_NumTypes,
};

class Voxel {
public:
	Voxel();
	~Voxel() = default;
	bool IsActive();
	void SetActive(bool active);
private:
	bool m_active;
	VoxelType m_blockType;
};