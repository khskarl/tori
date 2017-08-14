#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include <Types.hpp>

#include <Graphics/Texture.hpp>

namespace Loader {

Texture* LoadTexture (const std::string filename);
Texture* LoadPanorama (const std::string filename);

void TexturesWindow (bool* p_open);

}

#endif // TEXTURE_LOADER_HPP
