#include <glad/glad.h>


// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

// stb
#include <stb_image.h>

// std
#include <iostream>

#include "Renderer.h"
#include "Application.h"

int main() {
	
	std::cout << " -- Starting Maxwell -- " << std::endl;

	Application::Init();

	Renderer mw{};
	mw.Draw();

	Application::Terminate();

	std::cout << " -- End Maxwell Output -- " << std::endl;
	return 0;
}