#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <math.h>
#include <iostream>



const int steps = 100;
const float angle = 3.1415926 * 2.f / steps;

// Shaders
const GLchar* vertexShaderSrc =
"#version 330 core\n"		// 3.3版本
"in vec4 a_Position;"			// 三角形变颜色时加上的
"\n"
//"out vec3 vert_color;"					
"void main()"
"{"
//"gl_Position = a_Position;"
"gl_Position = vec4(0.0, 0.0, 0.0, 1.0);"
"gl_PointSize = 10.0;"
"}";
const GLchar* fragmentShaderSrc =
"#version 330 core\n"
"out vec4 frag_color;"
"void main()"
"{"
"frag_color = vec4(1.0, 0.0, 0.0, 1.0);"
"}";

GLint initShader()
{
	// 3. 创建vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShaderSrc, nullptr);	// 1 是count
	glCompileShader(vs);

	// Check for compile errors
	GLint result;
	GLchar infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vs, sizeof(infoLog), nullptr, infoLog);
		std::cout << "Error! Vertex shader failed to compile. " << infoLog << std::endl;
	}

	// 4. Create fragment shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShaderSrc, nullptr);	// 1表示个数
	glCompileShader(fs);

	// Check for compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fs, sizeof(infoLog), nullptr, infoLog);
		std::cout << "Error! Fragment shader failed to compile. " << infoLog << std::endl;
	}

	// 5. Create shader program and link shaders to program
	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);

	// Check for linker errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
		std::cout << "Error! Shader program linker failure " << infoLog << std::endl;
	}

	// Clean up shaders, do not need them anymore since they are linked to a shader program
	glDeleteShader(vs);
	glDeleteShader(fs);

	return shaderProgram;
}

int main() {
	GLFWwindow* window;

	if (!glfwInit()) {
		std::cout << "Init error";
		return -1;
	}

	window = glfwCreateWindow(800, 600, "Hello", 0, 0);
	if (!window) {
		std::cout << "Window creation error";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	GLint shaderProgram = initShader();
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0, 1.0, 1.0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);	// 渲染三角形
		glDrawArrays(GL_POINTS, 0, 1);  // first count

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}