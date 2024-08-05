#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Shader {
public:
	unsigned int ID;

	// Constructor that reads in file paths for vertex and fragment shader's source codes
	Shader(const char *vertexPath, const char *fragmentPath);

	// Activate the shader
	void use();

	// Used to query a uniform location and set it's value
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setFloat4f(const std::string& name, float value1, float value2, float value3, float value4) const;
	void setMatrixTransform4fv(const std::string& name, glm::mat4 matrix) const;

};
#endif