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
	void ProcessKeyboardState();
	void ProcessMousePosition();
	void ProcessScroll(glm::vec2 offset);
	~VoxelRenderer();

	static constexpr GLint MAX_CHUNKS = mw::MAX_CHUNKS;

	ArcballCamera ACam{};
	FlyCamera FCam{};

	void ShadowPass();
	void Draw();
private:
	Chunk ***m_pChunks;

	Shader m_voxelShader;

	unsigned int m_currentFrame = 0, m_lastFrame = 0, m_deltaTime = 0;
};