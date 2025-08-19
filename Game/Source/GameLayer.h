#pragma once
#include <Systems/Layers/Layer.h>



class GameLayer : public Aozora::Layer {
public:
	GameLayer(Aozora::SceneManager& sceneManager, Aozora::Graphics::SceneRenderer& sceneRenderer) : m_sceneManager(sceneManager) {
		m_gameViewPortID = sceneRenderer.createViewport(Aozora::ViewportType::PrimaryEditor);
	}


	void onUpdate(const Aozora::Context& context) override;

private:

	Aozora::SceneManager& m_sceneManager;
	uint32_t m_gameViewPortID;

};