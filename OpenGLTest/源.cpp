#include <iostream>
#include <sstream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

const char* APP_TITLE = "Introduction to Modern OpendGL - Hello Window 1";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow* gWindow = nullptr;
bool gFullScreen = false;

void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void showFPS(GLFWwindow* window);
bool initOpenGL();

// 当窗口被调整大小时调用
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void showFPS(GLFWwindow* window) {
	static double previosSeconds = 0.0;
	static int frameCount = 0;

	double currentSeconds = glfwGetTime();  // 返回GLFW开始的时间

	double elapsedSeconds = currentSeconds - previosSeconds;

	// 限制每秒更新4次
	if (elapsedSeconds > 0.25) {
		previosSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		std::ostringstream outs;
		outs.precision(3);
		outs << std::fixed << APP_TITLE << "  "
			<< "FPS: " << "fps" << "  "
			<< "Frame Time: " << msPerFrame << " (ms)";
		glfwSetWindowTitle(window, outs.str().c_str());

		frameCount = 0;
	}

	frameCount++;

}

bool initOpenGL() {
	if (!glfwInit()) {
		std::cerr << "GLFH initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// 大版本是3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	if (gFullScreen) {	// 全屏
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);
		if (pVmode != nullptr) {
			gWindow = glfwCreateWindow(pVmode->width, pVmode->height, APP_TITLE, pMonitor, nullptr);
		}
	}
	else {
		gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	}

	if (gWindow == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// 使窗口的上下文成为当前的上下文
	glfwMakeContextCurrent(gWindow);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {	// 初始化GLEW
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	// 设置需要的回调函数
	//glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
	// 颜色缓冲GL_COLOR_BUFFER_BIT   还有深度缓冲GL_COLOR_BUFFER_BIT 模板缓冲GL_COLOR_BUFFER_BIT
	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);	// 不需要这里前面调用一次，openGL是状态机，最后一次状态
	return true;
}

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
const GLchar * fragmentShaderSrc =
"#version 330 core\n"
"uniform vec4 u_Fragcolor;"
"void main()"
"{"
"gl_FragColor = u_Fragcolor;"
"}";

int main()
{
	if (!initOpenGL())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	
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

	GLint a_Position = glGetAttribLocation(shaderProgram, "a_Position");
	glVertexAttrib3f(a_Position, 0.0, 0.0, 0.0);
	GLint u_FragColor = glGetUniformLocation(shaderProgram, "u_FragColor");
	glUniform4f(u_FragColor, 1.0, 1.0, 1.0, 1.0);

	// 渲染主循环
	while (!glfwWindowShouldClose(gWindow)) {
		showFPS(gWindow);
		glfwPollEvents();

		// color对于GL是 0-1.0 的float
		glClear(GL_COLOR_BUFFER_BIT);	// 之前设置好的颜色缓冲 glcolorclear

		// Render the triangle
		glUseProgram(shaderProgram);	// 渲染三角形
		glPointSize(10);
		glDrawArrays(GL_POINTS, 0, 1);  // first count
		//glBindVertexArray(vao);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);

		// openGL是一个大的状态机 这里有许多状态  它维护状态直到你改变这个状态  
		// 这可以使应用双倍buffer 当需要画两个surface   前面是我们看到的  后面是我们正在绘制的
		// 将后面的转换到前面  因为绘制不是连续的才这样操作
		glfwSwapBuffers(gWindow);
	}
}