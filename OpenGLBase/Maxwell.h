#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "imgui.h"
#include "VoxelRenderer.h"
#include "MwTypes.h"

#include <memory>

class Maxwell {
public:
	Maxwell();
	~Maxwell();
	void Run();
	

	void ProcessKeyboardState();
	void ProcessScrollState();
	void ProcessMousePosition();

	VoxelRenderer vr{};
private:
	void _init_pipelines();
	void _init_imgui();

	GLdouble _deltaTime {0};
	GLdouble _currentFrame {0};
	GLdouble _lastFrame {0};

	glm::vec2 _lastOffset {0, 0};

	void _shadow_pass();
	void _render_pass();
	void _imgui_pass();
};