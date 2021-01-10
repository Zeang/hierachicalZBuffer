#include "application.h"

/*
 * @brief default constructor
 */
Application::Application() {
	if (glfwInit() != GLFW_TRUE) {
		std::cerr << "init glfw failure" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	_window = glfwCreateWindow(
		_windowWidth, _windowHeight, _windowTitle.c_str(), nullptr, nullptr);

	if (_window == nullptr) {
		std::cerr << "create glfw window failure" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, this);

	glfwSetCursorPosCallback(_window, _cursorMovedCallback);
	//glfwSetMouseButtonCallback(m_window, mouseClickedCallback);
	//glfwSetScrollCallback(m_window, scrollCallback);
	//
	glfwSetKeyCallback(_window, _keyPressedCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "initialize glad failure" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::vector<Vertex> _vertices;
	std::vector<uint32_t> _indices;
	_model.getFaces(_vertices, _indices);
	for (int i = 0; i < _indices.size(); i += 3) {
		_triangles.push_back({ _vertices[i], _vertices[i + 1] , _vertices[i + 2] });
	}

	_lastTimeStamp = std::chrono::high_resolution_clock::now();
}


/*
 * @brief default destructor
 */
Application::~Application() {
	if (_window != nullptr) {
		glfwDestroyWindow(_window);
	}
	glfwTerminate();
}


/*
 * @brief main loop
 */
void Application::run() {
	while (!glfwWindowShouldClose(_window)) {
		_updateTime();
		_handleInput();
		_renderFrame();

		glfwPollEvents();
	}
}


/*
 * @brief update time
 */
void Application::_updateTime() {
	auto currentTimeStamp = std::chrono::high_resolution_clock::now();
	_deltaTime = 0.001f * std::chrono::duration<double, std::milli>().count();
	_lastTimeStamp = currentTimeStamp;
}


/*
 * @brief response mouse move event
 */
void Application::_cursorMovedCallback(GLFWwindow* window, double xPos, double yPos) {
	Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
	app->_mouseInput.move.xCurrent = xPos;
	app->_mouseInput.move.yCurrent = yPos;

#ifdef SHOW_CALLBACK
#ifndef NDEBUG
	std::cout << "cursor moved to (" << xPos << ", " << yPos << ")" << std::endl;
#endif
#endif
}


/*
 * @brief response key press event
 */
void Application::_keyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		if (key != GLFW_KEY_UNKNOWN) {
			app->_keyboardInput.keyPressed[key] = true;
		}

#ifdef SHOW_CALLBACK
#ifndef NDEBUG
		auto getKeyInfo = [](int key, int scancode) {
			std::string keyInfo;
			if (glfwGetKeyName(key, scancode)) {
				keyInfo = std::string(glfwGetKeyName(key, scancode)) + "(" + std::to_string(key) + ")";
			}
			else {
				keyInfo = std::string("unprintable key") + "(" + std::to_string(key) + ")";
			}

			return keyInfo;
		};

		std::cout << "key " << getKeyInfo(key, scancode) << " pressed" << std::endl;
		std::cout << " ------ Pressed Key Table -------" << std::endl;
		for (int i = 0; i < app->_keyboardInput.keyPressed.size(); ++i) {
			if (app->_keyboardInput.keyPressed[key]) {
				std::cout << "\t" << getKeyInfo(i, 0) << std::endl;
			}
		}
#endif
#endif
	}
}


/*
 * @brief handle input and update camera
 */
void Application::_handleInput() {
	_fpsCamera.update(_keyboardInput, _mouseInput, _deltaTime);
	
	for (auto& keyPress : _keyboardInput.keyPressed) {
		keyPress = false;
	}

	_mouseInput.move.xOld = _mouseInput.move.xCurrent;
	_mouseInput.move.yOld = _mouseInput.move.yCurrent;
}

/*
 * @brief render frame with specified render mode
 */
void Application::_renderFrame() {
	auto start = std::chrono::high_resolution_clock::now();
	switch (_renderMode) {
		case RenderMode::ScanLineZBuffer:
			_renderWithScanLineZBuffer();
			break;
		case RenderMode::HierarchicalZBuffer:
			_renderWithScanLineZBuffer();
			break;
		case RenderMode::OctreeHierarchicalZBuffer:
			_renderWithScanLineZBuffer();
			break;
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto milliseconds = std::chrono::duration<double, std::milli>().count();

	std::cout << "+ render time: " << milliseconds << " ms" << std::endl;
}

void Application::_renderWithScanLineZBuffer() {
	/* write your code here */
}

void Application::_renderWithHierarchicalZBuffer() {
	/* write your code here */
}

void Application::_renderWithOctreeHierarchicalZBuffer() {
	/* write your code here */
}

