#include "Button.h"
#include "TextureManager.h"

//Builds a Button to be placed below the Board (ex: reset, debug, and test 1/2/3)
Button::Button(const char* name) {
	SetSprite(name);
}

void Button::SetSprite(const char* name) {
	this->image.setTexture(TextureManager::GetTexture(name));
}

void Button::SetPosition(float x, float y) {
	this->GetSprite().setPosition(x, y);
}

Sprite& Button::GetSprite() {
	return this->image;
}