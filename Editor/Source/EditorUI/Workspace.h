#pragma once
#include <string>
#include <vector>
#include "imgui/imgui.h"
#include <Context.h>
#include <Systems/AssetManager/AssetManager.h>

class Workspace {

public:


	// gets called too early
	Workspace(Aozora::Context& context) : m_context(context){

		m_file_3d_texture = (ImTextureID)context.resourcemanager->loadTexture(context.assetManager->createTexture("Resources/editor/file-3d.png"));
		m_image_texture = (ImTextureID)context.resourcemanager->loadTexture(context.assetManager->createTexture("Resources/editor/image.png"));
		m_folder_texture = (ImTextureID)context.resourcemanager->loadTexture(context.assetManager->createTexture("Resources/editor/folder.png"));
		m_script_texture = (ImTextureID)context.resourcemanager->loadTexture(context.assetManager->createTexture("Resources/editor/folder.png"));

		
	}



	void draw(const Aozora::Context& context);

private:

	ImTextureID m_file_3d_texture;
	ImTextureID m_image_texture;
	ImTextureID m_folder_texture;
	ImTextureID m_script_texture;

	Aozora::Context& m_context;

};