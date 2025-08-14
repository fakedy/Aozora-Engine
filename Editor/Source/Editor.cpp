#include <Application.h>
#include <iostream>
#include "EditorUI/EditorUILayer.h"
#include "EditorLayer.h"
#include <Context.h>
#include <memory>
#include <Systems/Events/EventSystem.h>
#include <Systems/Events/Events.h>
#include <Systems/Project/Project.h>
#include <Systems/Scene/Scene.h>
class Editor : public Aozora::Application {

public:

	

	Editor() : Application("Aozora Editor") { // calls the constructor of Application


		EditorLayer* editlayer = new EditorLayer(getSceneManager(), *m_sceneRenderer.get());
		layerStack->addLayer(editlayer);
		layerStack->addOverlay(new EditorUILayer(editlayer));

		createProject();


		run(); // run the app
	}

private:

	
};


int main() {
	Editor editor;
	
}
