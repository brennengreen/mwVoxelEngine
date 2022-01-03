#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <imgui.h>

class Renderer {
public:
	Renderer();
	~Renderer();

	void Draw();
};