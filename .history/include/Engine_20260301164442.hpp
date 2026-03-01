#pragma once
#include <GLheaders.hpp>
#include <stdexcept>
#include <Input.hpp>
class Engine
{
private:
	GLFWwindow* window = nullptr;
	int windowWidth = 600;
	int windowHeight = 600;

	int mousePosX = 0;
	int mousePosY = 0;
	Input input;
public:
	static void SetFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
		
		if (!engine) {
			throw std::runtime_error("No engine found.");
		}
		engine->windowHeight = height;
		engine->windowWidth = width;
		glViewport(0, 0, width, height);
	}
	Engine() {
		if (!glfwInit()) {
			throw std::runtime_error("glfw init failed.");
		}
		window = glfwCreateWindow(windowWidth, windowHeight, "idk", nullptr, nullptr);

		if (!window) {
			throw std::runtime_error("Window is not present for some reason");
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::runtime_error("Glad failed to load.");
		}
		glfwsetwindowuserpointer
		glfwSetFramebufferSizeCallback(window, SetFramebufferSizeCallback);
		glfwSetKeyCallback(window, Input::InputKeyCallback);
	}
	~Engine() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	int WindowWidth() {
		return windowWidth;
	}
	int WindowHeight() {
		return windowHeight;
	}
	int MousePosX() {
		return mousePosX;
	}
	int MousePosY() {
		return mousePosY;
	}
	GLFWwindow* GetWindow() {
		return window;
	}
};

