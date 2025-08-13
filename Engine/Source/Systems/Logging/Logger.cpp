#include "Logger.h"
#include <chrono>




 void Aozora::Log::info(const std::string& message)
{
	 auto time = std::chrono::high_resolution_clock::now();
	 
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
