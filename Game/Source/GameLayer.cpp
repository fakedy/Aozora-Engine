#include "GameLayer.h"
#include "Application.h"

void GameLayer::onUpdate(const Aozora::Context& context)
{
	// remove the getters and use context etc.
	auto& app = Aozora::Application::getApplication();
	auto current_scene = app.getSceneManager().getCurrentActiveScene();

	app.m_cameraSystem->update(current_scene->getRegistry());

	app.getScriptSystem().update(current_scene->getRegistry());
	app.getSceneManager().getCurrentActiveScene()->update();

	app.getRenderer().render();


}
