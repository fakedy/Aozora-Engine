#pragma once
#include <iostream>
#include <format>
#include <vector>

namespace Aozora {

	class Log {
	public:

		enum Level
		{
			Info,
			Warning,
			Error
		};

		struct LogEntry {
			Level level;
			std::string text;
		};

		inline static std::vector<LogEntry> textLog;

		static void info(const std::string& message);

		static void warning(const std::string& message);

		static void error(const std::string& message);

		static void clear();


	};

}
