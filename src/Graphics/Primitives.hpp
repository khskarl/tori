#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <cstdint>
#include <Graphics/Mesh.hpp>

namespace Primitives {
void Quad (Mesh* out_mesh);
void Grid (size_t const width, size_t const depth, Mesh* out_mesh);
}

#endif // PRIMITIVE_HPP
