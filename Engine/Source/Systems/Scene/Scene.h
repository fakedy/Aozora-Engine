#pragma once
#include <entt/entt.hpp>
#include <memory>
#include "Systems/Renderers/Opengl/OpenglShader.h"
#include "..\ResourceManager\ResourceManager.h"
#include <string>
#include <Systems/ECS/Components/Components.h>



namespace Aozora {

	class outputArchive {
	public:

		template <typename T>
		void operator()(const T& value) {
			data.insert(data.end(), reinterpret_cast<const char*>(&value), reinterpret_cast<const char*>(&value) + sizeof(T));
		}

		void operator()(const NameComponent& value) {
			(*this)(value.name);
		}
		void operator()(const TagComponent& value) {
			(*this)(value.tag);
		}
		void operator()(const RelationComponent& value) {
			(*this)(value.children);
		}

		void operator()(const std::string& value) {

			// save string length

			size_t len = value.length();

			(*this)(len); // calls itself with data len to save the length POD(plain old data)

			// save the actual string

			data.insert(data.end(), value.data(), value.data() + len);

		}

		template <typename T>
		void operator()(const std::vector<T>& vec) {
			size_t size = vec.size();
			(*this)(size);
			// because if the element is a non POD we need to do this
			for (const T& element : vec) {
				(*this)(element);
			}
		}

		std::vector<entt::entity> children;

		std::vector<char> data;
	};


	class inputArchive {
	public:
		inputArchive(const std::vector<char>& data) : data(data), pos(0) {}
		template <typename T>
		void operator()(T& value){
			if (pos + sizeof(T) <= data.size()) { // preventing reading beyond buffer
				std::memcpy(&value, data.data() + pos, sizeof(T));
				pos += sizeof(T);
			}
		}

		void operator()(NameComponent& value) {
			(*this)(value.name);
		}
		void operator()(TagComponent& value) {
			(*this)(value.tag);
		}
		void operator()(RelationComponent& value) {
			(*this)(value.children);
		}
		void operator()(std::string& value) {

			size_t len;

			(*this)(len);


			if (pos + len <= data.size()) { // preventing reading beyond buffer

				value.assign(data.data() + pos, len);

				pos += len;

			}

		}
		template <typename T>
		void operator()(std::vector<T>& vec) {
			size_t size;
			(*this)(size);
			if (pos + size <= data.size()) {
				vec.resize(size);
				for (T& element : vec) {
					(*this)(element);
				}
			}

		}


	private:

		const std::vector<char>& data;
		size_t pos;

	};


	class Scene {
	public:

		int m_gameViewPortX{ 1920 };
		int m_gameViewPortY{ 1080 };
	
		Scene();

		// ECS SYSTEM
		std::shared_ptr<entt::registry> m_registry;

		std::vector<char> m_snapshotData;

		const char* m_sceneName{ "New Scene" };

		void update();

		entt::registry& getRegistry();

		void takeSnapshot();
		void loadSnapShot();


	private:

		void updateTransform(entt::entity entity, const glm::mat4& model);

		
	};
}
