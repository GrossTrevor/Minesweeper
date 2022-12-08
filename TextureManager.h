#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
using std::string;
using std::unordered_map;
using sf::Texture;

class TextureManager {
	//Uses an unordered_map because iteration is not needed
	static std::unordered_map<std::string, sf::Texture> textures;
	static void LoadTexture(const char* fileName);

public:
	static sf::Texture& GetTexture(const char* name);
	static void Clear();
};