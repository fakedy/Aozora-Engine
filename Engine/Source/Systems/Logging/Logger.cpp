#include "Logger.h"
#include <chrono>




 void Aozora::Log::info(const std::string& message)
{
	 auto time = std::chrono::high_resolution_clock::now();
	 
	std::cout << "[INFO]: " << message << "\n";

	std::string text = std::format("[INFO]: {}", message);
	LogEntry log = LogEntry{ Level::Info, text };
	textLog.push_back(log);
}
 void Aozora::Log::warning(const std::string& message)
{
	std::cout << "[WARNING]: " << message << "\n";
	std::string text = std::format("[WARNING]: {}", message);
	LogEntry log = LogEntry{ Level::Warning, text };
	textLog.push_back(log);
}

 void Aozora::Log::error(const std::string& message)
{
	std::cout << "[ERROR]: " << message << "\n";
	std::string text = std::format("[ERROR]: {}", message);
	LogEntry log = LogEntry{ Level::Error, text };
	textLog.push_back(log);
}

 void Aozora::Log::clear() {
	 textLog.clear();
 }
