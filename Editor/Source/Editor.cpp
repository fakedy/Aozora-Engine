#include <iostream>
#include "AozoraAPI/Aozora.h"
#include "EditorUI/EditorUILayer.h"
#include "EditorLayer.h"

class Editor : public Aozora::Application {

public:

	

	Editor() : Application("Aozora Editor") { // calls the constructor of Application


		layerStack->addLayer(new EditorLayer());
		layerStack->addOverlay(new EditorUILayer());


		run(); // run the app
	}

private:

	
};


int main() {
	Editor editor;
	
}
