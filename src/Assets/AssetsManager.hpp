#ifndef IMGUI_HPP
#define IMGUI_HPP

#include <Types.hpp>

#include <vector>

class Texture;
class Mesh;

class AssetsManager {
private:
	std::vector<Texture*> m_textures;
	std::vector<Mesh*> m_meshes;

public:
	static AssetsManager& Get () {
		static AssetsManager instance;
		return instance;
	};

	AssetsManager ();
	~AssetsManager ();

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
