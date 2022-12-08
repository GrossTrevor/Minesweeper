#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
using sf::Sprite;

class Button {
public:
	Button(const char* name);
	Sprite image;
	void SetSprite(const char* name);
	void SetPosition(float x, float y);
	Sprite& GetSprite();
};