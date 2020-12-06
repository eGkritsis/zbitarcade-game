#include "Character.h"

Character::Character(float width, float height, float center_x, float center_y, float hp, const string assetFile){
		position = Vect(center_x, center_y);
		velocity = Vect(0, -40);
		this->width = width;
		this->height = height;
		this->hp = hp;
		asset_character_left = assetFile + "_left.png";
		asset_character_right = assetFile + "_right.png";
		this->assetFile = asset_character_left;
}


float Character::getWidth() {
	return width;
}

float Character::getHeight() {
	return height;
}

float Character::getHp() {
	return hp;
}

string Character::getAssetFile() {
	return assetFile;
}

void Character::setAssetFile(string const name) {
	assetFile = name;
}

void Character::setAssetFileMoveLeft() { 
	assetFile = asset_character_left;
}

void Character::setAssetFileMoveRight() {
	assetFile = asset_character_right;
}