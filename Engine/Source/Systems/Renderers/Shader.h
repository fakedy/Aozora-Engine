#pragma once
#include <string>
#include <glm/glm.hpp>
namespace Aozora {

	class Shader {


	public:

		unsigned int ID{};

		virtual void setVec3fv(const std::string& varName, const glm::vec3& vector) = 0;
		virtual void setInt(const std::string& varName, const int val) = 0;
		virtual void setFloat(const std::string& varName, const float val) = 0;
		virtual void setMat4(const std::string& varName, const glm::mat4 mat) = 0;
		virtual void setVec4fv(const std::string& varName, const glm::vec4& vector) = 0;
	private:



	};
}