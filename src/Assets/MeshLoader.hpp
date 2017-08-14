#ifndef MESH_LOADER_HPP
#define MESH_LOADER_HPP

#include <vector>
#include <Types.hpp>

#include <Graphics/Mesh.hpp>

namespace Loader {

Mesh* LoadMesh (const std::string filename);

}
#endif // MESH_LOADER_HPP
