#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "VoxelRenderer.h"

#include "Input.h"
#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

VoxelRenderer::VoxelRenderer()
{
	m_pVoxels = std::make_unique<Grid>(MAX_VOXELS, std::vector<std::vector<Voxel>>(MAX_VOXELS, std::vector<Voxel>(MAX_VOXELS, Voxel())));
}

void VoxelRenderer::Init() {
    cam.SetCameraView(glm::vec3(-MAX_VOXELS/2, -MAX_VOXELS/2, -3.0f), glm::vec3(MAX_VOXELS/2, MAX_VOXELS/2, MAX_VOXELS/2), glm::vec3(0, 1,0));

	m_voxelShader = Shader("Shaders/voxel.vert", "Shaders/voxel.frag");

	float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    glGenVertexArrays(1, &m_voxelVAO);
    glGenBuffers(1, &m_voxelVBO);

    glBindVertexArray(m_voxelVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_voxelVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_voxelShader.use();
    glm::mat4 projection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.001f, 1000.0f);
    m_voxelShader.setMat4("projection", projection);

    glEnable(GL_DEPTH_TEST);
}

void VoxelRenderer::ProcessMousePosition() {
    if (Input::IsMousePressed(0)) {
        glm::vec2 mousePos = Input::GetMousePos();
        glm::vec3 up =  glm::vec3(0, 1, 0);

        glm::vec4 position(cam.GetEye().x, cam.GetEye().y, cam.GetEye().z, 1);
        glm::vec4 pivot(cam.GetLookAt().x, cam.GetLookAt().y, cam.GetLookAt().z, 1);

        float deltaAngleX = (2 * glm::pi<double>() / Application::GetWindowExtent().x); 
        float deltaAngleY = (glm::pi<double>() / Application::GetWindowExtent().y);
        float xAngle = Input::GetMouseOffset().x * deltaAngleX;
        float yAngle = Input::GetMouseOffset().y * deltaAngleY;

        // step 2: Rotate the camera around the pivot point on the first axis.
        glm::mat4x4 rotationMatrixX(1.0f);
        rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, up);
        position = (rotationMatrixX * (position - pivot)) + pivot;

        // step 3: Rotate the camera around the pivot point on the second axis.
        glm::mat4x4 rotationMatrixY(1.0f);
        rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, cam.GetRightVector());
        glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

        // Update the camera view (we keep the same lookat and the same up vector)
        cam.SetCameraView(finalPosition, cam.GetLookAt(), up);
    }
}
VoxelRenderer::~VoxelRenderer()
{
}

void VoxelRenderer::Draw()
{
    m_voxelShader.use();
    glBindVertexArray(m_voxelVAO);

    glm::mat4 view = cam.GetViewMatrix();
    m_voxelShader.setMat4("view", view);

	for (int x = 0; x < MAX_VOXELS; x++)
	for (int y = 0; y < MAX_VOXELS; y++)
	for (int z = 0; z < MAX_VOXELS; z++)
		m_drawVoxelAt(x,y,z);
}

void VoxelRenderer::m_drawVoxelAt(uint64_t x, uint64_t y, uint64_t z)
{
	if (m_pVoxels->at(x).at(y).at(z).IsActive()) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x, y, z));
       // model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0, 0.3, 0.0));
        m_voxelShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    } else {
        ;
    }
}
