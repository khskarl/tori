#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include <Types.hpp>

#include "Texture.hpp"

namespace Data {

void LoadAllTextures ();
Texture* LoadTexture (const std::string filename);

}

#endif // TEXTURE_LOADER_HPP
