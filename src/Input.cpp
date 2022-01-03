#include "Input.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"

#include <iostream>

void Input::Update()
{
	for (unsigned i = 0; i < GLFW_KEY_LAST; ++i)
	{
		if (_keyStates[i] & Input::KeyState::up) _keyStates[i] = Input::KeyState::up;
		if(_keyStates[i] & Input::KeyState::down) _keyStates[i] = Input::KeyState::down;
	}

	_scrollOffset = glm::vec2(0);
	_mousePos = glm::vec2(0);
	_lastMousePos = glm::vec2(500);
	_firstMouse = false;
	_cursorEnabled = true;
	glfwPollEvents();

	std::cout << "updated input" << std::endl;
}

void Input::init_glfw_input_callbacks(GLFWwindow* window)
{
	_window = window;
	glfwSetKeyCallback(window, _process_input);
	glfwSetScrollCallback(window, _scroll_callback);
	glfwSetCursorPosCallback(window, _mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

Input::KeyState Input::GetKeyState(Key key)
{
	return _keyStates[key];
}

bool Input::IsKeyDown(Key key)
{
	return _keyStates[key] & Input::KeyState::down;
}

bool Input::IsKeyUp(Key key)
{
	return _keyStates[key] & Input::KeyState::up;
}

bool Input::IsKeyPressed(Key key)
{
	return _keyStates[key] & Input::KeyState::pressed;
}

bool Input::IsKeyReleased(Key key)
{
	return _keyStates[key] & Input::KeyState::released;
}

void Input::SetCursorEnabled(bool value)
{
	_cursorEnabled = value;
	glfwSetInputMode(_window, GLFW_CURSOR, _cursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

bool Input::IsCursorEnabled()
{
	return _cursorEnabled;
}

void Input::_mouse_callback(GLFWwindow * window, double xpos, double ypos) {
	  if (_firstMouse)
	  {
		_mouseOffset.x = static_cast<float>(xpos);
		_mouseOffset.y = static_cast<float>(ypos);
		_firstMouse = false;
	  }

	  _mousePos.x = static_cast<float>(xpos);
	  _mousePos.y = static_cast<float>(ypos);

	  _mouseOffset.x = static_cast<float>(xpos) - _lastMousePos.x;
	  _mouseOffset.y = _lastMousePos.y - static_cast<float>(ypos);
	  _lastMousePos = glm::vec2(xpos, ypos);
	  _mouseOffset *= sensitivity;
}
void Input::_scroll_callback(GLFWwindow * window, double xoffset, double yoffset) {
	//ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	_scrollOffset = glm::vec2(xoffset, yoffset);
}
void Input::_process_input(GLFWwindow* window, int key, int scancode, int action, int mods) {
	/*ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

	ImGuiIO& io = ImGui::GetIO();
	if(action == GLFW_PRESS) io.KeysDown[key] = true;
	if(action == GLFW_RELEASE) io.KeysDown[key] = false;

	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];*/

	std::cout << key << std::endl;
	if(key != GLFW_KEY_UNKNOWN)
	{
	switch(action)
	{
	case GLFW_RELEASE:
		_keyStates[key] = KeyState::released;
		break;
	case GLFW_PRESS:
		_keyStates[key] = KeyState::pressed;
		break;
	case GLFW_REPEAT:
		_keyStates[key] = KeyState::repeat;
		break;
	default:
		break;
    }
	}
}