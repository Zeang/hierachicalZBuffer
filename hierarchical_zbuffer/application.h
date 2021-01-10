#pragma once

#include <array>
#include <chrono>
#include <cstdlib> // exit
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "fps_camera.h"
#include "input.h"
#include "model.h"

#define SHOW_CALLBACK

class Application {
public:
	/*
	 * @brief default constructor
	 */
	Application();

	/*
	 * @brief default destructor
	 */
	virtual ~Application();

	/*
	 * @brief main loop
	 */
	void run();

	/*
	 * @brief render mode
	 */
	enum class RenderMode {
		ScanLineZBuffer,
		HierarchicalZBuffer,
		OctreeHierarchicalZBuffer
	};

private:
	/* window */
	GLFWwindow* _window = nullptr;
	std::string _windowTitle = "Hierarchical Z-Buffer";
	int _windowWidth = 1280;
	int _windowHeight = 720;
	glm::vec3 _clearColor = glm::vec3(0.0f, 0.0f, 0.0f);

	/* time */
	std::chrono::time_point<std::chrono::high_resolution_clock> _lastTimeStamp;
	double _deltaTime = 0.0f;

	/* model */
	Model _model{ "../resources/bunny.obj" };

	/* triangle data: local space */
	std::vector<Triangle> _triangles;

	///* camera */
	FpsCamera _fpsCamera{glm::radians(54.0f), 1.0 * _windowWidth / _windowHeight};

	/* input */
	KeyboardInput _keyboardInput;
	MouseInput _mouseInput;

	/* render mode */
	enum RenderMode _renderMode = RenderMode::ScanLineZBuffer;

	/*
	 * @brief update time
	 */
	void _updateTime();

	/*
	 * @brief response mouse move event
	 */
	static void _cursorMovedCallback(GLFWwindow* window, double xPos, double yPos);

	/*
	 * @brief response key press event
	 */
	static void _keyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	/*
	 * @brief handle input and update camera
	 */
	void _handleInput();

	/*
	 * @brief render frame with specified render mode
	 */
	void _renderFrame();

	// todo
	void _renderWithScanLineZBuffer();

	// todo
	void _renderWithHierarchicalZBuffer();
	
	// todo
	void _renderWithOctreeHierarchicalZBuffer();
};



