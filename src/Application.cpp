#include "Application.h"

#include "Input.h"

#include <iostream>

void Application::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_windowExtent = glm::vec2(1920, 1080);
	_window = glfwCreateWindow(_windowExtent.x, _windowExtent.y, "Maxwell", NULL, NULL);
	if (!_window) {
		std::cerr << "Failed to create GLFW Window!" << std::endl;
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(_window);

	_init_callbacks();

	Input::Update();
}

void Application::Terminate()
{
	glfwTerminate();
}

GLFWwindow* Application::GetWindow()
{
	return _window;
}

glm::vec2 Application::GetWindowExtent()
{
	return _windowExtent;
}

bool Application::ShouldTerminate()
{
	return glfwWindowShouldClose(_window);
}

void Application::SwapBuffers()
{
	glfwSwapBuffers(_window);
}

void Application::PollEvents()
{
	glfwPollEvents();
}

void Application::_init_callbacks()
{
	glfwSetErrorCallback(_glfw_error_callback);
	glfwSetFramebufferSizeCallback(_window, _framebuffer_size_callback);
	Input::init_glfw_input_callbacks(_window);
}

void Application::_glfw_error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << error << ": " << description << std::endl;
}

void Application::_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0,0,width,height);
}
