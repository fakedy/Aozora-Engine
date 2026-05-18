#include "Workspace.h"
#include <Context.h>
#include <Systems/AssetManager/AssetManager.h>
#include <Systems/Scene/Scene.h>
#include <Systems/SceneManager/SceneManager.h>
#include <Systems/ResourceManager/ResourceManager.h>
#include <Systems/Renderers/SceneRenderer.h>
#include <Systems/CommandQueue/CommandQueue.h>


Workspace::Workspace(Aozora::Context& context) : m_context(context)
{
	// load icons for different asset types
	m_file_3d_texture = (ImTextureID)context.resourcemanager->loadTexturePersistent("Resources/editor/file-3d.png");
	m_image_texture = (ImTextureID)context.resourcemanager->loadTexturePersistent("Resources/editor/image.png");
	m_folder_texture = (ImTextureID)context.resourcemanager->loadTexturePersistent("Resources/editor/folder.png");
	m_script_texture = (ImTextureID)context.resourcemanager->loadTexturePersistent("Resources/editor/script.png");
}

void Workspace::draw(const Aozora::Context& context)
{

	ImGui::Begin("workspace", NULL, ImGuiWindowFlags_MenuBar); // file browser

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::BeginMenu("New")) {
				if (ImGui::MenuItem("Scene")) {

				}
				if (ImGui::MenuItem("Folder")) {

				}
				if (ImGui::MenuItem("Material")) {

				}
				if (ImGui::MenuItem("Script")) {

				}
				if (ImGui::MenuItem("Skybox")) {

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Import")) {
				if (ImGui::MenuItem("Model")) {

				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}


	int thumbnailSize = 64;
	float padding = 8.0f;
	float windowWidth = ImGui::GetContentRegionAvail().x; // to figure out where we create new row
	
	int columnCount = (int)(windowWidth / (thumbnailSize + padding));
	if (columnCount < 1) columnCount = 1;
	ImGui::Columns(columnCount, 0, false);


	for (Aozora::Resources::Asset& asset : context.assetManager->getLoadedAssets()) {
		if (asset.hidden) continue;

		ImGui::PushID((int)asset.hash);
		ImGui::BeginGroup();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		switch (asset.type)
		{
		case(Aozora::Resources::AssetType::Model):
			if (ImGui::ImageButton("##btn", m_file_3d_texture, ImVec2(thumbnailSize, thumbnailSize))) {
				context.sceneManager->getCurrentActiveScene()->instantiateEntity(asset.hash, *context.resourcemanager);
			}
			break;
		case(Aozora::Resources::AssetType::Texture):
			break;
		case(Aozora::Resources::AssetType::Material):
			break;
		case(Aozora::Resources::AssetType::Scene):
			if (ImGui::ImageButton("##btn", m_image_texture, ImVec2(thumbnailSize, thumbnailSize))) {

				context.commandQueue->queueAction([&]() {
					// make sure we dont load the scene we are already on
					if (context.sceneManager->getCurrentActiveScene()->hash != asset.hash) {
						Aozora::Scene scene = context.assetManager->loadSceneFromDisk(asset.hash);
						// doing this if we load the same scene as the existing one is dumb
						context.resourcemanager->m_containerMap[scene.hash] = Aozora::ResourceManager::ResourceContainer();
						context.sceneManager->loadScene(scene);
						context.sceneRenderer->updatePrimaryScene(0); // argument doesnt matter atm
					}
					});
			}
			break;
		case(Aozora::Resources::AssetType::Skybox):
			break;
		default:
			break;
		}
		ImGui::PopStyleVar();

		ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + thumbnailSize);
		ImGui::TextUnformatted(asset.name.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndGroup();

		ImGui::PopID();
		ImGui::NextColumn();

	}
	ImGui::Columns(1);

	ImGui::End();


}
