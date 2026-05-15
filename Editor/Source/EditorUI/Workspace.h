#pragma once
#include "imgui/imgui.h"

namespace Aozora {
	class Context;
}

class Workspace {

public:

	Workspace(Aozora::Context& context);

	void draw(const Aozora::Context& context);

private:

	ImTextureID m_file_3d_texture;
	ImTextureID m_image_texture;
	ImTextureID m_folder_texture;
	ImTextureID m_script_texture;

	Aozora::Context& m_context;

};