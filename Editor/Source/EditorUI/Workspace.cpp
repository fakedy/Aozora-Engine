#include "Workspace.h"

void Workspace::draw(const Aozora::Context& context)
{

	ImGui::Begin("workspace", NULL, ImGuiWindowFlags_MenuBar); // file browser

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::BeginMenu("New")) {
				if (ImGui::MenuItem("Scene")) {
					// create asset of scene not a live scene in scenemanager
					//context.sceneManager->createScene();
				}
				if (ImGui::MenuItem("Folder")) {
					// create new asset folder or something idk
				}
				if (ImGui::MenuItem("Material")) {
					// create new asset folder or something idk
				}
				if (ImGui::MenuItem("Script")) {
					// create new asset folder or something idk
				}
				if (ImGui::MenuItem("Skybox")) {
					// create new asset folder or something idk
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
