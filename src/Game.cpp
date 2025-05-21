#include "header/Game.hpp"
using namespace std;

void Game::start() {
	std::cout << "GameUI Constructor Start\n";
	initGame();
 	while (true) {
		if (!playAgain) {
			gameUI.titleScreen(realHealth, fightingBoss, enemySpawnTimer,gPlaying);
		}
		while (gPlaying) {
			managerRender.prepareScene();
			managerRender.drawBackground();
			if (player.getHealth()) {
				getInput();
			}
			if (!gPause) {
				presentEntities();
				gameUI.HUD(currbossHP, bossHP, fightingBoss);
			}
			else {
				Pause();
			}
			gameUI.presentScene(realHealth, fightingBoss, enemySpawnTimer,gPlaying);
		}
		gameUI.endScreen(realHealth, fightingBoss, enemySpawnTimer,gPlaying,playAgain);
	}
}


void Game::initGame() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		cout << "Could not initialize SDL: " << SDL_GetError() << endl;
		exit(-1);
	}

	gWindow = SDL_CreateWindow("GAME GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!gWindow) {
		cout << "Could not create window : " << SDL_GetError() << endl;
		exit(-1);
	}
	SDL_Surface* sf = IMG_Load("./resources/gfx/other/logo.png");
	SDL_SetWindowIcon(gWindow, sf);

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (!gRenderer) {
		cout << "Could not create renderer : " << SDL_GetError() << endl;
		exit(-1);
	}

	if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) {
		cout << "Could not initialize SDL Image : " << SDL_GetError() << endl;
		exit(-1);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		cout << "Could not initialize SDL Mixer : " << SDL_GetError() << endl;
		exit(-1);
	}

	if (TTF_Init() == -1) {
		cout << "Could not initialize SDL TTF : " << SDL_GetError() << endl;
		exit(-1);
	}

	Mix_AllocateChannels(8);

	gPlaying = false;
	
	playerBullet.setDX(PLAYER_BULLET_SPEED);
	playerBullet.setHealth(1);
	enemyBulletTexture = managerRender.loadTexture("./resources/gfx/bulletsprites/alienBullet.png");
	enemyTexture = managerRender.loadTexture("./resources/gfx/spaceship/enemy.png");
	managerResour.loadResources();
	explosionTexture = managerRender.loadTexture("./resources/gfx/other/explosion.png");

	gameUI.setUpMenu();

	playAgain = false;


	fightingBoss = 0;
	enemySpawnTimer = 60;
}


void Game::getInput() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			exit(0);
			break;
		case SDL_KEYUP:
			player.keyUp(&e.key);
			break;
		case SDL_KEYDOWN:
			if (e.key.keysym.scancode == SDL_SCANCODE_P) {
				Mix_PlayChannel(-1, managerResour.buttonSound, 0);
				gPause = !gPause;
			}
			if (e.key.keysym.scancode == SDL_SCANCODE_M) {
				if (Mix_PausedMusic()) {
					Mix_ResumeMusic();
				} else {
					Mix_PauseMusic();
				}
			}
			player.keyDown(&e.key);
			break;
		}
	}
}

void Game::Pause() {
	managerRender.draw(gameUI.pauseIcon, SCREEN_WIDTH / 2 - 210, SCREEN_HEIGHT / 2 - 225);
	string gamepause = "GAME PAUSED";
	SDL_Surface* pauseSf = TTF_RenderText_Solid(managerResour.font, gamepause.c_str(), { 255, 255, 255, 0 });
	SDL_DestroyTexture(gameUI.pauseTXT);
	gameUI.pauseTXT = SDL_CreateTextureFromSurface(gRenderer, pauseSf);
	gPause = true;
	SDL_FreeSurface(pauseSf);
	managerRender.draw(gameUI.pauseTXT, SCREEN_WIDTH / 2 - 100, 3 * SCREEN_HEIGHT / 4 - 50);
}

