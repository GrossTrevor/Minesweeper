#include "Counter.h"

//Sets the digits image as the Sprite and shifts the Texture depending on what number needs to be displayed
Counter::Counter(int num) {
	SetSprite("digits");
	GetSprite().setTextureRect(sf::IntRect((num * 21), 0, 21, 32));
}