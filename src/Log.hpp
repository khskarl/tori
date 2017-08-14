#ifndef LOG_HPP
#define LOG_HPP

#include <stdio.h>
#include <cstdint>
#include <string>

#define LOG_INFO(_msg, ...) { Log::Log(Log::Type::Info, __FILE__, __LINE__, __FUNCTION__, _msg); }
#define LOG_WARN(_msg, ...) { Log::Log(Log::Type::Warn, __FILE__, __LINE__, __FUNCTION__, _msg); }
#define LOG_ERROR(_msg, ...) { Log::Log(Log::Type::Error, __FILE__, __LINE__, __FUNCTION__, _msg); }
#define LOG_FATAL(_msg, ...) { Log::Log(Log::Type::Fatal, __FILE__, __LINE__, __FUNCTION__, _msg); }


namespace Log {
enum Type {
	Info,
	Warn,
	Error,
	Fatal,
};

void Log(Type type, const char* file, uint32_t line, const char* function, std::string msg);
// void Log(std::ostream& out, std::string msg);
}


#endif
