#pragma once


struct SkyboxTextures {
	uint32_t skyboxTextureID{ 0 };
	uint32_t irradianceTextureID{ 0 };
};

struct SkyboxComponent {

	SkyboxTextures data;

};