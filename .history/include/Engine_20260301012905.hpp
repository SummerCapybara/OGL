#include <GLheaders.hpp>
#include <stdexcept>

class Engine
{
private:
	GLFWwindow* window = nullptr;
	int WindowWidth = 600;
	int WindowHeight = 600;

	int MousePosX = 0;
	int MousePosY = 0;
public:
	static void glfwSetFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
		
		if (!engine) {
			throw std::runtime_error("No engine found.");
		}
		glViewport()
	}
	Engine() {
		if (!glfwInit()) {
			throw std::runtime_error("glfw init failed.");
		}
		window = glfwCreateWindow(WindowWidth, WindowHeight, "idk", nullptr, nullptr);

		if (!window) {
			throw std::runtime_error("Window is not present for some reason");
		}

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::runtime_error("Glad failed to load.");
		}
	}
};

