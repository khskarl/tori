#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include <Types.hpp>

#include <Graphics/Texture.hpp>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

namespace Loader {

Texture* LoadTexture (const fs::path filepath);
Texture* LoadPanorama (const fs::path filepath);

void TexturesWindow (bool* p_open);

}

#endif // TEXTURE_LOADER_HPP
