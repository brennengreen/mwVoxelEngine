#include "Renderer.h"

#include "Input.h"
#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

// imgui
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Renderer::Renderer()
{
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		exit(1);
	}
	/*
	glViewport(0, 0, Application::GetWindowExtent().x, Application::GetWindowExtent().y);*/

	// Pipelines

	// Imgui
	/*IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(Application::GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460 core");*/
}

Renderer::~Renderer()
{
	/*ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();*/
}

void Renderer::Draw()
{
	while (!Application::ShouldTerminate()) {
		/*glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);*/

		Application::SwapBuffers();
		Application::PollEvents();
	}
}
