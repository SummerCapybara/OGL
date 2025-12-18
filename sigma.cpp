#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

char* FileToChrP(std::string FileName) {
    std::ifstream file(FileName, std::ios::binary);
    if (!file.is_open()) std::cerr << "ur file is cooked twin | FileToChrP\n";

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size + 1];

    file.read(buffer, size);
    buffer[size] = '\0';

    return buffer;
}

struct Engine {
    GLFWwindow* window;
    int WindowWidth = 800;
    int WindowHeight = 600;
    
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
        Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
        if (!engine) {
            std::cerr << "no engine? FlamebufferSizecalback.";
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        engine -> WindowWidth = width;
        engine -> WindowHeight = height;
        // std::cout << width << "x" << height << "\n";
        glViewport(0, 0, width, height);
    };

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

        if (!engine) {
            std::cerr << "no engine? KeyCallback.";
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        if (action == GLFW_PRESS) {
            std::cout << key << "\n";
        }
    };

    static void MouseCallback(GLFWwindow* window, int button, int action, int mods) {
        Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
        
        if (!engine) {
            std::cerr << "no engine? MouseCallback.";
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        std::cout << "Mouse button: " << button << " action: " << action << std::endl;
    };

    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
        Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
        
        if (!engine) {
            std::cerr << "no engine? CursorPosCallback.";
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
    }

    Engine() {

        // HINTS
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
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetMouseButtonCallback(window, MouseCallback);
        glfwSetCursorPosCallback(window, CursorPosCallback);  
    };

        ~Engine() {
            glfwDestroyWindow(window);
            glfwTerminate();
        };
};


int main() {
    Engine engine;
    while (!glfwWindowShouldClose(engine.window)) {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        

        glfwSwapBuffers(engine.window);
        glfwPollEvents();
    }

}