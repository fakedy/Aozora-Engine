#pragma once
#include "Systems/Renderers/Shader.h"
#include "glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <unordered_map>

namespace Aozora {

	class OpenglShader : public Shader {
	public:
		OpenglShader(const char* vertexPath, const char* fragmentPath);


		unsigned int vertexShader;
		unsigned int shaderProgram;
		unsigned int fragmentShader;

		std::string m_vertexPath;
		std::string m_fragmentPath;

		std::unordered_map<std::string, GLint> uniformLocationMap;

		// TODO move stuff to cpp file 
		void setVec3fv(const std::string& varName, const glm::vec3& vector) override;

		void setInt(const std::string& varName, const int val) override;
		void setFloat(const std::string& varName, const float val) override;

		void setMat4(const std::string& varName, const glm::mat4 mat) override;

		void setVec4fv(const std::string& varName, const glm::vec4& vector) override;

		void recompile();



	};


}