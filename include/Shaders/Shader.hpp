#pragma once

#include <GLheaders.hpp>
#include <fstream>
#include <string>
#include <stdexcept>
#include <iterator>

std::string FileToString(const std::string& FileName) {
    std::ifstream file(FileName);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file: " + FileName);
    
    return std::string(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>());
}

class Shader {

private:
	std::string ShaderSource = "";
	GLuint ShaderID = 0;
	GLenum ShaderType = 0;
public:
	Shader() = default;

	Shader(GLuint Type, const std::string& Source)
		: ShaderType(Type), ShaderSource(Source) {
		
		ShaderID = glCreateShader(ShaderType);
	}

	static Shader FromFile(GLuint ShaderType, const std::string& FilePath) {
		std::string ShaderSource = FileToString(FilePath);
		return Shader(ShaderType, ShaderSource);
	}

	GLuint GetShaderID() {
		return ShaderID;
	}
	std::string GetShaderSource() {
		return ShaderSource;
	}
	GLuint GetShaderType() {
		return ShaderType;
	}

	void Compile() {
		int Success = 1;
		const GLchar* src = ShaderSource.c_str();

		glShaderSource(ShaderID, 1, &src, nullptr);
		glCompileShader(ShaderID);
		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Success);

		if (!Success) {
			int LogLength = 0;
			glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &LogLength);
			std::string LogMessage = std::to_string(ShaderType) + " Shader compilation failed";
			if (LogLength > 0) {
				char* InfoLog = new char[LogLength];
				glGetShaderInfoLog(ShaderID, LogLength, nullptr, InfoLog);

				LogMessage += ": ";
				LogMessage += InfoLog;

				delete[] InfoLog;
			}
			throw std::runtime_error(LogMessage);
		}
	}
 
	~Shader() {
		glDeleteShader(ShaderID);
	}
};
