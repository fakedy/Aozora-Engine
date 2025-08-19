#pragma once
#include <Application.h>
#include <GameLayer.h>
#include <GameUILayer.h>
class Game : public Aozora::Application {

public:



	Game() : Application("Aozora Editor") { // calls the constructor of Application

		GameLayer* gameLayer = new GameLayer(getSceneManager(), *m_sceneRenderer.get());
		layerStack->addLayer(gameLayer);

		loadProject(); // load scene or something instead, or setup for scene

		layerStack->addOverlay(new GameUILayer());


		run(); // run the app
	}

private:


};


int main() {
	Game editor;

}