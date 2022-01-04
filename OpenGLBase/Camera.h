#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera {
public:
	Camera() = default;

	Camera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up) : m_eye(std::move(eye)), m_lookAt(std::move(lookat)), m_up(std::move(up)) {
		UpdateViewMatrix();
	}

	glm::mat4x4 GetViewMatrix() const { return m_viewMatrix; }
    glm::vec3 GetEye() const { return m_eye; }
    glm::vec3 GetUpVector() const { return m_up; }
    glm::vec3 GetLookAt() const { return m_lookAt; }

    // Camera forward is -z
    glm::vec3 GetViewDir() const { return -glm::transpose(m_viewMatrix)[2]; }
    glm::vec3 GetRightVector() const { return glm::transpose(m_viewMatrix)[0]; }

	void SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up)
    {
        m_eye = std::move(eye);
        m_lookAt = std::move(lookat);
        m_up = std::move(up);
        UpdateViewMatrix();
    }

	void UpdateViewMatrix() {
		m_viewMatrix = glm::lookAt(m_eye, m_lookAt, m_up);
	}

private:
	glm::mat4x4 m_viewMatrix;
	glm::vec3 m_eye;
	glm::vec3 m_lookAt;
	glm::vec3 m_up;
};
