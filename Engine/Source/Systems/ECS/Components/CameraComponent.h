#pragma once
#include <memory>
#include <cereal/cereal.hpp>
#include <Systems/Serialization/SerializationGLM.h>
namespace Aozora {

	struct CameraComponent {


		glm::mat4 getProjection() const {
			return m_proj;
		}

		void setProjection(glm::mat4 proj) {
			m_proj = proj;
		}

		glm::mat4 getView() const {
			return m_view;
		}

		void setView(glm::mat4 view) {
			m_view = view;
		}

		float getFov() const {
			return m_fovY;
		}

		void setFov(float fovY) {
			m_fovY = fovY;
		}

		glm::vec3 getForwardVector() const{
			return m_forward;
		}

		glm::vec3 getUpVector() const {
			return m_up;
		}

		bool isActive() {
			return m_isActive;
		}

		void setActive(bool state) {
			m_isActive = state;
		}

		int m_viewPortWidth{ 1920 };
		int m_viewPortHeight{ 1080 };
		float m_fovY{ 45.0f };

		glm::mat4 m_proj = glm::mat4(1.0f);
		glm::mat4 m_view = glm::mat4(1.0f);
		glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 m_forward = glm::vec3(0.0f, 0.0f, -1.0f);



		bool perspective{ true };

		bool m_isActive{ true };

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(m_viewPortWidth),
				CEREAL_NVP(m_viewPortHeight),
				CEREAL_NVP(m_fovY),
				CEREAL_NVP(m_proj),
				CEREAL_NVP(m_view),
				CEREAL_NVP(m_up),
				CEREAL_NVP(m_right),
				CEREAL_NVP(m_forward),
				CEREAL_NVP(perspective),
				CEREAL_NVP(m_isActive));
		}

	private:

	};
	

}