#include <iostream>
#include "AozoraAPI/Aozora.h"
#include "EditorUI/EditorUILayer.h"
#include "EditorLayer.h"

class Editor : public Aozora::Application {

public:

	

	Editor() : Application("Aozora Editor") { // calls the constructor of Application

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
