#include "TextureManager.h"

unordered_map<string, Texture> TextureManager::textures;

//Builds a filePath to load a Texture from a .png file
void TextureManager::LoadTexture(const char* fileName) {
	string filePath = "images/";
	filePath += fileName;
	filePath += ".png";
	textures[fileName].loadFromFile(filePath);
}

//Uses LoadTexture() to get a Texture (if not already stored) and return it to be applied to a Sprite
Texture& TextureManager::GetTexture(const char* name) {
	if (textures.find(name) == textures.end()) {
		LoadTexture(name);
	}
	return textures[name];
}

//Clears the unordered_map
void TextureManager::Clear() {
	textures.clear();
}