#include "AssetManager.hpp"

#include <Types.hpp>
#include <Log.hpp>

#include <Graphics/Texture.hpp>
#include <Graphics/Mesh.hpp>
#include "TextureLoader.hpp"
#include "MeshLoader.hpp"

#include <vector>

// This is some edgy dangerou stuff right here
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;


#define TEXTURES_PATH "data/textures/"

AssetManager::AssetManager () {}
AssetManager::~AssetManager () {}

void AssetManager::Setup () {

}

void AssetManager::Shutdown () {

}

void AssetManager::UnloadAllAssets () {

}

// --------------- //
// Texture related //
// --------------- //
Texture* AssetManager::FindLoadedTexture (const std::string filename) {
	for (Texture* const texture : m_textures) {
		if (texture->m_filename == filename)
			return texture;
	}
	return nullptr;
}

Texture* AssetManager::GetTexture (const std::string _filename) {
	// Check if texture is already loaded;
	const fs::path filepath(TEXTURES_PATH + _filename);
	if (auto tex = FindLoadedTexture(filepath.stem().string()); tex != nullptr) {
		return tex;
	}

	LOG_INFO("Loading " + filepath.string());
	std::string extension = filepath.extension().string();

	Texture* texture = nullptr;
	if (extension == ".png") {
		texture = Loader::LoadTexture(filepath);
	}
	else if (extension == ".hdr") {
		texture = Loader::LoadPanorama(filepath);
	}
	else {
		LOG_WARN("Unknown file extension " + filepath.string());
		texture = Loader::LoadTexture(filepath);
	}

	m_textures.push_back(texture);
	return texture;
}


// ------------ //
// Mesh related //
// ------------ //
Mesh* AssetManager::FindLoadedMesh (const std::string filename) {
	for (Mesh* const mesh : m_meshes) {
		if (mesh->m_filename == filename)
			return mesh;
	}
	return nullptr;
}

Mesh* AssetManager::GetMesh (const std::string filename) {
	// Check if texture is already loaded;
	if (auto mesh = FindLoadedMesh(filename); mesh != nullptr) {
		return mesh;
	}

	Mesh* const mesh = Loader::LoadMesh (filename);
	m_meshes.push_back(mesh);
	return mesh;
}
