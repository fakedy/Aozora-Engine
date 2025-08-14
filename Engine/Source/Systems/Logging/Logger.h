#pragma once
#include <iostream>
#include <format>
namespace Aozora {

	class Log {
	public:

		static void info(const std::string& message);

		static void warning(const std::string& message);

		static void error(const std::string& message);



	};

}
