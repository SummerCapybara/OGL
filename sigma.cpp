#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>



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

struct Shader {
    int success;
    char InfoLog[512];

    char* FileToChrP(const std::string& FileName) {
        std::ifstream file(FileName, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            throw std::runtime_error("Your file is cooked twin | FileToChrP\n");
        }

        std::streamsize size = file.tellg();
        if (size < 0) throw std::runtime_error("Ur file is cooked twin | FileToChrP\n");
        file.seekg(0, std::ios::beg);

        char* buffer = new char[size + 1];

        file.read(buffer, size);
        buffer[size] = '\0';

        return buffer;
    };

    GLuint CompileShader(const char* ShaderSource, GLenum type) {
        GLuint Shader = glCreateShader(type);
        glShaderSource(Shader, 1, &ShaderSource, NULL);
        glCompileShader(Shader);
        delete[] ShaderSource;
        glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(Shader, 512, NULL, InfoLog);
            std::cout << "YOUR SHADER IS COOKED TWIN :SKULL:\n" << InfoLog;
        }
        return Shader;
    };

    GLuint initializeShaderProgram(std::initializer_list<GLuint> Shaders, bool ShouldOutput) {

        GLuint ShaderProgram = glCreateProgram();
        
        for (GLuint Shader : Shaders) {
            glAttachShader(ShaderProgram, Shader);
            if (ShouldOutput) {
                std::cout << "Shader has been attached\n";
            }
        }
        glLinkProgram(ShaderProgram);
        glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
            std::cerr << "Your shader program is so cooked lil bro\n" << InfoLog; 
        }

        for (GLuint Shader : Shaders) {
            glDeleteShader(Shader);
            std::cout << "Shader has been deleted\n";
        }

        return ShaderProgram;
    };

};

int main() {
    Engine engine;
    Shader shader;
    GLuint vertShader = shader.CompileShader(shader.FileToChrP("shaders/vert.vert"), GL_VERTEX_SHADER);
    GLuint fragShader = shader.CompileShader(shader.FileToChrP("shaders/frag.frag"), GL_FRAGMENT_SHADER);

    GLuint ShaderProgram = shader.initializeShaderProgram({vertShader, fragShader}, true);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
        0.5f, -0.5f, 0.0f, // right 
        0.0f,  0.5f, 0.0f  // top   
    }; 

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
    
    while (!glfwWindowShouldClose(engine.window)) {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(ShaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(engine.window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(ShaderProgram);

    glfwTerminate();
    return 0;
} 