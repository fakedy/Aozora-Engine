#pragma once
#include <vector>
#include "Mesh.h"
#include "Systems/Renderers/Shader.h"


namespace Aozora {

	class Model {
	public:

		Model();
		void draw(Shader& shader);

	private:

		
		
		std::vector<Mesh> m_meshes;



	};
}
