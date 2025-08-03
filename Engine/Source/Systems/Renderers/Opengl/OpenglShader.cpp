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

}
