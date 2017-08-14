#include "AssetsManager.hpp"

#include <Types.hpp>
#include <Log.hpp>
#include <vector>

#include <Graphics/Texture.hpp>
#include <Graphics/Mesh.hpp>
#include "TextureLoader.hpp"
#include "MeshLoader.hpp"

AssetsManager::AssetsManager () {}
AssetsManager::~AssetsManager () {}

void AssetsManager::Setup () {

}

void AssetsManager::Shutdown () {

}

void AssetsManager::UnloadAllAssets () {

}

// --------------- //
// Texture related //
// --------------- //
Texture* AssetsManager::FindLoadedTexture (const std::string filename) {
	for (Texture* const texture : m_textures) {
		if (texture->m_filename == filename)
			return texture;
	}
	return nullptr;
}

Texture* AssetsManager::GetTexture (const std::string filename) {
	// Check if texture is already loaded;
	if (auto tex = FindLoadedTexture(filename); tex != nullptr) {
		return tex;
	}

	Texture* const texture = Loader::LoadTexture(filename);
	m_textures.push_back(texture);
	return texture;
}


// ------------ //
// Mesh related //
// ------------ //
Mesh* AssetsManager::FindLoadedMesh (const std::string filename) {
	for (Mesh* const mesh : m_meshes) {
		if (mesh->m_filename == filename)
			return mesh;
	}
	return nullptr;
}

Mesh* AssetsManager::GetMesh (const std::string filename) {
	// Check if texture is already loaded;
	if (auto mesh = FindLoadedMesh(filename); mesh != nullptr) {
		return mesh;
	}

	Mesh* const mesh = Loader::LoadMesh (filename);
	m_meshes.push_back(mesh);
	return mesh;
}
