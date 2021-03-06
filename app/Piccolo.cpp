#include "graphics.h"
#include "Metrics.h"
#include "Piccolo.h"
#include "Game.h"


using namespace graphics;

void Piccolo::init()
{
	attackTimer = 0;
}

void Piccolo::draw()
{
	// Draw player 
	Brush br;
	br.fill_opacity = 1;
	br.outline_opacity = 0;
	br.texture = assetFile;
	drawRect(position.getX(), position.getY(), width, height, br);

	// Draw Health Bar

	// Inner rectangle
	br.fill_opacity = 1;
	br.fill_color[0] = 1.f;
	br.fill_color[1] = 0.f;
	br.fill_color[2] = 0.f;
	br.texture = "";
	
	drawRect(position.getX() - (((100-hp)/100)*100/2), position.getY() - height / 2 - 20, (hp/100)*100, 10, br);

	//Outer rectangle
	Brush br1;
	br1.fill_opacity = 0;
	br1.outline_width = 3;
	br1.outline_opacity = 1;
	br1.outline_color[0] = 0.f;
	br1.outline_color[1] = 0.f;
	br1.outline_color[2] = 0.f;
	drawRect(position.getX(), position.getY() - height/2 -20, 100, 10, br1);
}

void Piccolo::update()
{
	chasePlayer(game->player);
	attack();
	if(hp <= 0.f){
       	active = false;
		game->score++;
	}
	if (attackTimer < 1500) {
		attackTimer += getDeltaTime();
	}
	else {
		attackTimer = 1500;
	}
}

void Piccolo::chasePlayer(const Player* const p) {

	if(p->position.getX() > this->position.getX())
		this->setAssetFileMoveRight();

	if (p->position.getX() + 40 > this->position.getX()) {
		this->position.setX(this->position.getX() + (getDeltaTime()/14.f));
	}

	if(p->position.getX() < this->position.getX())
		this->setAssetFileMoveLeft();
	
	if (p->position.getX() - 40 < this->position.getX()) {
		this->position.setX(this->position.getX() - (getDeltaTime()/14.f));
		
	}
}

void Piccolo::attack() {
	
	if (attackTimer >= 1500 && (sqrt(pow(position.getX() - game->player->position.getX(), 2) + pow(position.getY() - game->player->position.getY(), 2))) < 61) {
		playSound(string(MINECRAFT_SOUND_OUH),0.1f);
		game->player->setHp(game->player->getHp() - 5);
		attackTimer = 0;
	}
}