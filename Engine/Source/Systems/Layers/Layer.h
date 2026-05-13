#pragma once
#include <Context.h>
namespace Aozora {

	class Layer
	{
	public:

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(const Aozora::Context& context) = 0;



	private:

	};

}
