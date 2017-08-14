#ifndef IMGUI_HPP
#define IMGUI_HPP

#include <Types.hpp>

#include <vector>

class Texture;
class Mesh;

class AssetManager {
private:
	std::vector<Texture*> m_textures;
	std::vector<Mesh*> m_meshes;

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

};



#endif
