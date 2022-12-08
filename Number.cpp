#include "Number.h"

//Sets Sprite based on input number derived from the number of Mines surrounding a Tile
Number::Number(int num) {
	if (num == 1)
		SetSprite("number_1");
	if (num == 2)
		SetSprite("number_2");
	if (num == 3)
		SetSprite("number_3");
	if (num == 4)
		SetSprite("number_4");
	if (num == 5)
		SetSprite("number_5");
	if (num == 6)
		SetSprite("number_6");
	if (num == 7)
		SetSprite("number_7");
	if (num == 8)
		SetSprite("number_8");
}
