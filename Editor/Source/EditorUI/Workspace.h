#pragma once
#include <string>
#include <vector>
#include "AozoraAPI/Aozora.h"
#include "imgui/imgui.h"

class Workspace {

public:

	class Asset {

		enum class AssetType {
			Texture,
			Model,
			Scene,
			Material,
			Folder,
			Script
		};

		AssetType type;
		std::string name;
		uint32_t textureID; // for future custom thumbnails etc
	};

	Workspace() {
		// I added another storage in resourcemanager to store "persistent" textures that should stay there until shutdown.
		// I do not know yet if it was wise, but we will eventually find out.
		m_file_3d_texture = (ImTextureID)(intptr_t)Aozora::ResourcesAPI::loadTexture("Resources/editor/file-3d.png", true);
		m_image_texture = (ImTextureID)(intptr_t)Aozora::ResourcesAPI::loadTexture("Resources/editor/image.png", true);
		m_folder_texture = (ImTextureID)(intptr_t)Aozora::ResourcesAPI::loadTexture("Resources/editor/folder.png", true);
		m_script_texture = (ImTextureID)(intptr_t)Aozora::ResourcesAPI::loadTexture("Resources/editor/folder.png", true); // TEMP it should be script
	}



	void draw();

private:

	ImTextureID m_file_3d_texture;
	ImTextureID m_image_texture;
	ImTextureID m_folder_texture;
	ImTextureID m_script_texture;


};