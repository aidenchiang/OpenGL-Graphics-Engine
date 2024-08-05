#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

	std::string vertexSourceCode;
	std::string fragmentSourceCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	// Ensure ifstream objects can throw exceptions
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// Open files
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);

		std::stringstream vertexShaderStream;
		std::stringstream fragmentShaderStream;

		// Read file's buffer content into streams
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		// Close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// Convert streams into code
		vertexSourceCode = vertexShaderStream.str();
		fragmentSourceCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	// Convert the source code into char arrays
	const char* vertexCode = vertexSourceCode.c_str();
	const char* fragmentCode = fragmentSourceCode.c_str();

	// Compile shaders and check for errors
	unsigned int vertexShader;
	unsigned int fragmentShader;

	int vertexSuccess;
	char vertexInfoLog[512];

	int fragmentSuccess;
	char fragmentInfoLog[512];

	// Compile vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glCompileShader(vertexShader);
	// Check for vertex shader compilation errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);

	if (!vertexSuccess) {
		glGetShaderInfoLog(vertexShader, 512, NULL, vertexInfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexInfoLog << std::endl;
	}


	// Compile fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
	glCompileShader(fragmentShader);
	// Check for fragment shader compilation errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
	if (!fragmentSuccess) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentInfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentInfoLog << std::endl;
	}

	int shaderSuccess;
	char shaderInfoLog[512];
	// Create shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	// Check for shader program errors
	glGetProgramiv(ID, GL_LINK_STATUS, &shaderSuccess);
	if (!shaderSuccess) {
		glGetProgramInfoLog(ID, 512, NULL, shaderInfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << shaderInfoLog << std::endl;
	};


	// Cleanup
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// Activate the shader
void Shader::use() {
	glUseProgram(ID);
}

// Used to query a uniform location and set it's value
void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat4f(const std::string& name, float value1, float value2, float value3, float value4) const {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}

void Shader::setMatrixTransform4fv(const std::string& name, glm::mat4 matrix) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}