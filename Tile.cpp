#include "Tile.h"
using sf::Sprite;

//Creating a hidden Tile
Tile::Tile() {
	SetSprite("tile_hidden");
	isMine = false;
	isOpen = false;
	isFlag = false;
}

void Tile::SetSprite(const char* name) {
	this->tile.setTexture(TextureManager::GetTexture(name));
}

Sprite& Tile::GetSprite() {
	return this->tile;
}

void Tile::SetPosition(float x, float y) {
	this->GetSprite().setPosition(x, y);
}

//Opening the Tile only if there is no Flag
void Tile::OpenTile() {
	if (!this->IsFlag()) {
		this->SetSprite("tile_revealed");
		this->isOpen = true;
	}
}

void Tile::SetMine() {
	this->isMine = true;
}

//Checks if a Tile contains a Mine
bool Tile::IsMine() {
	if (this->isMine) {
		return true;
	}
	else {
		return false;
	}
}

//Checks if a Tile is revealed
bool Tile::IsOpen() {
	if (this->isOpen) {
		return true;
	}
	return false;
}

//Checks if neighboring Tiles contain a Mine
int Tile::CheckNeighbors() {
	int count = 0;
	for (int i = 0; i < 8; i++) {
		if (this->neighbors[i] != nullptr) {
			if (this->neighbors[i]->IsMine())
				count++;
		}
	}
	return count;
}

//Checks the values of neighboring Tiles and opens them if they do not contain a Mine or Flag (then recurses)
void Tile::OpenNeighbors() {
	if (this != nullptr && this->CheckNeighbors() == 0 && !this->IsMine() && !this->IsFlag()) {
		for (int i = 0; i < 8; i++) {
			if (this->neighbors[i] != nullptr) {
				if (!this->neighbors[i]->IsOpen() && !this->neighbors[i]->IsMine() && !this->IsFlag()) {
					this->neighbors[i]->OpenTile();
					if (this->neighbors[i]->CheckNeighbors() == 0) {
						this->neighbors[i]->OpenNeighbors();
					}
				}
			}
		}
	}
}

void Tile::GiveFlag() {
	this->isFlag = true;
}

//Checks if a Tile contains a Flag
bool Tile::IsFlag() {
	if (this->isFlag) {
		return true;
	}
	else {
		return false;
	}
}

void Tile::RemoveFlag() {
	this->isFlag = false;
}

//Resets a Tile to its default state
void Tile::ResetTile() {
	this->SetSprite("tile_hidden");
	this->isOpen = false;
	isMine = false;
	isFlag = false;
}