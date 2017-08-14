#include "AssetsManager.hpp"

#include <Types.hpp>
#include <Log.hpp>
#include <vector>

#include <Graphics/Texture.hpp>
#include "TextureLoader.hpp"

AssetsManager::AssetsManager () {}
AssetsManager::~AssetsManager () {}

void AssetsManager::Setup () {

}

void AssetsManager::Shutdown () {

}

void AssetsManager::UnloadAllAssets () {

}

Texture* AssetsManager::FindLoadedTexture (const std::string filename) {
	for (Texture* const texture : m_textures) {
		if (texture->m_filename == filename)
			return texture;
	}
	return nullptr;
}

Texture* AssetsManager::LoadTexture (const std::string filename) {
	return nullptr;
}

Texture* AssetsManager::GetTexture (const std::string filename) {
	// Check if texture is already loaded
	Texture* const loadedTexture = FindLoadedTexture(filename);
	if (loadedTexture != nullptr) {
		return loadedTexture;
	}

	Texture* const newTexture = Loader::LoadTexture(filename);
	m_textures.push_back(newTexture);
	return newTexture;
}
