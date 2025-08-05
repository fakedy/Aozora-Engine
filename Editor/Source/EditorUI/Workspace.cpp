#include "Workspace.h"

void Workspace::draw(const Aozora::Context& context)
{

	ImGui::Begin("workspace", NULL, ImGuiWindowFlags_MenuBar); // file browser

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::BeginMenu("New")) {
				if (ImGui::MenuItem("Scene")) {
					Aozora::ApplicationAPI::createNewScene();
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


	// want to create virtual workspace, meaning folders but its all virtual.
	// so what I'll probably do is create a form of structure were we can query all the "assets" in the current "directory"
	std::vector <std::string> loadedModelNames = Aozora::ResourcesAPI::getLoadedModelNames();

	int thumbnailSize = 64;
	float assetSpacing = 4.0f;
	float windowWidth = ImGui::GetContentRegionAvail().x; // to figure out where we create new row
	for (std::string modelName : loadedModelNames) {
		ImGui::SameLine(0.0f, assetSpacing);
		ImGui::BeginGroup();
    		if (ImGui::ImageButton(modelName.c_str(), m_file_3d_texture, ImVec2(thumbnailSize, thumbnailSize))) {
			Aozora::ResourcesAPI::instantiateModel(modelName);
		}
		ImGui::TextWrapped("%s", modelName.c_str());
		ImGui::EndGroup();
	}




	ImGui::End();


}
