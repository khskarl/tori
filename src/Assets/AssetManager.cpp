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
	if (auto tex = FindLoadedTexture(_filename); tex != nullptr) {
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
	if (auto material = FindLoadedMaterial(filepath.filename().string()); material != nullptr) {
		return material;
	}

	Material* const material = Loader::LoadMaterial (filepath.string());
	material->m_filename = filepath.filename().string();

	m_materials.push_back(material);
	return material;
}


// ----- //
// IMGUI //
// ----- //
#include <ImGui.hpp>
bool ImHelpTexturesNamesGetter (void* data, int n, const char** out_text) {
	const std::vector<Texture*>* v = (std::vector<Texture*>*) data;
	*out_text = ((*v)[n])->m_filename.c_str();
	return true;
}

bool ImHelpMaterialsNamesGetter (void* data, int n, const char** out_text) {
	const std::vector<Material*>* v = (std::vector<Material*>*) data;
	*out_text = ((*v)[n])->m_filename.c_str();
	return true;
}

void AssetManager::TexturesWindow (bool* p_open) {
	ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiSetCond_FirstUseEver);

	if (ImGui::Begin("Textures", p_open))
	{
		static int selected_item_index = 0;
		ImGui::BeginChild("texture list", ImVec2(200, 0), true);
			ImGui::PushItemWidth(-1);
			ImGui::ListBox("", &selected_item_index, ImHelpTexturesNamesGetter, &m_textures, m_textures.size(), m_textures.size());
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::TextureInfo(m_textures[selected_item_index]);

	}
	ImGui::End();
}

void AssetManager::MaterialsWindow (bool* p_open) {
	ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiSetCond_FirstUseEver);

	if (ImGui::Begin("Materials", p_open))
	{
		static int selected_item = 0;
		ImGui::BeginChild("materials list", ImVec2(200, 0), true);
			ImGui::PushItemWidth(-1);
			ImGui::ListBox("", &selected_item, ImHelpMaterialsNamesGetter, &m_materials, m_materials.size(), m_textures.size());
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::MaterialInfo(m_materials[selected_item]);

	}
	ImGui::End();
}
