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
}

VoxelRenderer::~VoxelRenderer()
{
    for (int i = 0; i < MAX_CHUNKS; ++i) {
		for (int j = 0; j < MAX_CHUNKS; ++j) {
			delete[] m_pChunks[i][j];
		}
		delete[] m_pChunks[i];
	 }
	 delete[] m_pChunks;
}

void VoxelRenderer::Init() {
    m_pChunks = new Chunk **[MAX_CHUNKS];
	for (int i = 0; i < MAX_CHUNKS; i++) {
		m_pChunks[i] = new Chunk * [MAX_CHUNKS];
		for (int j = 0; j < MAX_CHUNKS; j++) {
			m_pChunks[i][j] = new Chunk[MAX_CHUNKS];
		}
	}

    cam.SetCameraView(glm::vec3(0, 0, -(MAX_CHUNKS*Chunk::CHUNK_SIZE*1)), glm::vec3(MAX_CHUNKS*Chunk::CHUNK_SIZE/2, MAX_CHUNKS*Chunk::CHUNK_SIZE/2, MAX_CHUNKS*Chunk::CHUNK_SIZE/2), glm::vec3(0, 1,0));

	m_voxelShader = Shader("./Shaders/voxel.vert", "./Shaders/voxel.frag");

    m_voxelShader.use();
    glm::mat4 projection = glm::ortho((float)-MAX_CHUNKS*Chunk::CHUNK_SIZE*1.5f, (float)MAX_CHUNKS*Chunk::CHUNK_SIZE*1.5f, (float)-MAX_CHUNKS*Chunk::CHUNK_SIZE*1.5f, (float)MAX_CHUNKS*Chunk::CHUNK_SIZE*1.5f, 0.1f, 2500.f);
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

void VoxelRenderer::ProcessScroll(glm::vec2 offset)
{
}

void VoxelRenderer::ShadowPass()
{
}

void VoxelRenderer::Draw()
{
    glViewport(0, 0, Application::GetWindowExtent().x, Application::GetWindowExtent().y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_voxelShader.use();

    glm::mat4 view = cam.GetViewMatrix();
    m_voxelShader.setVec3("eye", cam.GetEye());
    m_voxelShader.setMat4("view", view);
	m_voxelShader.setVec3("lightPos", glm::vec3(mw::MAX_CHUNKS * mw::CHUNK_SIZE * 1.5, mw::MAX_CHUNKS * mw::CHUNK_SIZE * 1.5, mw::MAX_CHUNKS * mw::CHUNK_SIZE * 1.5));

	for (int x = 0; x < MAX_CHUNKS; x++) {
    	for (int y = 0; y < MAX_CHUNKS; y++) {
    	    for (int z = 0; z < MAX_CHUNKS; z++) {
		        m_pChunks[x][y][z].Draw(m_voxelShader, x,y,z);
                m_pChunks[x][y][z].Update(0);
            }
        }
    }
}
