#pragma once
#include <string>

// abstract window

namespace Aozora {

	class Window {
	public:


		struct WindowProps {
			std::string title;
			unsigned int width, height;
			bool vSync;

			WindowProps(const std::string& title, unsigned int width, unsigned int height) {
				this->title = title;
				this->width = width;
				this->height = height;
			}
		};

		virtual void* getNativeWindow() = 0;

		virtual void onUpdate() = 0;
		virtual void shutdown() = 0;
		virtual bool windowShouldClose() = 0;


		// factory window method
		static Window* create(const WindowProps& props);

	private:



	};




}