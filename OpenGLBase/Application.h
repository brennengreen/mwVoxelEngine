#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Application {
public:
	Application() = default;
	~Application() = default;

	static void Init();
	static void Terminate();

	static GLFWwindow* GetWindow();
	static glm::vec2 GetWindowExtent();
	static bool ShouldTerminate();

	static void SwapBuffers();
	static void PollEvents();

private:
	static void _init_callbacks();

	static inline GLFWwindow * _window = nullptr;
	static inline glm::vec2 _windowExtent = glm::vec2(0, 0);

	static void _glfw_error_callback(int error, const char * description);
	static void _framebuffer_size_callback(GLFWwindow * window, int width, int height);

};