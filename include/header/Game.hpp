#pragma once
#include "header/common.h"

#include "header/Entity.hpp"
#include "header/Player.hpp"
#include "header/Enemy.hpp"
#include "header/Effect.hpp"
#include "header/Button.hpp"
#include "header/ManagerRender.hpp"
#include "header/ManagerResour.hpp"
#include "header/GameUI.hpp"

class Game {
private:
	double bossHP, currbossHP;
	int realHealth = 0;
	int fightingBoss;

	Player player;
	Entity playerBullet;
	std::vector <Entity> Bullets; 
 
	Enemy* enemy;
	SDL_Texture* enemyTexture;
	Entity enemyBullet;
	SDL_Texture* enemyBulletTexture;
	int enemySpawnTimer;
	std::vector <Enemy*> Enemies;

	ManagerRender managerRender;
	ManagerResour managerResour = ManagerResour(managerRender);
	Effect explosion;
	std::vector <std::vector<Effect>> Explosion;
	SDL_Texture* explosionTexture;

	std::vector<Entity> powerUps;
	Entity powerup;
	bool playAgain;
	GameUI gameUI = GameUI(managerRender,managerResour, player, playerBullet, Bullets, Explosion, powerUps, Enemies);

	

	void initGame();
	void presentEntities();
	void getInput();
	void addExplosion(int x, int y, int type);
	void addPowerUp(int x, int y, int type);
	bool detectCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
	void Pause();
public:
	void start();
};