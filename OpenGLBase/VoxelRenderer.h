#pragma once

#include <glad/glad.h>

#include "Chunk.h"
#include "Voxel.h"
#include "Shader.h"
#include "Camera.h"
#include "MwTypes.h"

#include <vector>
#include <memory>
#include <cstdint>

class VoxelRenderer {
public:
	VoxelRenderer();
	void Init();
	void ProcessMousePosition();
	void ProcessScroll(glm::vec2 offset);
	~VoxelRenderer();

	static constexpr GLint MAX_CHUNKS = mw::MAX_CHUNKS;

	Camera cam{};

	void ShadowPass();
	void Draw();
private:
	Chunk ***m_pChunks;

	Shader m_voxelShader;
};