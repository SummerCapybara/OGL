#include <GLheaders.hpp>
#include <Shaders/Shaders.hpp>
#include <iostream>
#include <vector>
#include <Input.hpp>

__attri

struct Engine {
	GLFWwindow *window;
	int WindowWidth = 800;
	int WindowHeight = 600;
	double cursorX = 0;
	double cursorY = 0;

	static void mouse_button_callback(GLFWwindow *window, int button,
	                                  int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			printf("Left mouse button clicked!\n");
		}
	}

	static void cursorCallback(GLFWwindow *window, double xpos, double ypos) {
		Engine *engine =
		    static_cast<Engine *>(glfwGetWindowUserPointer(window));
		engine->cursorX = xpos;
		engine->cursorY = ypos;
	}

	static void FramebufferSizeCallback(GLFWwindow *window, int width,
	                                    int height) {
		Engine *engine =
		    static_cast<Engine *>(glfwGetWindowUserPointer(window));
		if (!engine) {
			std::cerr << "no engine? FlamebufferSizecalback.";
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		engine->WindowWidth = width;
		engine->WindowHeight = height;
		// std::cout << width << "x" << height << "\n";
		glViewport(0, 0, width, height);
	};

	Engine() {

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

		if (!glfwInit()) {
			std::cerr << "ur glfwInit is cooked twin\n";
			glfwTerminate();
			exit(EXIT_FAILURE);
		};

		window = glfwCreateWindow(WindowWidth, WindowHeight, "OGL", NULL, NULL);

		if (!window) {
			std::cerr << "no window?\n";
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "no glad?\n";
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
		glfwSetCursorPosCallback(window, cursorCallback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
	};

	~Engine() {
		glfwDestroyWindow(window);
		glfwTerminate();
	};
};

bool HandleInput(GLFWwindow *window, int key) {
	static std::unordered_map<int, bool> pressState;

	if (glfwGetKey(window, key) == GLFW_PRESS && !pressState[key]) {
		pressState[key] = true;
		return pressState[key];
	}

	if (glfwGetKey(window, key) == GLFW_RELEASE && pressState[key]) {
		pressState[key] = false;
		return pressState[key];
	}
	return false;
}

std::vector<glm::vec2> genCircle(int segments, float scalar = 1.0f) {
	std::vector<glm::vec2> vertices;
	vertices.push_back(glm::vec2(0.0f));

	for (int i = 0; i <= segments; i++) {
		double radians = 2 * 3.1415926535897 * i / segments;
		vertices.push_back(glm::vec2((float)cos(radians) * scalar,
		                             (float)sin(radians) * scalar));
	}

	return vertices;
}

int main() {
	Engine engine;
	Shader VS = Shader::FromFile(GL_VERTEX_SHADER, "shaders/vert.vert");
	Shader FS = Shader::FromFile(GL_FRAGMENT_SHADER, "shaders/frag.frag");
	VS.Compile();
	FS.Compile();

	ShaderProgram SHP;

	SHP.AttachShader({VS, FS});
	SHP.LinkProgram();
	std::vector<glm::vec2> vertices = genCircle(3, 100.f);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2),
	             vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2),
	                      (void *)0);
	glEnableVertexAttribArray(0);

	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
	// (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	float offset = 0.f;
	float vertOffset = 0.f;
	float speed = 1.f;

	float halfWidth = engine.WindowWidth / 2.0f;
	float halfHeight = engine.WindowHeight / 2.0f;

	glm::mat4 projection =
	    glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
	float accum = 50.0f;
	while (!glfwWindowShouldClose(engine.window)) {
		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		halfWidth = engine.WindowWidth / 2.0f;
		halfHeight = engine.WindowHeight / 2.0f;

		projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight,
		                        -1.0f, 1.0f);

		SHP.Use();
		Input input(engine.window);
		SHP.SetUniform("projection", projection);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());

		glfwSwapBuffers(engine.window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}