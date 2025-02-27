#pragma once
#include <memory>
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
		glm::vec3 getPos() const {
			return m_pos;
		}

		void setPos(glm::vec3 pos) {
			m_pos = pos;
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

		int m_viewPortX{ 1920 };
		int m_viewPortY{ 1080 };
		float m_fovY{ 45.0f };

	private:
		glm::mat4 m_proj = glm::mat4(1.0f);
		glm::mat4 m_view = glm::mat4(1.0f);
		glm::vec3 m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 m_forward = glm::vec3(0.0f, 0.0f, -1.0f);



		bool perspective{ true };

		bool m_isActive{ true };
		// I want to move this to implementation specific stuff aka defined in user scripts
		float lastX = 1920 / 2.0f;
		float lastY = 1080 / 2.0f;

		float yaw = -90.0f; // rotate
		float pitch = 0.0f; // up and down

		float sensitivity = 0.3f;

	};


}