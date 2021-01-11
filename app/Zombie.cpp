#include "graphics.h"
#include "Metrics.h"
#include "Game.h"
#include "Zombie.h"

using namespace graphics;

void Zombie::init()
{
	attackTimer = 0;
}

void Zombie::draw()
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
	//br.fill_secondary_color[0] = 0.2f;
	//br.fill_secondary_color[1] = 0.2f;
	//br.fill_secondary_color[2] = 1.0f;
	//br.gradient = true;
	//br.gradient_dir_u = 1.0f;
	//br.gradient_dir_v = 0.0f;
	drawRect(position.getX() - (((150 - hp) / 150) * 100 / 2), position.getY() - height / 2 - 20, (hp / 150) * 100, 10, br);

	//Outer rectangle
	Brush br1;
	br1.fill_opacity = 0;
	br1.outline_width = 3;
	br1.outline_opacity = 1;
	br1.outline_color[0] = 0.f;
	br1.outline_color[1] = 0.f;
	br1.outline_color[2] = 0.f;
	drawRect(position.getX(), position.getY() - height / 2 - 20, 100, 10, br1);
}

void Zombie::update()
{
	Game* game = reinterpret_cast<Game*>(getUserData());
	
	//int probability = rand() % 50;

	//if (probability == 2)
		//playSound(string(MINECRAFT_BRUH_SOUND_EFFECT), 0.1f);

	chasePlayer(&game->player);
	attack();
	
	if (hp <= 0.f) {
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

void Zombie::chasePlayer(Player* p) {

	if (p->position.getX() > this->position.getX())
		this->setAssetFileMoveRight();

	if (p->position.getX() + 40 > this->position.getX()) {
		this->position.setX(this->position.getX() + (getDeltaTime() / 17.f));
	}

	if (p->position.getX() < this->position.getX())
		this->setAssetFileMoveLeft();

	if (p->position.getX() - 40 < this->position.getX()) {
		this->position.setX(this->position.getX() - (getDeltaTime() / 17.f));

	}
}

void Zombie::attack() {
	// If timer >= 1500 && distance between player.poisition and enemy.position < fixed_number then attack
	Game* g = reinterpret_cast<Game*>(getUserData());

	if (attackTimer >= 1500 && (sqrt(pow(position.getX() - g->player.position.getX(), 2) + pow(position.getY() - g->player.position.getY(), 2))) < 61) {
		playSound(string(MINECRAFT_SOUND_OUH), 0.1f);
		g->player.setHp(g->player.getHp() - 5);
		attackTimer = 0;
	}
}