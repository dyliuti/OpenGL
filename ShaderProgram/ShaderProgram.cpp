#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>

ShaderProgram::ShaderProgram()
	:mHandle(0)
{

}

ShaderProgram::~ShaderProgram() {
	// Delete the program
	glDeleteProgram(mHandle);
}

//-----------------------------------------------------------------------------
// Returns the active shader program
//-----------------------------------------------------------------------------
GLuint ShaderProgram::getProgram() const
{
	return mHandle;
}

//-----------------------------------------------------------------------------
// Sets a glm::vec2 shader uniform
//-----------------------------------------------------------------------------
void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v)
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}

//-----------------------------------------------------------------------------
// Sets a glm::vec3 shader uniform
//-----------------------------------------------------------------------------
void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v)
{
	GLint loc = getUniformLocation(name);
	glUniform3f(loc, v.x, v.y, v.z);
}

//-----------------------------------------------------------------------------
// Sets a glm::vec4 shader uniform
//-----------------------------------------------------------------------------
void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v)
{
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

//-----------------------------------------------------------------------------
// Returns the uniform identifier given it's string name. 或者是个索引
// NOTE: Shader must be currently active first.
//-----------------------------------------------------------------------------
GLint ShaderProgram::getUniformLocation(const GLchar* name)
{
	std::map<string, GLint>::iterator it = mUniformLocations.find(name);
	// Only need to query the shader program IF it doesn't already exist.
	if (it == mUniformLocations.end())
	{
		// Find it and add it to the map
		mUniformLocations[name] = glGetUniformLocation(mHandle, name);
	}

	return mUniformLocations[name];
}

//-----------------------------------------------------------------------------
// Activate the shader program
//-----------------------------------------------------------------------------
void ShaderProgram::use()
{
	if (mHandle > 0)
		glUseProgram(mHandle);	// shader program in the GPU
}

//-----------------------------------------------------------------------------
// Checks for shader compiler errors	shader改为hander更合适
//-----------------------------------------------------------------------------
void  ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type)
{
	int status = 0;

	if (type == PROGRAM)
	{
		glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);
			// 长度包括nullptr字符
			string errorLog(length, ' ');	// Resize and fill with space character
			glGetProgramInfoLog(mHandle, length, &length, &errorLog[0]);
			std::cerr << "Error! Shader program failed to link. " << errorLog << std::endl;
		}
	}
	else                   // VERTEX FRAGMENT
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			// 长度包括nullptr字符
			string errorLog(length, ' ');  // Resize and fill with space character
			glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
			std::cerr << "Error! Shader failed to compile. " << errorLog << std::endl;
		}
	}
}

//-----------------------------------------------------------------------------
// Loads vertex and fragment shaders
//-----------------------------------------------------------------------------
bool ShaderProgram::loadShaders(const char* vsFilename, const char* fsFilename)
{
	string vsString = fileToString(vsFilename);
	string fsString = fileToString(fsFilename);
	const GLchar* vsSourcePtr = vsString.c_str();
	const GLchar* fsSourcePtr = fsString.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsSourcePtr, NULL);
	glShaderSource(fs, 1, &fsSourcePtr, NULL);

	glCompileShader(vs);
	checkCompileErrors(vs, VERTEX);
	glCompileShader(fs);
	checkCompileErrors(fs, FRAGMENT);

	// 相当于是第5步了. Create shader program and link shaders to program
	mHandle = glCreateProgram();
	if (mHandle == 0)
	{
		std::cerr << "Unable to create shader program!" << std::endl;
		return false;
	}

	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, fs);

	glLinkProgram(mHandle);
	checkCompileErrors(mHandle, PROGRAM);


	glDeleteShader(vs);
	glDeleteShader(fs);
	// 注意这句新加的
	mUniformLocations.clear();

	return true;
}



//-----------------------------------------------------------------------------
// Opens and reads contents of ASCII file to a string.  Returns the string.
// Not good for very large files.
//-----------------------------------------------------------------------------
string ShaderProgram::fileToString(const string& filename)
{
	std::stringstream ss;
	std::ifstream file;

	try
	{
		file.open(filename, std::ios::in);
		if (!file.fail())
		{
			// Using a std::stringstream is easier than looping through each line of the file
			ss << file.rdbuf();
		}

		file.close();
	}
	catch (std::exception ex)
	{
		std::cerr << "Error reading shader filename!" << std::endl;
	}

	return ss.str();
}