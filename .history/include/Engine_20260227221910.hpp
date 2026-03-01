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
	Engine() {
		if (!glfwInit()) {
			throw std::runtime_error("nuh")
		}
	}
};

