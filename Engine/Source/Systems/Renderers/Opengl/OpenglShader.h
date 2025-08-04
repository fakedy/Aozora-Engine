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


		std::unordered_map<std::string, GLint> uniformLocationMap;

		// TODO move stuff to cpp file 
		void setVec3fv(const std::string& varName, const glm::vec3& vector) override {
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

		void setInt(const std::string& varName, const int val) override {
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
		void setFloat(const std::string& varName, const float val) override {
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

		void setMat4(const std::string& varName, const glm::mat4 mat) override {
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

		void setVec4fv(const std::string& varName, const glm::vec4& vector) override {
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



	};


}