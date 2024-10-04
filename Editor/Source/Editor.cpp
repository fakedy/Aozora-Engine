#include <iostream>
#include "Engine.h"
#include "EditorUI/EditorUILayer.h"

class Editor : public Aozora::Application {

public:

	

	Editor() : Application("Aozora Editor") { // calls the constructor of Application


		layerStack->addLayer(new EditorUILayer(registry));
		run(); // run the app

	}

private:

	
};


int main() {
	Editor editor;
	
}
