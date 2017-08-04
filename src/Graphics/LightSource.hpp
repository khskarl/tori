#ifndef LIGHT_SOURCE_HPP
#define LIGHT_SOURCE_HPP

#include <Types.hpp>

struct LightSource {
	glm::vec3 position  = glm::vec3(0, 0, 0);
	glm::vec3 direction = glm::vec3(0, -1, 0);
	glm::vec3 color     = glm::vec3(1, 1, 1);

	enum Type {
		Directional = 0,
		Point = 1,
	 };

	Type type;

	LightSource (const Type arg_type = Directional) {
		type = arg_type;
	}
	// LightSource (const glm::vec3 arg_pos    = glm::vec3(0),
	// 	           const glm::vec3 arg_dir    = glm::vec3(0, -1, 0),
	// 	           const glm::vec3 arg_color  = glm::vec3(1),
	// 	           const float     arg_radius = 3.0f) {
	// 	position  = arg_pos;
	// 	direction = arg_dir;
	// 	color     = arg_color;
	// 	radius    = arg_radius;
	// }
};

#endif
