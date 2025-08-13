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
	float assetSpacing = 4.0f;
	float windowWidth = ImGui::GetContentRegionAvail().x; // to figure out where we create new row
	
	for (Aozora::Resources::Asset& asset : context.assetManager->getLoadedAssets()) {
		if (asset.hidden) { // skip hidden assets
			continue;
		}
		ImGui::SameLine(0.0f, assetSpacing);
		ImGui::BeginGroup();
		switch (asset.type)
		{
		case(Aozora::Resources::AssetType::Model):
			if (ImGui::ImageButton(asset.name.c_str(), 0, ImVec2(thumbnailSize, thumbnailSize))) {
				context.sceneManager->getCurrentActiveScene()->instantiateEntity(asset.hash);
			}
			break;
		case(Aozora::Resources::AssetType::Texture):
			break;
		case(Aozora::Resources::AssetType::Material):
			break;
		case(Aozora::Resources::AssetType::Scene):
			break;
		default:
			break;
		}
		ImGui::TextWrapped("%s", asset.name.c_str()); // i expected this to cut the name short to prevent oversize, but didnt do what i thought.
		ImGui::EndGroup();
	}




	ImGui::End();


}