void Game::presentEntities() {
	if (player.getDecay() > 1) player.setDecay(player.getDecay() - 1);
	else if (player.getDecay() == 1) {
		player.setDecay(0);
		player.setHealth(realHealth);
	}

	if (player.getRGBTimer() > 0) player.setRGBTimer(player.getRGBTimer() - 1);
	else SDL_SetTextureColorMod(player.getTexture(), 255, 255, 255);

	if (player.getHealth() > 0) {
		if (player.getStunt() == 0) player.move();
		else player.setStunt(player.getStunt() - 1);

		managerRender.draw(player.getTexture(), player.getX(), player.getY());
		int wP, hP;
		if (player.Fire() == true && player.getReload() == 0) {
			SDL_QueryTexture(player.getTexture(), NULL, NULL, &wP, &hP);
			playerBullet.setX(player.getX() + wP / 2);
			playerBullet.setY(player.getY() + hP / 2);
			playerBullet.setType(-1);
			player.setReload(player.getReloadRate());
			Bullets.push_back(playerBullet);
			if (player.getEnhance() > 0) {
				playerBullet.setX(player.getX());
				playerBullet.setY(player.getY());
				Bullets.push_back(playerBullet);

				playerBullet.setX(player.getX());
				playerBullet.setY(player.getY() + hP);
				Bullets.push_back(playerBullet);
			}
			Mix_PlayChannel(-1, managerResour.bulletSound, 0);
		}
	}


	if (gameUI.score % 5 == 0) fightingBoss = gameUI.score/5;

	//Spawn enemy
	if (enemySpawnTimer == 0) {
		if(fightingBoss == 0) {
			enemy = new Enemy();
			enemy->setTexture(enemyTexture);
			//enemy->setTexture(managerRender.loadTexture("gfx/enemy.png"));
			int rate = rand() % 4;
			if (rate < 2) {
				enemy->setX(SCREEN_WIDTH - 80);
				enemy->setDX(ENEMY_SPEED - rand() % 8);
				enemy->setY(rand() % 400 + 100);
			}
			else {
				enemy->setX(SCREEN_WIDTH / 2 + rand() % 200);
				if (rate == 1) {
					enemy->setDY(-ENEMY_SPEED + rand() % 8);
					enemy->setY(-10);
				}
				else {
					enemy->setDY(ENEMY_SPEED - rand() % 8);
					enemy->setY(690);
				}
			}
			enemy->setHealth(5);
			enemySpawnTimer = 60;
			Enemies.push_back(enemy);
		} else {
			enemy = new Enemy();
			cout << "Boss " << fightingBoss << endl;
			enemy->setTexture(managerResour.getEnemyBoss((fightingBoss - 1) % 3));
			enemy->setX(SCREEN_WIDTH / 2 + rand() % 300);
			enemy->setDY(-ENEMY_SPEED/3 + rand() % 3);
			enemy->setY(-300);
			enemy->setHealth(50*fightingBoss);
			bossHP = enemy->getHealth();
			enemy->setType(2);
			enemySpawnTimer = -1; //no normal enemy will be spawn while fighting boss
			Enemies.push_back(enemy);
		}
	}


	//Checking removable fighter
	for (auto i = Enemies.begin(); i != Enemies.end(); ) {
		int w, h;
		SDL_QueryTexture((*i)->getTexture(), NULL, NULL, &w, &h);
		if ((*i)->getX() <= -w) {
			i = Enemies.erase(i);
		}
		else if ((*i)->getHealth() <= 0) {
			addExplosion((*i)->getX(), (*i)->getY(), 0);
			gameUI.setScore(gameUI.score + 1);
			Mix_PlayChannel(-1, managerResour.explosionSound, 0);
			if ((*i)->getType() == 2) {
				fightingBoss = 0;
				enemySpawnTimer = 80;
			}
			i = Enemies.erase(i);
		}
		else {
			if ((*i)->getRGBTimer() > 0) (*i)->setRGBTimer((*i)->getRGBTimer() - 1);
			else SDL_SetTextureColorMod((*i)->getTexture(), 255, 255, 255);
			if ((*i)->getReload() > 0) (*i)->setReload((*i)->getReload() - 1);
			else {
				if ((*i)->getType() == 1) {
					enemyBullet.setTexture(enemyBulletTexture);
					enemyBullet.setX((*i)->getX() - w / 2);
					enemyBullet.setY((*i)->getY() + h / 2);
					float dx = player.getX() - enemyBullet.getX();
					float dy = player.getY() - enemyBullet.getY();
					float distance = sqrt(dx * dx + dy * dy);
					dx /= distance;
					dy /= distance;
					enemyBullet.setDX(dx * (ENEMY_BULLET_SPEED + rand() % 8));
					enemyBullet.setDY(dy * (ENEMY_BULLET_SPEED + rand() % 8));
					enemyBullet.setHealth(1);
					enemyBullet.setType(0);
					(*i)->setReload(30 + rand() % 30);
					Bullets.push_back(enemyBullet);
				} else {
					enemyBullet.setTexture(managerResour.getBossBullet((fightingBoss-1) % 3));
					enemyBullet.setX((*i)->getX() - w / 2);
					enemyBullet.setY((*i)->getY() + h / 2);
					float dx = player.getX() - enemyBullet.getX();
					float dy = player.getY() - enemyBullet.getY();
					float distance = sqrt(dx * dx + dy * dy);
					dx /= distance;
					dy /= distance;
					enemyBullet.setDX(dx * ENEMY_BULLET_SPEED);
					enemyBullet.setDY(dy * ENEMY_BULLET_SPEED);
					enemyBullet.setHealth(1);
					enemyBullet.setType((fightingBoss - 1) % 3 + 1);
					(*i)->setReload(30 + rand() % 30);
					Bullets.push_back(enemyBullet);
				}
			}

			(*i)->move();
			if ( ((*i)->getY() < 0 && (*i)->getDY() < 0) || ((*i)->getY() + h > SCREEN_HEIGHT && (*i)->getDY() > 0) ) {
				(*i)->setDY(-(*i)->getDY());
			} // for enemy move vertically 

			managerRender.draw((*i)->getTexture(), (*i)->getX(), (*i)->getY());
			i++;
		}

	}

	//Checking removable bullets
	for (auto i = Bullets.begin(); i != Bullets.end(); ) {
		int w, h;
		SDL_QueryTexture(i->getTexture(), NULL, NULL, &w, &h);
		if (i->getX() <= -w || i->getX() >= SCREEN_WIDTH || i->getY() <= -h || i->getY() > SCREEN_HEIGHT || !i->getHealth()) {
			i = Bullets.erase(i);
		}
		else {
			i->move();
			managerRender.draw(i->getTexture(), i->getX(), i->getY());
			i++;
		}
	}

	//Check bullet hit plane
	for (auto i = Bullets.begin(); i != Bullets.end(); i++) {
		int wB, hB, wP, hP;
		SDL_QueryTexture(i->getTexture(), NULL, NULL, &wB, &hB);
		SDL_QueryTexture(player.getTexture(), NULL, NULL, &wP, &hP);
		if (detectCollision(i->getX(), i->getY(), wB, hB, player.getX(), player.getY(), wP, hP)) {
			switch (i->Type()) {
			case 0:
				player.setRGBTimer(10);
				SDL_SetTextureColorMod(player.getTexture(), 255, 0, 0);
				i->setHealth(0);
				player.setHealth(player.getHealth() - 2);
				if (player.getHealth() <= 0) {
					player.setHealth(0);
					addExplosion(player.getX(), player.getY(), 1);
					Mix_PlayChannel(-1, managerResour.playerExplosionSound, 0);
				}
				break;
			case 1:
				player.setRGBTimer(10);
				SDL_SetTextureColorMod(player.getTexture(), 255, 0, 0);
				i->setHealth(0);
				player.setHealth(player.getHealth() - 6);
				if (player.getHealth() <= 0) {
					player.setHealth(0);
					addExplosion(player.getX(), player.getY(), 1);
					Mix_PlayChannel(-1, managerResour.playerExplosionSound, 0);
				}
				break;
			case 2:
				player.setRGBTimer(60);
				SDL_SetTextureColorMod(player.getTexture(), 255, 255, 51);
				i->setHealth(0);
				player.setHealth(player.getHealth() - 2);
				if(player.getStunt() == 0) player.setStunt(60);
				if (player.getHealth() <= 0) {
					player.setHealth(0);
					addExplosion(player.getX(), player.getY(), 1);
					Mix_PlayChannel(-1, managerResour.playerExplosionSound, 0);
				}
				break;
			case 3:
				player.setRGBTimer(180);
				SDL_SetTextureColorMod(player.getTexture(), 147, 112, 219);
				i->setHealth(0);
				if (player.getDecay() == 0) {
					realHealth = player.getHealth();
					player.setHealth(1);
					player.setDecay(180);
				}
				else {
					player.setHealth(0);
				}
				if (player.getHealth() <= 0) {
					player.setHealth(0);
					addExplosion(player.getX(), player.getY(), 1);
					Mix_PlayChannel(-1, managerResour.playerExplosionSound, 0);
				}
				break;
			}
		}

		for (auto j = Enemies.begin(); j != Enemies.end(); j++) {
			int wE, hE;
			SDL_QueryTexture((*j)->getTexture(), NULL, NULL, &wE, &hE);

			if (i->Type() == -1 && detectCollision(i->getX(), i->getY(), wB, hB, (*j)->getX(), (*j)->getY(), wE, hE)) {
				(*j)->setRGBTimer(10);
				SDL_SetTextureColorMod((*j)->getTexture(), 255, 0, 0);
				i->setHealth(0);
				(*j)->setHealth((*j)->getHealth() - player.getDamageRate());
				if ((*j)->getHealth() <= 0) {
					if ((*j)->getType() == 1) {
						int num = rand() % 10 + 1;
						if (num < 4) {
							addPowerUp((*j)->getX(), (*j)->getY(), 1);
						}
						else if (num < 3) {
							addPowerUp((*j)->getX(), (*j)->getY(), 2);
						}
					}
					else {
						addPowerUp((*j)->getX(), (*j)->getY(), 1);
						addPowerUp((*j)->getX(), (*j)->getY(), 2);
					}
				}
			}

			if (detectCollision(player.getX(), player.getY(), wP, hP, (*j)->getX(), (*j)->getY(), wE, hE)) {
				player.setHealth(0);
				addExplosion(player.getX(), player.getY(), 1);
				Mix_PlayChannel(-1, managerResour.playerExplosionSound, 0);
				(*j)->setHealth(0);
			}

		}
	}

	//Draw explosion effect
	for (auto i = Explosion.begin(); i != Explosion.end(); ) {
		bool remove = false;
		for (auto j = i->begin(); j != i->end(); j++) {
			if (j->getA() <= 0) {
				remove = true;
				break;
			}
			SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_ADD);
			SDL_SetTextureBlendMode(j->getTexture(), SDL_BLENDMODE_ADD);

			SDL_SetTextureColorMod(j->getTexture(), j->getR(), j->getG(), j->getB());
			SDL_SetTextureAlphaMod(j->getTexture(), j->getA());

			managerRender.draw(j->getTexture(), j->getX(), j->getY());
			j->updateA(-15);

		}

		if (remove) {
			i = Explosion.erase(i);
		}
		else {
			i++;
		}
	}

	//do power ups
	for (auto i = powerUps.begin(); i != powerUps.end(); ) {
		if (i->getHealth() == 0 || i->getX() < 0 || i->getY() < 0 || i->getY() > 680) {
			i = powerUps.erase(i);
			break;
		} else {
			int wPU, hPU, wP, hP;
			SDL_QueryTexture(i->getTexture(), NULL, NULL, &wPU, &hPU);
			SDL_QueryTexture(player.getTexture(), NULL, NULL, &wP, &hP);
			if (detectCollision(i->getX(), i->getY(), wPU, hPU, player.getX(), player.getY(), wP, hP)) {
				if (i->Type() == 1) {
					if (player.getDecay() > 0) {
						realHealth += 4;
					}
					else {
						player.setHealth(player.getHealth() + 4);
					}
				} else {
					player.setEnhance(300);
				}
				Mix_PlayChannel(-1, managerResour.powerupSound, 0);
				i = powerUps.erase(i);
				break;
			}

			i->move();
			managerRender.draw(i->getTexture(), i->getX(), i->getY());
			i->setHealth(i->getHealth() - 1);
			i++;
		}
	}

}



