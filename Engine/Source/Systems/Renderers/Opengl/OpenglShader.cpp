#include "OpenglShader.h"



Aozora::OpenglShader::OpenglShader(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	
	std::string vertexCode;
	std::string fragmentCode;

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);
		std::stringstream vertexShaderStream, fragmentShaderStream;
		vertexShaderStream << vertexFile.rdbuf();
		fragmentShaderStream << fragmentFile.rdbuf();
		vertexFile.close();
		fragmentFile.close();

		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();

	}
	catch (std::ifstream::failure& e) {
		std::cout << "FAILED TO READ SHADER" << std::endl;
		std::cout << e.what() << std::endl;
	}


	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();


	// create our shaders
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// specify the shader source
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// compile shaders
	glCompileShader(vertexShader);

	int success;
	char info[512];

	// error checking
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(vertexShader, 512, NULL, info);
	if (!success) {
		std::cout << vertexPath <<  "Vertex shader compilation failed" << info << std::endl;
	}

	glCompileShader(fragmentShader);

	// error checking
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(fragmentShader, 512, NULL, info);
	if (!success) {
		std::cout << fragmentPath << "Fragment shader compilation failed" << info << std::endl;
	}

	// create the shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glUseProgram(ID);

	// delete the shaders as we have the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	m_vertexPath = vertexPath;
	m_fragmentPath = fragmentPath;

}

void Aozora::OpenglShader::setVec3fv(const std::string& varName, const glm::vec3& vector)
{
	GLint location;
	auto it = uniformLocationMap.find(varName);
	// if its cached
	if (it != uniformLocationMap.end()) {
		location = it->second;
	}
	else {
		// cache it
		location = glGetUniformLocation(ID, varName.c_str());
		uniformLocationMap[varName] = location;
	}
	glUniform3fv(uniformLocationMap[varName], 1, &vector[0]);
}

void Aozora::OpenglShader::setInt(const std::string& varName, const int val)
{
	GLint location;
	auto it = uniformLocationMap.find(varName);
	// if its cached
	if (it != uniformLocationMap.end()) {
		location = it->second;
	}
	else {
		// cache it
		location = glGetUniformLocation(ID, varName.c_str());
		uniformLocationMap[varName] = location;
	}
	glUniform1i(uniformLocationMap[varName], val);
}

void Aozora::OpenglShader::setFloat(const std::string& varName, const float val)
{
	GLint location;
	auto it = uniformLocationMap.find(varName);
	// if its cached
	if (it != uniformLocationMap.end()) {
		location = it->second;
	}
	else {
		// cache it
		location = glGetUniformLocation(ID, varName.c_str());
		uniformLocationMap[varName] = location;
	}
	glUniform1f(uniformLocationMap[varName], val);
}

void Aozora::OpenglShader::setMat4(const std::string& varName, const glm::mat4 mat)
{
	GLint location;
	auto it = uniformLocationMap.find(varName);
	// if its cached
	if (it != uniformLocationMap.end()) {
		location = it->second;
	}
	else {
		// cache it
		location = glGetUniformLocation(ID, varName.c_str());
		uniformLocationMap[varName] = location;
	}
	glUniformMatrix4fv(uniformLocationMap[varName], 1, GL_FALSE, &mat[0][0]);
}

void Aozora::OpenglShader::setVec4fv(const std::string& varName, const glm::vec4& vector)
{
	GLint location;
	auto it = uniformLocationMap.find(varName);
	// if its cached
	if (it != uniformLocationMap.end()) {
		location = it->second;
	}
	else {
		// cache it
		location = glGetUniformLocation(ID, varName.c_str());
		uniformLocationMap[varName] = location;
	}
	glUniform4fv(uniformLocationMap[varName], 1, &vector[0]);

}

void Aozora::OpenglShader::recompile()
{

	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	std::string vertexCode;
	std::string fragmentCode;

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertexFile.open(m_vertexPath);
		fragmentFile.open(m_fragmentPath);
		std::stringstream vertexShaderStream, fragmentShaderStream;
		vertexShaderStream << vertexFile.rdbuf();
		fragmentShaderStream << fragmentFile.rdbuf();
		vertexFile.close();
		fragmentFile.close();

		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();

	}
	catch (std::ifstream::failure& e) {
		std::cout << "FAILED TO READ SHADER" << std::endl;
		std::cout << e.what() << std::endl;
	}


	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();


	// create our shaders
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// specify the shader source
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// compile shaders
	glCompileShader(vertexShader);

	int success;
	char info[512];

	// error checking
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(vertexShader, 512, NULL, info);
	if (!success) {
		std::cout << m_vertexPath << "Vertex shader compilation failed" << info << std::endl;
	}

	glCompileShader(fragmentShader);

	// error checking
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(fragmentShader, 512, NULL, info);
	if (!success) {
		std::cout << m_fragmentPath << "Fragment shader compilation failed" << info << std::endl;
	}

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// delete the shaders as we have the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}
