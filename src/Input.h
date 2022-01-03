#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#pragma warning(disable: 26812)
#pragma optimize("", off)

using Key = int;

class Input {
public:
	enum KeyState : uint8_t {
		down     = 0b00001,
		pressed  = 0b00011,
		up       = 0b00100,
		released = 0b01100,
		repeat   = 0b10001
	};

	static void Update();

	static void init_glfw_input_callbacks(GLFWwindow* window);

	static inline float sensitivity = 3.1415f / 50.0f;

	static bool DidMouseUpdate() { return (_lastMousePos != _mousePos);}
	static glm::vec2 GetScrollOffset() { return _scrollOffset; }
	static glm::vec2 GetMousePos() { return _mousePos; }
	static glm::vec2 GetMouseOffset() { return _mouseOffset; }


	static KeyState GetKeyState(Key key);
	static bool IsKeyDown(Key key);
	static bool IsKeyUp(Key key);
	static bool IsKeyPressed(Key key);
	static bool IsKeyReleased(Key key);

	static void SetCursorEnabled(bool value);
	static bool IsCursorEnabled();

	static inline GLFWwindow* _window;
private:
	// Input Buffers
	static inline bool _firstMouse;
	static inline glm::vec2 _scrollOffset;
	static inline glm::vec2 _mousePos;
	static inline glm::vec2 _lastMousePos;
	static inline glm::vec2 _mouseOffset;
	static inline bool _cursorEnabled;

	static inline KeyState _keyStates[GLFW_KEY_LAST] = {KeyState(0)};

	static void _mouse_callback(GLFWwindow * window, double xpos, double ypos);
	static void _scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
	static void _process_input(GLFWwindow* window, int key, int scancode, int action, int mods);
};