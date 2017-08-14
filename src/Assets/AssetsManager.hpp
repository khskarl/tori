#ifndef IMGUI_HPP
#define IMGUI_HPP

#include <Types.hpp>

#include <vector>

class Texture;

class AssetsManager {
private:
	std::vector<Texture*> m_textures;

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
	Texture* LoadTexture (const std::string filename);
	Texture* GetTexture (const std::string filename);

};



#endif
