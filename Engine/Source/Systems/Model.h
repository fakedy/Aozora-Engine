#pragma once
#include <string>
#include <vector>

namespace Aozora {


	class Model {

	public:

		// for preserving the hierarchal data
		class Node {
		public:
			Node* parentNode;
			std::vector<Node*> childrenNodes;
		
			uint32_t meshID;

		};
		
		std::string name{};
		std::string filePath{};

		Node* node;
		std::vector<Node*> allNodes;



	private:

	};


}
