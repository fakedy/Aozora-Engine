#include "Aozora.h"
#include "Systems/ResourceManager/ResourceManager.h"
#include "Systems/ECS/Components/Components.h"
#include "Systems/Renderers/IrenderAPI.h"




namespace Aozora {


	uint32_t ResourcesAPI::loadCubemap(const std::vector<std::string>& paths)
	{
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();
		return resourceManager.loadCubemap(paths);
	}
	uint32_t ResourcesAPI::loadCubemap()
	{
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();
		return resourceManager.loadCubemap();
	}

	SkyboxTextures ResourcesAPI::loadSkybox(const std::vector<std::string>& paths)
	{
		SkyboxTextures data;
		data.skyboxTextureID = loadCubemap(paths);
		// bake the irradiance map
		IrenderAPI& renderAPI = Application::getApplication().getRenderAPI();

		data.irradianceTextureID = renderAPI.bakeCubemapIrradiance(data.skyboxTextureID, loadCubemap());
		return data;
	}


}