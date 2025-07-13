#pragma once
#include <string>
#include <vector>

namespace Aozora {


	class Model {

	public:

		// for preserving the hierarchal data
		class Node {
		public:
			Node* parentNode{ nullptr };
			std::vector<Node*> childrenNodes;
		
			uint32_t meshID;
			bool hasMesh{ false };
			std::string name{};

		};
		
		std::string name{};
		std::string filePath{};

		Node* originNode{nullptr};
		std::vector<Node*> allNodes;



	private:

	};


}
