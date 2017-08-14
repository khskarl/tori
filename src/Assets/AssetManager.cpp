#include "AssetManager.hpp"

#include <Types.hpp>
#include <Log.hpp>

#include <Graphics/Texture.hpp>
#include <Graphics/Mesh.hpp>
#include <Graphics/Material.hpp>

#include "TextureLoader.hpp"
#include "MeshLoader.hpp"
#include "MaterialLoader.hpp"

#include <vector>

// This is some edgy dangerou stuff right here
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#define PATH_TEXTURES  "data/textures/"
#define PATH_MODELS    "data/models/"
#define PATH_MATERIALS "data/materials/"

// TODO: Create an 'Asset' base class to reduce repeated code
AssetManager::AssetManager () {}
AssetManager::~AssetManager () {}

void AssetManager::Setup () {}
void AssetManager::Shutdown () {}
void AssetManager::UnloadAllAssets () {}

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
	const fs::path filepath(PATH_TEXTURES + _filename);
	if (auto tex = FindLoadedTexture(filepath.stem().string()); tex != nullptr) {
		return tex;
	}

	LOG_INFO("Loading " + filepath.string());
	std::string extension = filepath.extension().string();

	// TODO: Move the extension handling to the Texture Loader
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
	// Check if mesh is already loaded;
	if (auto mesh = FindLoadedMesh(filename); mesh != nullptr) {
		return mesh;
	}

	Mesh* const mesh = Loader::LoadMesh (filename);
	m_meshes.push_back(mesh);
	return mesh;
}

// ---------------- //
// Material related //
// ---------------- //
Material* AssetManager::FindLoadedMaterial (const std::string filename) {
	for (Material* const material : m_materials) {
		if (material->m_filename == filename)
			return material;
	}
	return nullptr;
}

Material* AssetManager::GetMaterial (const std::string _filename) {
	const fs::path filepath(PATH_MATERIALS + _filename);
	// Check if material is already loaded;
	if (auto material = FindLoadedMaterial(filepath.stem().string()); material != nullptr) {
		return material;
	}

	Material* const material = Loader::LoadMaterial (filepath.string());
	m_materials.push_back(material);
	return material;
}
