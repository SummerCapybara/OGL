#pragma once

#include <GLheaders.hpp>
#include <variant>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram
{
private:
	GLuint ShaderProgramID = 0;
	std::unordered_map<std::string, int> UniformCache = {};
public:
	ShaderProgram() {
		ShaderProgramID = glCreateProgram();
	};
	~ShaderProgram() {
		glDeleteProgram(ShaderProgramID);
	};

	void AttachShader(GLuint Shader) {
		glAttachShader(ShaderProgramID, Shader);
	}

	void AttachShader(std::initializer_list<GLuint> Shaders) {
		for (GLuint Shader : Shaders) {
			glAttachShader(ShaderProgramID, Shader);
		}
	}

	void AttachShader(std::initializer_list<std::reference_wrapper<Shader>> Shaders) {
		for (Shader& Shader : Shaders) {
			glAttachShader(ShaderProgramID, Shader.GetShaderID());
		}
	}


	void AttachShader(GLuint Shaders[], GLsizeiptr Size) {
		int ShaderArraySize = Size / sizeof(GLuint);
		for (int i = 0; i < ShaderArraySize; i++) {
			glAttachShader(ShaderProgramID, Shaders[i]);
		}
	}

	void LinkProgram() {
		int Success = 1;

		glLinkProgram(ShaderProgramID);
		glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &Success);

		if (!Success) {
			int LogLength = 0;
			glGetProgramiv(ShaderProgramID, GL_INFO_LOG_LENGTH, &LogLength);
			std::string LogMessage = "Shader program linking failed";

			if (LogLength > 0) {
				char* InfoLog = new char[LogLength];
				glGetProgramInfoLog(ShaderProgramID, LogLength, nullptr, InfoLog);

				LogMessage += ": ";
				LogMessage += InfoLog;

				delete[] InfoLog;
			}
			throw std::runtime_error(LogMessage);
		}
	}

	void Use() {
		glUseProgram(ShaderProgramID);
	}

	void SetUniform(const std::string& UniformName, std::variant<int, float, glm::vec2, glm::vec3,
		glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4,
		glm::mat2, glm::mat3, glm::mat4> const& value, GLint transpose = GL_FALSE) {

		int UniformLocation;
		auto it = UniformCache.find(UniformName);
		if (it != UniformCache.end()) {
			UniformLocation = it->second;
		} 
		else {
			UniformLocation = glGetUniformLocation(ShaderProgramID, UniformName.c_str());
			UniformCache[UniformName] = UniformLocation;
		}

		if (UniformLocation == -1) return;

		std::visit([UniformLocation, transpose](auto&& UniformValue) {
			using T = std::decay_t<decltype(UniformValue)>;
			if      constexpr (std::is_same_v<T, int>)        glUniform1i(UniformLocation, 									   UniformValue);
			else if constexpr (std::is_same_v<T, float>)      glUniform1f(UniformLocation, 			 						   UniformValue);
			else if constexpr (std::is_same_v<T, glm::vec2>)  glUniform2fv(UniformLocation,	      1, 			glm::value_ptr(UniformValue));
			else if constexpr (std::is_same_v<T, glm::vec3>)  glUniform3fv(UniformLocation, 	  1, 			glm::value_ptr(UniformValue));
			else if constexpr (std::is_same_v<T, glm::vec4>)  glUniform4fv(UniformLocation,		  1, 			glm::value_ptr(UniformValue));
			else if constexpr (std::is_same_v<T, glm::ivec2>) glUniform2iv(UniformLocation,	 	  1, 			glm::value_ptr(UniformValue));
			else if constexpr (std::is_same_v<T, glm::ivec3>) glUniform3iv(UniformLocation,		  1, 			glm::value_ptr(UniformValue));
			else if constexpr (std::is_same_v<T, glm::ivec4>) glUniform4iv(UniformLocation,       1, 			glm::value_ptr(UniformValue));
			else if constexpr (std::is_same_v<T, glm::mat2>)  glUniformMatrix2fv(UniformLocation, 1, transpose, glm::value_ptr(UniformValue));
			else if constexpr (std::is_same_v<T, glm::mat3>)  glUniformMatrix3fv(UniformLocation, 1, transpose, glm::value_ptr(UniformValue));
			else if constexpr (std::is_same_v<T, glm::mat4>)  glUniformMatrix4fv(UniformLocation, 1, transpose, glm::value_ptr(UniformValue));
		}, value);
	}

};