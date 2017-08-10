#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include <Types.hpp>

#include "Texture.hpp"

namespace Data {

void LoadAllTextures ();
Texture* LoadTexture (const std::string filename);
Texture* LoadCubemap (const std::string filename);
Texture* LoadPanorama (const std::string filename);

void TextureWindow ();

}

#endif // TEXTURE_LOADER_HPP
