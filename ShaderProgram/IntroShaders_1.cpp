////-----------------------------------------------------------------------------
//// IntroShaders_1.cpp by Steve Jones 
//// Copyright (C) 2015-2016 Game Institute. All Rights Reserved.
////
//// - Creates and uses ShaderProgram class
//// - Colored quad bouncing around the screen
////-----------------------------------------------------------------------------
//#include <iostream>
//#include <sstream>
//#define GLEW_STATIC
//
//#include "GL/glew.h"	// Important - this header must come before glfw3 header
//#include "GLFW/glfw3.h"
//#include "glm/glm.hpp"
//
//#include "ShaderProgram.h"
//
//// Global Variables
//const char* APP_TITLE = "Introduction to Modern OpenGL - Shaders";
//const int gWindowWidth = 800;
//const int gWindowHeight = 600;
//GLFWwindow* gWindow = NULL;
//bool gWireframe = false;
//
//// Function prototypes
//void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
//void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
//void showFPS(GLFWwindow* window);
//bool initOpenGL();
//
//// 程序入口
//int main()
//{
//	if (!initOpenGL())
//	{
//		std::cerr << "GLFW initialization failed" << std::endl;
//		return -1;
//	}
//
//	// 1. Set up an array of vertices for a quad (2 triangls) with an index buffer data
//	//   4个顶点
//	GLfloat vertices[] = {
//		-0.5f,  0.5f, 0.0f,		// Top left
//		 0.5f,  0.5f, 0.0f,		// Top right
//		 0.5f, -0.5f, 0.0f,		// Bottom right
//		-0.5f, -0.5f, 0.0f		// Bottom left 
//	};
//	// 两个面
//	GLuint indices[] = {
//		0, 1, 2,  // First Triangle
//		0, 2, 3   // Second Triangle
//	};
//
//	// 2. Set up buffers on the GPU
//	GLuint vbo, ibo, vao;
//
//	glGenBuffers(1, &vbo);					// Generate an empty vertex buffer on the GPU
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);		// "bind" or set as the current buffer we are working with
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// copy the data from CPU to GPU
//	// 第一个参数 glsize 个数吧
//	glGenVertexArrays(1, &vao);				// Tell OpenGL to create new Vertex Array Object
//	glBindVertexArray(vao);					// Make it the current one
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);	// Define a layout for the first vertex buffer "0"	倒数第二个0表示stride
//	glEnableVertexAttribArray(0);			// Enable the first attribute or attribute "0"
//
//	// Set up index buffer
//	glGenBuffers(1, &ibo);	// Create buffer space on the GPU for the index buffer
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//	glBindVertexArray(0);					// 不绑定，保证其他代码不改变vertex array
//
//	ShaderProgram shaderProgram;
//	shaderProgram.loadShaders("shaders/basic_introShader.vert", "shaders/basic.frag");
//
//	// 主渲染循环
//	while (!glfwWindowShouldClose(gWindow))
//	{
//		showFPS(gWindow);
//
//		// 获得进程窗口事件
//		glfwPollEvents();
//
//		// 清除屏幕
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		// Render the quad
//		// 必须在loadShaders之后，也必须在setting uniforms之前调用，因为setting uniforms作用于当前激活的active shader program.
//		shaderProgram.use();	// mHandle是shader program，
//
//		GLfloat time = (GLfloat)glfwGetTime();
//		GLfloat blueColor = (sin(time) / 2) + 0.5f;	// -1 1  -> 0<->1
//		glm::vec2 pos;
//		pos.x = sin(time) / 2;
//		pos.y = cos(time) / 2;
//		shaderProgram.setUniform("vertColor", glm::vec4(0.0f, 0.0f, blueColor, 1.0f));	// 设置颜色
//		shaderProgram.setUniform("posOffset", pos);	// 设置位置
//
//		glBindVertexArray(vao);	// vertex array object
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0); // 6=3x2(面)	这里的0是对应layout中的0吗？
//		glBindVertexArray(0);
//
//		// 交换front and back buffers
//		glfwSwapBuffers(gWindow);
//	}
//
//	// Clean up
//	glDeleteVertexArrays(1, &vao);
//	glDeleteBuffers(1, &vbo);
//	glDeleteBuffers(1, &ibo);
//	
//	glfwTerminate();
//
//	return 0;
//}
//
////-----------------------------------------------------------------------------
//// Initialize GLFW and OpenGL
////-----------------------------------------------------------------------------
//bool initOpenGL()
//{
//	// Intialize GLFW 
//	// GLFW is configured.  Must be called before calling any GLFW functions
//	if (!glfwInit())
//	{
//		// An error occured
//		std::cerr << "GLFW initialization failed" << std::endl;
//		return false;
//	}
//
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3
//
//	// Create an OpenGL 3.3 core, forward compatible context window
//	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
//	if (gWindow == NULL)
//	{
//		std::cerr << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//		return false;
//	}
//
//	// Make the window's context the current one
//	glfwMakeContextCurrent(gWindow);
//
//	// Initialize GLEW
//	glewExperimental = GL_TRUE;
//	if (glewInit() != GLEW_OK)
//	{
//		std::cerr << "Failed to initialize GLEW" << std::endl;
//		return false;
//	}
//
//	// Set the required callback functions
//	glfwSetKeyCallback(gWindow, glfw_onKey);
//	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
//
//	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
//
//	// Define the viewport dimensions
//	glViewport(0, 0, gWindowWidth, gWindowHeight);
//
//	return true;
//}
//
////-----------------------------------------------------------------------------
//// Is called whenever a key is pressed/released via GLFW
////-----------------------------------------------------------------------------
//void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
//{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GL_TRUE);
//	
//	if (key == GLFW_KEY_W && action == GLFW_PRESS)
//	{
//		gWireframe = !gWireframe;
//		if (gWireframe)
//			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//		else
//			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	}
//}
//
////-----------------------------------------------------------------------------
//// Is called when the window is resized
////-----------------------------------------------------------------------------
//void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//}
//
////-----------------------------------------------------------------------------
//// Code computes the average frames per second, and also the average time it takes
//// to render one frame.  These stats are appended to the window caption bar.
////-----------------------------------------------------------------------------
//void showFPS(GLFWwindow* window)
//{
//	static double previousSeconds = 0.0;
//	static int frameCount = 0;
//	double elapsedSeconds;
//	double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float
//
//	elapsedSeconds = currentSeconds - previousSeconds;
//
//	// Limit text updates to 4 times per second
//	if (elapsedSeconds > 0.25)
//	{
//		previousSeconds = currentSeconds;
//		double fps = (double)frameCount / elapsedSeconds;
//		double msPerFrame = 1000.0 / fps;
//
//		// The C++ way of setting the window title
//		std::ostringstream outs;
//		outs.precision(3);	// decimal places
//		outs << std::fixed
//			<< APP_TITLE << "    "
//			<< "FPS: " << fps << "    "
//			<< "Frame Time: " << msPerFrame << " (ms)";
//		glfwSetWindowTitle(window, outs.str().c_str());
//
//		// Reset for next average.
//		frameCount = 0;
//	}
//
//	frameCount++;
//}