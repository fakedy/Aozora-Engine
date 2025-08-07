#include "Logger.h"





 void Aozora::Log::info(const std::string& message)
{
	std::cout << "[INFO]: " << message << "\n";
}
 void Aozora::Log::warning(const std::string& message)
{
	std::cout << "[WARNING]: " << message << "\n";
}

 void Aozora::Log::error(const std::string& message)
{
	std::cout << "[ERROR]: " << message << "\n";
}
