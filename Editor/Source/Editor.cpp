#include <iostream>
#include "AozoraAPI/Aozora.h"
#include "EditorUI/EditorUILayer.h"
#include "EditorLayer.h"
#include <Context.h>
#include <memory>

class Editor : public Aozora::Application {

public:

	

	Editor() : Application("Aozora Editor") { // calls the constructor of Application


		Aozora::Context context;
		context.renderAPI = m_renderAPI.get();
		context.sceneManager = m_sceneManager.get();
		context.sceneRenderer = m_sceneRenderer.get();

		Aozora::ApplicationAPI::newProject();
		EditorLayer* editlayer = new EditorLayer();
		layerStack->addLayer(editlayer);
		layerStack->addOverlay(new EditorUILayer(editlayer));

		// alright lets go

		run(); // run the app
	}

private:

	
};


int main() {
	Editor editor;
	
}