void Game::addExplosion(int x, int y, int type) {
	explosion.setTexture(explosionTexture);
	explosion.setX(x + rand() % 32);
	explosion.setY(y + rand() % 32);
	explosion.setDX(0); explosion.setDY(0);
	explosion.setType(type);
	std::vector<Effect> tmp;
	for (int j = 0; j < 10; j++) {
		switch (rand() % 4) {
		case 0:
			explosion.setRGBA(255, 255, 0, 200); //YELLOW
			break;
		case 1:
			explosion.setRGBA(255, 0, 0, 200); //RED
			break;
		case 2:
			explosion.setRGBA(255, 128, 0, 200); //ORANGE
			break;
		case 3:
			explosion.setRGBA(255, 255, 255, 200); //WHITE
			break;
		}
		tmp.push_back(explosion);
	}
	Explosion.push_back(tmp);
}



void Game::addPowerUp(int x, int y, int type) {
	if(type == 1) powerup.setTexture(gameUI.powerUp1);
	else if(type == 2) powerup.setTexture(gameUI.powerUp2);
	powerup.setX(x + 20);
	powerup.setY(y + 20);
	powerup.setDX( -rand() % 5  - 1);
	powerup.setDY(rand() % 5 + 1);
	powerup.setHealth(150);
	powerup.setType(type);
	powerUps.push_back(powerup);
}

bool Game::detectCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	return (max(x1, x2) < min(x1 + w1, x2 + w2)) && (max(y1, y2) < min(y1 + h1, y2 + h2));
}

