#pragma once
#include "Enemy.h"
class Jason : public Enemy {
public:
	Jason(float width, float height, float center_x, float center_y, float hp, const string assetFile) : Enemy(width, height, center_x, center_y, hp, assetFile) {}

	void init() override;
	void draw() override;
	void update() override;

	void attack() override;

	void chasePlayer(Player* p);

};
