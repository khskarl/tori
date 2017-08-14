#ifndef MATERIAL_LOADER_HPP
#define MATERIAL_LOADER_HPP

#include <Types.hpp>
#include <Graphics/Material.hpp>

#include <vector>

namespace Loader {

Material* LoadMaterial (const std::string filename);

}
#endif // MATERIAL_LOADER_HPP
