#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <Types.hpp>

#include <vector>

class Texture;
class Mesh;
class Material;

class AssetManager {
private:
	std::vector<Texture*> m_textures;
	std::vector<Mesh*> m_meshes;
	std::vector<Material*> m_materials;

public:
	static AssetManager& Get () {
		static AssetManager instance;
		return instance;
	};

	AssetManager ();
	~AssetManager ();

	void Setup ();
	void Shutdown ();

	// void LoadAllAssets ();
	void UnloadAllAssets ();

	Texture* FindLoadedTexture (const std::string filename);
	Texture* GetTexture (const std::string filename);

	Mesh* FindLoadedMesh (const std::string filename);
	Mesh* GetMesh (const std::string filename);

	Material* FindLoadedMaterial (const std::string filename);
	Material* GetMaterial (const std::string filename);

	// ImGui
	void TexturesWindow  (bool* p_open);
	void MaterialsWindow (bool* p_open);
};

#endif
