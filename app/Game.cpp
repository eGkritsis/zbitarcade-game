#include "Game.h"
#include "Haunter.h"
#include "Piccolo.h"
#include "Metrics.h"
#include "graphics.h"
#include <math.h>
#include <random>
#include "Zombie.h"
#include "Goomba.h"
#include "Jason.h"
#include "PowerUp.h"
#include "Player.h"

using namespace graphics;

Game::Game()
{
}

Game::~Game() {

	for (list<Projectile*>::iterator it = player.projectile_list.begin(); it != player.projectile_list.end(); ++it) {
		delete *it;
	}
	for (list<Enemy *>::iterator it = enemy_list.begin(); it != enemy_list.end(); ++it) {
		delete *it;
	}

	if (pu)
		delete pu;
}

void Game::init() {
	timer = 0.f;
	timerLimit = 4000.f;
	score = 0;
	prevScore = 0;
	button_timer = .0f;
	//state = PLAYING;
	if (state == MAIN_MENU)
		playSound(string(MAKE_YOUR_SELECTION_NOW), 0.3f);
	if(state == PLAYING)
		playMusic(string(JOJOS_MAIN_MUSIC), 0.05f);
	player.init();
}

void Game::draw()
{
	// background
	Brush br;
	br.fill_opacity = 1;
	br.outline_opacity = 0;
	br.texture = "assets\\background.png";
	drawRect((CANVAS_WIDTH) / 2, (CANVAS_HEIGHT) / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	switch(state) {
		case PLAYING: {
			// Score text
			Brush br0;
			br0.fill_opacity = 1;
			br0.outline_opacity = 0;
			br0.texture = "";
			br0.fill_color[0] = .0f;
			br0.fill_color[1] = .0f;
			br0.fill_color[2] = .0f;
			setFont(string(POCOYO_TV_FONT));
			drawText(160, 50, 30, "Score: " + to_string(score), br0);

			// Goku player picture
			Brush br1;
			br1.fill_opacity = 1;
			br1.outline_opacity = 0;
			br1.texture = string(GOKU_PROFILE_PIC);
			drawRect(65, 57, 75, 75, br1);

			// UI Hp bar
			float player_hp = (player.getHp() > 0) ? player.getHp() : 0;
			Brush br3;
			br3.fill_opacity = 1;
			br3.fill_color[0] = 0.f + (200 - player_hp) / 200; // RED
			br3.fill_color[1] = 1.f - (200 - player_hp) / 200; // GREEN
			br3.fill_color[2] = 0.f;					   // BLUE
			br3.texture = "";


			drawRect(236 - (((200 - player_hp) / 200) * 205 / 2), 75, (player_hp / 200) * 205, 27, br3);

			// Player UI
			Brush br2;
			br2.fill_opacity = 1;
			br2.outline_opacity = 0;
			br2.texture = string(PLAYER_UI);
			drawRect(180, 60, 360, 100, br2);

			// Draw Entities:
			for (list<Enemy*>::iterator it = enemy_list.begin(); it != enemy_list.end(); ++it) {
				(*it)->draw();
			}
			// Draw Player:
			if (player.getActiveStatus()) {
				player.draw();
			}
			// Draw PowerUp:
			if (pu && !pu->getCaptured()) {
				pu->draw();
			}
			break;
		}
		case MAIN_MENU: {

			Brush br0;
			br0.fill_opacity = 1;
			br0.outline_opacity = 1;
			br0.texture = "";
			br0.fill_color[0] = .0f;
			br0.fill_color[1] = .0f;
			br0.fill_color[2] = .0f;
			setFont(string(DRAGON_BALL_Z_FONT));
			drawText(CANVAS_WIDTH / 2 - 230, CANVAS_HEIGHT / 2 - 130, 100, "Zbit Arcade", br0);
			br0.fill_color[0] = 1.0f;
			br0.fill_color[1] = 0.84f;
			br0.fill_color[2] = .0f;
			drawText(CANVAS_WIDTH / 2 - 240, CANVAS_HEIGHT / 2 - 120, 105, "Zbit Arcade", br0);

			if (buttonMM == PLAY) {
				br0.fill_color[0] = 1.0f;
				br0.fill_color[1] = 0.84f;
			}
			else {
				br0.fill_color[0] = .0f;
				br0.fill_color[1] = .0f;
			}
			br0.fill_color[2] = .0f;
			drawText(CANVAS_WIDTH/2 - 70, CANVAS_HEIGHT/2 + 20, 50, "PLAY", br0);

			if (buttonMM == ABOUT) {
				br0.fill_color[0] = 1.0f;
				br0.fill_color[1] = 0.84f;
			}
			else {
				br0.fill_color[0] = .0f;
				br0.fill_color[1] = .0f;
			}
			br0.fill_color[2] = .0f;
			drawText(CANVAS_WIDTH/2 - 70, CANVAS_HEIGHT/2 + 90 , 50, "ABOUT", br0);
			

			if (buttonMM == EXIT) {
				br0.fill_color[0] = 1.0f;
				br0.fill_color[1] = 0.84f;
			}
			else {
				br0.fill_color[0] = .0f;
				br0.fill_color[1] = .0f;
			}
			br0.fill_color[2] = .0f;
			drawText(CANVAS_WIDTH/2 - 70, CANVAS_HEIGHT/2 + 160 , 50, "EXIT", br0);

			// ARROW
			Brush br1;
			br1.fill_opacity = 1;
			br1.outline_opacity = 0;
			br1.texture = string(_8BIT_ARROW);
			drawRect(CANVAS_WIDTH/2 - 110, CANVAS_HEIGHT/2 + 5 + arrow_offset, 50, 50, br1);

			break;
		}
		case RETRY:
			break;
	}
}

void Game::update()
{	
	switch (state) {

		case PLAYING:
			if (getGlobalTime() > 2000 && timer > timerLimit) {

				int choice = rand() % 5;

				switch (choice) {

				case 0:
					enemy_list.push_back(new Piccolo(CHARACTER_WIDTH, CHARACTER_HEIGHT, (rand() % 2) * CANVAS_WIDTH, CANVAS_HEIGHT - CHARACTER_HEIGHT / 2, 100, string(PICCOLO)));
					enemy_list.back()->init();
					break;
				case 1:
					if (rand() % 2 == 1) {
						enemy_list.push_back(new Zombie(CHARACTER_WIDTH, CHARACTER_HEIGHT, (rand() % 2) * CANVAS_WIDTH, CANVAS_HEIGHT - CHARACTER_HEIGHT / 2, 200, string(ZOMBIE)));
						enemy_list.back()->init();
					}
					break;
				case 2:
					if (rand() % 3 == 1) {
						enemy_list.push_back(new Haunter(80, 80, (rand() % 2) * CANVAS_WIDTH, CANVAS_HEIGHT - CHARACTER_HEIGHT / 2 - 50, 100, string(HAUNTER)));
						enemy_list.back()->init();
					}
					break;
				case 3:
					if (rand() % 2 == 1) {
						enemy_list.push_back(new Goomba(60, 60, (rand() % 2) * CANVAS_WIDTH, CANVAS_HEIGHT - 25, 25, string(GOOMBA)));
						enemy_list.back()->init();
					}
					break;
				case 4:
					enemy_list.push_back(new Jason(CHARACTER_WIDTH, CHARACTER_HEIGHT, (rand() % 2) * CANVAS_WIDTH, CANVAS_HEIGHT - CHARACTER_HEIGHT / 2, 100, string(JASON)));
					enemy_list.back()->init();
					break;
				}


				timer = 0.f;
			}


			for (list<Enemy*>::iterator it = enemy_list.begin(); it != enemy_list.end(); ++it) {
				if (*it != nullptr) {
					if ((*it)->getActiveStatus()) {
						(*it)->update();
					}
					else {
						// Spawn power up:
						if (pu == nullptr && !player.getUpgraded() && rand() % 9 == 4) {
							pu = new PowerUp(50, 50, (*it)->position.getX(), (*it)->position.getY() - 20, string(POWER_UP_STAR));
							pu->init();
						}


						delete* it;
						it = enemy_list.erase(it);


						if (it == enemy_list.end())
							break;
					}
				}
			}
			player.update();
			if (pu && pu->getActiveStatus()) {
				pu->update();
			}
			else if (pu) {
				delete pu;
				pu = nullptr;
			}
			updateTimers();
			if (score > 0 && score != prevScore && score % waveVariable == 0 && timerLimit > 1500) {
				timerLimit -= 1000;
				prevScore = int(score);
			}
			break;
		case MAIN_MENU:

			if(getKeyState(SCANCODE_DOWN) && button_timer >= BUTTON_DELAY){
				switch (buttonMM) {

				case PLAY:
					buttonMM = ABOUT;
					arrow_offset += 70;
					break;
				case ABOUT:
					buttonMM = EXIT;
					arrow_offset += 70;
					break;
				case EXIT:
					buttonMM = PLAY;
					arrow_offset -= 140;
					break;
				}
				playSound(string(BEEP_SOUND_EFFECT), 0.3f);
				button_timer = 0.f;
			}

			if(getKeyState(SCANCODE_UP) && button_timer >=BUTTON_DELAY) {
				switch (buttonMM) {

				case PLAY:
					buttonMM = EXIT;
					arrow_offset += 140;
					break;
				case ABOUT:
					buttonMM = PLAY;
					arrow_offset -= 70;
					break;
				case EXIT:
					buttonMM = ABOUT;
					arrow_offset -= 70;
					break;
				}
				playSound(string(BEEP_SOUND_EFFECT), 0.3f);
				button_timer = 0.f;
			}

			// ENTER
			if (getKeyState(SCANCODE_RETURN) && button_timer >= BUTTON_DELAY) {
				switch (buttonMM) {
				case PLAY:
					state = PLAYING;
					init();
					break;
				case ABOUT:
					break;
				case EXIT:
					destroyWindow();
					break;
				}
				playSound(string(BEEP_SOUND_EFFECT), 0.3f);
				button_timer = 0.f;
			}
			button_timer += getDeltaTime();
			if (button_timer > BUTTON_DELAY)
				button_timer = BUTTON_DELAY;
			break;
		case RETRY:
			break;	
	}
	
}

void Game::updateTimers() {
	timer += getDeltaTime();
}
