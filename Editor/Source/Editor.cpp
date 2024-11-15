#include <iostream>
#include "Engine.h"
#include "EditorUI/EditorUILayer.h"

class Editor : public Aozora::Application {

public:

	

	Editor() : Application("Aozora Editor") { // calls the constructor of Application


		layerStack->addLayer(new EditorUILayer(m_currentScene));

		// create project which essentially is the application we create
		// project will have a scene that the engine works on


		run(); // run the app
	}

private:

	
};


int main() {
	Editor editor;
	
}
