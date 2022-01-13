#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "MwTypes.h"

#include "Application.h"

class ArcballCamera {
public:
	ArcballCamera() = default;

	ArcballCamera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up) : m_eye(std::move(eye)), m_lookAt(std::move(lookat)), m_up(std::move(up)) {
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

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.f;
const float PITCH = 0.0f;
const float SPEED = 0.005f;
const float SENSITIVITY = 7.f;
const float ZOOM = 45.0f;

class FlyCamera {
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	FlyCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        _update_camera_vectors();
    }
    FlyCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        _update_camera_vectors();
    }

	glm::mat4 GetViewMatrix() {
	    return glm::lookAt(Position, Position + Front, Up);
	}
	glm::mat4 GetProjectionMatrix() {
		return glm::perspective(glm::radians(Zoom), (float)Application::GetWindowExtent().x / (float)Application::GetWindowExtent().y, 0.1f, 5000.0f);
	}

	glm::vec3 GetEye() {
		return Position;
	}

	void ProcessKeyboard(CameraMovement direction, float deltaTime) {
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD) {
			Position += Front * velocity;
		}
		if (direction == BACKWARD) {
			Position -= Front * velocity;
		}
		if (direction == LEFT) {
			Position -= Right * velocity;
		}
		if (direction == RIGHT) {
			Position += Right * velocity;
		}
	}
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
		Yaw   += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		_update_camera_vectors();
	}
	void ProcessMouseScroll(float yoffset) {
		Zoom -= static_cast<float>(yoffset);
		Zoom = (Zoom < 1.0f) ? 1.0f : Zoom;
		Zoom = (Zoom > 45.0f) ? 45.0f : Zoom;
	}
private:
	void _update_camera_vectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up    = glm::normalize(glm::cross(Right, Front));
	}
};