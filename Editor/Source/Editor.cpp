#include <iostream>
#include "AozoraAPI/Aozora.h"
#include "EditorUI/EditorUILayer.h"
#include "EditorLayer.h"
#include <Context.h>
#include <memory>
#include <Systems/Events/EventSystem.h>
#include <Systems/Events/Events.h>
#include <Systems/Project/Project.h>
class Editor : public Aozora::Application {

public:

	

	Editor() : Application("Aozora Editor") { // calls the constructor of Application


		
		createNewProject();


		EditorLayer* editlayer = new EditorLayer();
		layerStack->addLayer(editlayer);
		layerStack->addOverlay(new EditorUILayer(editlayer));


		run(); // run the app
	}

private:

	
};


int main() {
	Editor editor;
	
}
