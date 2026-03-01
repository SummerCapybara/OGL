#pragma once
#include <GLheaders.hpp>
#include <Shaders/Shaders.hpp>
#include <iostream>
#include <vector>
#include <Engine.hpp>

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

	float halfWidth = engine.WindowHeight() / 2.0f;
	float halfHeight = engine.WindowWidth() / 2.0f;

	glm::mat4 projection =
	    glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
	float accum = 50.0f;
	while (!glfwWindowShouldClose(engine.GetWindow())) {
		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		halfWidth = engine.WindowWidth() / 2.0f;
		halfHeight = engine.WindowHeight() / 2.0f;

		projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight,
		                        -1.0f, 1.0f);

		SHP.Use();
		Engine:P:
		Input input(engine.GetWindow());
		SHP.SetUniform("projection", projection);
		static int vertic = 3;
		input.AddKeybind(Keys::Equal, "key", KeyStates::Release, [&](){
			vertic++;
			vertices = genCircle(vertic);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2),
						 vertices.data(), GL_STATIC_DRAW);
						 
		});

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());

		glfwSwapBuffers(engine.GetWindow());
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}