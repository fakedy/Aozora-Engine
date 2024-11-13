#pragma once
#include "Systems/Renderers/Shader.h"
#include "glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


namespace Aozora {

	class OpenglShader : public Shader {
	public:
		OpenglShader(const char* vertexPath, const char* fragmentPath);


		unsigned int vertexShader;
		unsigned int shaderProgram;
		unsigned int fragmentShader;



	};


}