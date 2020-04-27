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

// Shaders
const GLchar* vertexShaderSrc =
"#version 330 core\n"		// 3.3版本
"layout (location = 0) in vec3 pos;"	// 同 glVertexAttribPointer(0, ...中的第一个参数0
"layout (location = 1) in vec3 color;"	// 三角形变颜色时加上的
"\n"
"out vec3 vert_color;"					// 三角形变颜色时加上的
"void main()"
"{"
"	vert_color = color;"					// 三角形变颜色时加上的
"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"}";
const GLchar* fragmentShaderSrc =
"#version 330 core\n"
"in vec3 vert_color;"					// 三角形变颜色时加上的
"out vec4 frag_color;"
"void main()"
"{"
//"   frag_color = vec4(0.35f, 0.96f, 0.3f, 1.0f);"		//rgb alpha alpha为1是不透明的 为0就是透明的，在屏幕上不可见
"   frag_color = vec4(vert_color, 1.0f);"	// 三角形变颜色时加上的
"}";

// 函数接口
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void showFPS(GLFWwindow* window);
bool initOpenGL();

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//if (key == GLFW_KEY_A && action == GLFW_PRESS) {
	//	glfwSetWindowShouldClose(window, GL_TRUE);
	//}
}

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
	glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
	// 颜色缓冲GL_COLOR_BUFFER_BIT   还有深度缓冲GL_COLOR_BUFFER_BIT 模板缓冲GL_COLOR_BUFFER_BIT
	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);	// 不需要这里前面调用一次，openGL是状态机，最后一次状态
	return true;
}

int main()
{
	if(!initOpenGL())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	// 1.设置三角形的三个顶点 vertices就是顶点，边界点的意思
	GLfloat vertices[] = {
	// 位置				 // 颜色
	 0.0f,  0.5f, 0.0f,	 1.0f, 0.0f, 0.0f, // Top	XYZ
	 0.5f, -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f, // Right 
	-0.5f, -0.5f, 0.0f,	 0.0f, 0.0f, 1.0f, // Left
	};

	//GLfloat vert_color[] = {
	//	1.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f,
	//};

	// 2. Set up buffers on the GPU	  VERTICES ARRAY BUFFER 保持着数据  真正draw时可以不用buffer去draw，而是用这个数据结构
	GLuint vbo, vao;
	glGenBuffers(1, &vbo);					// Generate an empty vertex buffer on the GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo);		// "bind" or set as the current buffer we are working with
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// copy the data from CPU to GPU
	// GL_STATIC_DRAW  GL_DYNAMIC_DRAW  GL_STREAM_DRAW  这三种绘制方法

	// The vertex array object (VAO) is a little descriptor that defines which data from vertex buffer objects should be used as input 
	// variables to vertex shaders. in our case - use our only VBO, and say 'every three floats is a variable'
	// Modern OGL requires that we use a vertex array object
	glGenVertexArrays(1, &vao);				// Tell OpenGL to create new Vertex Array Object
	glBindVertexArray(vao);					// Make it the current one

	// Position attribute, "0"	最后两个参数就是每组有几个参数，从每组第几个参数开始算起，    第二个参数就是算几个参数
	glVertexAttribPointer(
		0,							// Attribute index, "0".  The vertex shader should have layout "0" for the position of the vertex
		3,							// Number of components of attribute "0". In this case 3 floats for x,y,z
		GL_FLOAT,					// The data type of each component
		GL_FALSE,					// Normalize component values to [-1, 1]? No, not for floating point component types
		// //从0变为6*    连续的顶点属性之间的间隔。如果传1取值方式为0123、1234、2345   如果下面还是之前的0，三个点坐标就是0.0f,  0.5f, 0.0f,	 1.0f, 0.0f, 0.0f,  0.5f, -0.5f, 0.0f,
		sizeof(GLfloat) * 6,		// Stride, number of bytes between two instances of the attribute in the buffer. This buffer is "Tightly packed" 紧凑的
		(GLvoid*)0					// Offset inside the structure to find the attribute  数据在缓冲区起始位置的偏移量。
	);				
	glEnableVertexAttribArray(0);	// Enable the first attribute or attribute "0"

	// Color attribute, identified as "1"
	glVertexAttribPointer(1, 3,	GL_FLOAT, GL_FALSE,	sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));	
	glEnableVertexAttribArray(1);	// Enable the first attribute or attribute "1"

	// 3. 创建vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShaderSrc, NULL);
	glCompileShader(vs);

	// Check for compile errors
	GLint result;
	GLchar infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Vertex shader failed to compile. " << infoLog << std::endl;
	}

	// 4. Create fragment shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShaderSrc, NULL);	// 1表示个数
	glCompileShader(fs);

	// Check for compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
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
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Shader program linker failure " << infoLog << std::endl;
	}

	// Clean up shaders, do not need them anymore since they are linked to a shader program
	glDeleteShader(vs);
	glDeleteShader(fs);

	// 主循环
	while (!glfwWindowShouldClose(gWindow)) {
		showFPS(gWindow);
		glfwPollEvents();

		// color对于GL是 0-1.0 的float
		glClear(GL_COLOR_BUFFER_BIT);	// 之前设置好的颜色缓冲 glcolorclear

		// Render the triangle
		glUseProgram(shaderProgram);	// 渲染三角形
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// openGL是一个大的状态机 这里有许多状态  它维护状态直到你改变这个状态  
		// 这可以使应用双倍buffer 当需要画两个surface   前面是我们看到的  后面是我们正在绘制的
		// 将后面的转换到前面  因为绘制不是连续的才这样操作
		glfwSwapBuffers(gWindow);
	}
	
	// 清除shader 顶点  缓存buffer
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();

    std::cout << "Hello World!\n";
}
