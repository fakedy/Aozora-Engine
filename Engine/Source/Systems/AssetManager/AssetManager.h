#pragma once
#include <filesystem>


namespace Aozora::Resources {

	class AssetManager {
	public:

		AssetManager();

		// GOALS -----------
		// load from disk here, store models in binary format.
		// create virtual filesystem for the editor
		// resourcemanager will ask for stuff to be loaded from disk
		// no file will ever be loaded without passing through here


		bool createProject(std::string name);
		void createScene();


	private:


	};
}
