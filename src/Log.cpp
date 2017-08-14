#include "Log.hpp"

#include <cstdint>
#include <iostream>
#include <rang.hpp>

namespace Log {

// The code may be ugly, but the output is absolutely beautiful!
// So remember kids, inner beauty is bs, the output is what really counts
void Log(Type _type, const char* _file, uint32_t _line, const char* _function, std::string msg) {
	std::string file = std::string(_file);
	std::string line = std::to_string(_line);
	switch (_type) {
		case Type::Info :
		std::cout <<
			rang::style::bold << "[Info] " << rang::style::reset <<
			msg << "\n";
		break;

		case Type::Warn:
		std::cerr <<
			rang::style::bold << rang::fg::yellow << "[Warn] " << rang::style::reset <<
			msg << "\n";
		break;

		case Type::Error:
		std::cerr <<
			rang::style::bold << rang::fg::red << "[Error] " << rang::style::reset <<
			msg << "\n";
		break;

		case Type::Fatal:
		std::cerr <<
			rang::style::bold << rang::fg::red << "[FATAL] " << rang::style::reset <<
			rang::style::dim << "(" << file << ":" << line << ") " << rang::style::reset <<
			msg << "\n";
		break;

		default:
		break;
	}
}

// void Log(std::ostream& out, std::string msg) {
// 	out << msg << "\n";
// }

}
