#include "header/GameUI.hpp"

GameUI::GameUI(ManagerRender& render,
	ManagerResour& resources,
	Player& player,
	Entity& playerBullet,
	std::vector<Entity>& Bullets_,
	std::vector<std::vector<Effect>>& Explosion_,
	std::vector<Entity>& powerUps_,
	std::vector<Enemy*>& Enemies_)
    : managerRender(render),
	managerResour(resources),
	player(player),
	playerBullet(playerBullet),
	Bullets(Bullets_),
	Explosion(Explosion_),
	powerUps(powerUps_),
	Enemies(Enemies_) {
	
	score = 0;
	std::cout << "GameUI Constructor Start\n";
    std::ifstream fin("./data/scores.txt");
    if (!fin.is_open()) {
        std::cerr << "Can't open file scores.txt" << std::endl;
        highScore = 0;
    } else {
        fin >> highScore;
        fin.close();
    }
}



void GameUI::setUpMenu() {
    int w, h;
	menuButton[0].setTexture(managerResour.getButton(0));
	menuButton[1].setTexture(managerResour.getButton(2));
	menuButton[2].setTexture(managerResour.getButton(4));
	for (int i = 0; i < 3; i++) {
		menuButton[i].setX(475);
		menuButton[i].setY(SCREEN_WIDTH / 4 + 25);
		SDL_QueryTexture(menuButton[i].getTexture(), NULL, NULL, &w, &h);
		menuButton[i].setW(w);
		menuButton[i].setH(h);
		if (i) {
			menuButton[i].setY(menuButton[i - 1].getY() + menuButton[i - 1].getH() + 25);
		}
	}

	arrowButton[0].setTexture(managerResour.getButton(6));
	arrowButton[1].setTexture(managerResour.getButton(8));
	for (int i = 0; i < 2; i++) {
		arrowButton[i].setX(300);
		arrowButton[i].setY(300);
		SDL_QueryTexture(menuButton[i].getTexture(), NULL, NULL, &w, &h);
		arrowButton[i].setW(w);
		arrowButton[i].setH(h);
		if (i) {
			arrowButton[i].setX(arrowButton[i - 1].getX() + 575);
		}
	}

	backButton.setY(600);
	backButton.setX(450);
	backButton.setTexture(managerResour.getButton(10));
	SDL_QueryTexture(backButton.getTexture(), NULL, NULL, &w, &h);
	backButton.setW(w);
	backButton.setH(h);

	endButton[0].setTexture(managerResour.getButton(12));
	endButton[1].setTexture(managerResour.getButton(14));
	for (int i = 0; i < 2; i++) {
		endButton[i].setX(SCREEN_WIDTH / 4 + 25);
		endButton[i].setY(500);
		SDL_QueryTexture(endButton[i].getTexture(), NULL, NULL, &w, &h);
		endButton[i].setW(w);
		endButton[i].setH(h);
		if (i) {
			endButton[i].setX(endButton[i - 1].getX() + endButton[i - 1].getW() + 50);
		}
	}

	pauseIcon = managerRender.loadTexture("./resources/gfx/other/pauseIcon.png");
	lock = managerRender.loadTexture("./resources/gfx/other/lock.png");
	guide = managerRender.loadTexture("./resources/gfx/other/guide.png");
	healthbar = managerRender.loadTexture("./resources/gfx/other/healthbar.png");
	bar = managerRender.loadTexture("./resources/gfx/other/bar.png");
	powerUp1 = managerRender.loadTexture("./resources/gfx/other/LP.png");
	powerUp2 = managerRender.loadTexture("./resources/gfx/other/enhanceATK.png");
	srclock.w = srclock.h = 200;
}

void GameUI::HUD(double& currbossHP, double& bossHP, int& fightingBoss ) {

	if (score > highScore) surpass = 1;

	highScore = std::max(highScore, score);
	scoreText << "Score : " << score;
	hScoreText << "High Score : " << highScore;

	SDL_Surface* scoreSurface = TTF_RenderText_Solid(managerResour.font, scoreText.str().c_str(), { 255, 255, 255, 0 });
	SDL_DestroyTexture(scoreTXT); 
	scoreTXT = SDL_CreateTextureFromSurface(gRenderer, scoreSurface);
	SDL_FreeSurface(scoreSurface);

	SDL_Surface* hsSurface = TTF_RenderText_Solid(managerResour.font, hScoreText.str().c_str(), { 181, 13, 13, 0 });
	SDL_DestroyTexture(hsTXT);
	hsTXT = SDL_CreateTextureFromSurface(gRenderer, hsSurface);
	SDL_FreeSurface(hsSurface);

	LPText << "Life Points: " << player.getHealth();
	if (player.getDecay()) LPText << " (DECAY)";

	SDL_Surface* LPSurface = TTF_RenderText_Solid(managerResour.font, LPText.str().c_str(), { 22, 184, 17, 0 });
	if (player.getDecay()) LPSurface = TTF_RenderText_Solid(managerResour.font, LPText.str().c_str(), { 255, 99, 71, 0 });

	SDL_DestroyTexture(LPTXT);
	LPTXT = SDL_CreateTextureFromSurface(gRenderer, LPSurface);
	SDL_FreeSurface(LPSurface);

	managerRender.draw(scoreTXT, 20, 20);
	managerRender.draw(hsTXT, 20, 60);
	managerRender.draw(LPTXT, 20, 100);

	if (player.getStunt()) {
		std::string stunt = "STUNT";
		SDL_Surface* stuntSf = TTF_RenderText_Solid(managerResour.font, stunt.c_str(), { 255, 200, 0, 0 });
		SDL_DestroyTexture(stuntTXT);
		stuntTXT = SDL_CreateTextureFromSurface(gRenderer, stuntSf);
		SDL_FreeSurface(stuntSf);
		managerRender.draw(stuntTXT, player.getX(), player.getY() - 25);
	}

	if (fightingBoss) {
		currbossHP = bossHP;
		for (auto i = Enemies.begin(); i != Enemies.end(); i++) {
			if ((*i)->getType() == 2) {
				currbossHP = (*i)->getHealth();
				break;
			}
		}

		SDL_Rect rect{ 500, 500, 400, 22};
		SDL_Rect rect2{ 502, 502, currbossHP/bossHP * 392, 18 };
		
		managerRender.drawRect(bar, &rect, 850, 650);
		managerRender.drawRect(healthbar, &rect2, 854, 652);
	}
}

void GameUI::titleScreen(int& realHealth, int& fightingBoss, int& enemySpawnTimer, bool& gPlaying) { 
	SDL_Event e;
	std::string title1 = "Star ", title2 = "Force";
	std::string instruct = "Press Space to protect the galaxy!";
	SDL_Surface* titleSpace = TTF_RenderText_Solid(managerResour.titleFont, title1.c_str(), { 255, 255, 255, 0 });
	SDL_Texture* tSTXT = SDL_CreateTextureFromSurface(gRenderer, titleSpace);

	SDL_Surface* titleShooter = TTF_RenderText_Solid(managerResour.titleFont, title2.c_str(), { 255, 255, 255, 0 });
	SDL_Texture* tShTXT = SDL_CreateTextureFromSurface(gRenderer, titleShooter);

	SDL_Surface* pressSf = TTF_RenderText_Solid(managerResour.font, instruct.c_str(), { 255, 255, 255, 0 });
	SDL_Texture* pressTXT = SDL_CreateTextureFromSurface(gRenderer, pressSf);
	while (true) {
		managerRender.drawBackground();
		managerRender.draw(tSTXT, SCREEN_WIDTH/4, 100);
		managerRender.draw(tShTXT, SCREEN_WIDTH/4 + 100, 200);
		managerRender.draw(pressTXT, SCREEN_WIDTH / 4 + 75, 300);
		for (int i = 0; i < 3; i++) {
			managerRender.draw(menuButton[i].getTexture(), menuButton[i].getX(), menuButton[i].getY());
		}
		presentScene(realHealth, fightingBoss, enemySpawnTimer, gPlaying);
		int x, y;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				exit(0);
				break;

			case SDL_KEYDOWN:
				if (e.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					if (chooseSpaceship(realHealth, fightingBoss, enemySpawnTimer, gPlaying)) return;
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					exit(0);
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_M) {
					if (Mix_PausedMusic()) {
						Mix_ResumeMusic();
					}
					else {
						Mix_PauseMusic();
					}
				}
				break;

			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&x, &y);
				for (int i = 0; i < 3; i++) {
					if (x >= menuButton[i].getX() && x <= menuButton[i].getX() + menuButton[i].getW()
						&& y >= menuButton[i].getY() && y <= menuButton[i].getY() + menuButton[i].getH()) {
						if (!menuButton[i].isHovered()) {
							menuButton[i].setHovered(true);
							if (i == 0) menuButton[i].setTexture(managerResour.getButton(1));
							if (i == 1) menuButton[i].setTexture(managerResour.getButton(3));
							if (i == 2) menuButton[i].setTexture(managerResour.getButton(5));
						}
					}
					else if (menuButton[i].isHovered()) {
						menuButton[i].setHovered(false);
						if (i == 0) menuButton[i].setTexture(managerResour.getButton(0));
						if (i == 1) menuButton[i].setTexture(managerResour.getButton(2));
						if (i == 2) menuButton[i].setTexture(managerResour.getButton(4));
					}
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&x, &y);
				for (int i = 0; i < 3; ++i) {
					if (x >= menuButton[i].getX() && x <= menuButton[i].getX() + menuButton[i].getW()) {
						if (y >= menuButton[i].getY() && y <= menuButton[i].getY() + menuButton[i].getH()) {
							Mix_PlayChannel(-1, managerResour.buttonSound, 0);
							if (i == 0) {
								if (chooseSpaceship(realHealth, fightingBoss, enemySpawnTimer,gPlaying)) return;
							}
							else if (i == 1) {
								Guide(realHealth, fightingBoss, enemySpawnTimer,gPlaying);
							}
							else {
								exit(0);
							}
						}
					}
				}
				break;
			}
		}
	}
}


bool GameUI::chooseSpaceship(int& realHealth, int& fightingBoss, int& enemySpawnTimer, bool& gPlaying) {
	SDL_Event e;
	int w, h;
	backButton.setY(550);
	backButton.setX(350);
	backButton.setTexture(managerResour.getButton(10));
	SDL_QueryTexture(backButton.getTexture(), NULL, NULL, &w, &h);
	backButton.setW(w);
	backButton.setH(h);

	playButton.setY(550);
	playButton.setX(350 + backButton.getW() + 50);
	playButton.setTexture(managerResour.getButton(16));
	SDL_QueryTexture(playButton.getTexture(), NULL, NULL, &w, &h);
	playButton.setW(w);
	playButton.setH(h);

	std:: string instruct = "Choose your spaceshooter";
	SDL_Surface* insSf = TTF_RenderText_Solid(managerResour.font, instruct.c_str(), { 255, 255, 255, 0 });
	SDL_Texture* insTXT = SDL_CreateTextureFromSurface(gRenderer, insSf);

	int pos = 0;
	SDL_Rect src;
	src.w = src.h = 300;

	while (true) {
		managerRender.drawBackground();
		managerRender.draw(insTXT, SCREEN_WIDTH / 4 + 150, 100);
		managerRender.draw(backButton.getTexture(), backButton.getX(), backButton.getY());
		managerRender.draw(playButton.getTexture(), playButton.getX(), playButton.getY());

		if (pos == 0 || highScore >= pos * 25 + (pos - 1) * (pos - 1) * 5) {
			managerRender.drawRect(managerResour.getShipModel(pos), &src, SCREEN_WIDTH / 4 + 175, 200);
		}
		else {
			std::string unlockRequire = "Reach at least " + std::to_string(pos * 25 + (pos - 1) * (pos - 1) * 5) + " high score to unlock";
			std::string currHS = "Your curent high score: " + std::to_string(highScore);

			SDL_Surface* requireSf = TTF_RenderText_Solid(managerResour.font, unlockRequire.c_str(), { 255, 0, 0, 0 });
			SDL_DestroyTexture(requireTXT);
			requireTXT = SDL_CreateTextureFromSurface(gRenderer, requireSf);
			SDL_FreeSurface(requireSf);

			SDL_Surface* currSf = TTF_RenderText_Solid(managerResour.font, currHS.c_str(), { 255, 0, 0, 0 });
			SDL_DestroyTexture(currTXT);
			currTXT = SDL_CreateTextureFromSurface(gRenderer, currSf);
			SDL_FreeSurface(currSf);

			managerRender.draw(requireTXT, SCREEN_WIDTH / 4 + 60, 425);
			managerRender.draw(currTXT, SCREEN_WIDTH / 4 + 150, 470);
			managerRender.drawRect(lock, &srclock, SCREEN_WIDTH / 4 + 225, 200); 
		}


		for (int i = 0; i < 2; i++) {
			managerRender.draw(arrowButton[i].getTexture(), arrowButton[i].getX(), arrowButton[i].getY());
		}
		presentScene(realHealth, fightingBoss, enemySpawnTimer,gPlaying);

		int x, y;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				exit(0);
				break;

			case SDL_KEYDOWN:
				if (e.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					if (pos == 0 || highScore >= pos * 25 + (pos - 1) * (pos - 1) * 5) {
						player.setTexture(managerResour.getShipModel(pos));
						playerBullet.setTexture(managerResour.getBulletModel(pos));

						if (pos < 2) player.setDamageRate(1);
						else if (pos < 4) player.setDamageRate(3);
						else player.setDamageRate(5);

						if (pos % 2 == 0) player.setReloadRate(5);
						else player.setReloadRate(4);
						if (pos == 4) player.setReloadRate(3);

						gPlaying = true;
						player.restoreInitPlayer();
						return true;
					}
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					pos = (pos + 1) % 5;
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					if (pos > 0) pos = (pos - 1) % 5;
					else pos = 4;
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					exit(0);
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_M) {
					if (Mix_PausedMusic()) {
						Mix_ResumeMusic();
					}
					else {
						Mix_PauseMusic();
					}
				}
				break;

			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&x, &y);
				for (int i = 0; i < 2; i++) {
					if (x >= arrowButton[i].getX() && x <= arrowButton[i].getX() + arrowButton[i].getW()
						&& y >= arrowButton[i].getY() && y <= arrowButton[i].getY() + arrowButton[i].getH()) {
						if (!arrowButton[i].isHovered()) {
							arrowButton[i].setHovered(true);
							if (i == 0) arrowButton[i].setTexture(managerResour.getButton(7));
							if (i == 1) arrowButton[i].setTexture(managerResour.getButton(9));
						}
					}
					else if (arrowButton[i].isHovered()) {
						arrowButton[i].setHovered(false);
						if (i == 0) arrowButton[i].setTexture(managerResour.getButton(6));
						else
						if (i == 1) arrowButton[i].setTexture(managerResour.getButton(8));
					}
				}

				if (x >= backButton.getX() && x <= backButton.getX() + backButton.getW()
					&& y >= backButton.getY() && y <= backButton.getY() + backButton.getH()) {
					if (!backButton.isHovered()) {
						backButton.setHovered(true);
						backButton.setTexture(managerResour.getButton(11));
					}
				}
				else if (backButton.isHovered()) {
					backButton.setHovered(false);
					backButton.setTexture(managerResour.getButton(10));
				}

				if (x >= playButton.getX() && x <= playButton.getX() + playButton.getW()
					&& y >= playButton.getY() && y <= playButton.getY() + playButton.getH()) {
					if (!playButton.isHovered()) {
						playButton.setHovered(true);
						playButton.setTexture(managerResour.getButton(17));
					}
				}
				else if (playButton.isHovered()) {
					playButton.setHovered(false);
					playButton.setTexture(managerResour.getButton(16));
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&x, &y);
				for (int i = 0; i < 2; ++i) {
					if (x >= arrowButton[i].getX() && x <= arrowButton[i].getX() + arrowButton[i].getW()) {
						if (y >= arrowButton[i].getY() && y <= arrowButton[i].getY() + arrowButton[i].getH()) {
							Mix_PlayChannel(-1, managerResour.buttonSound, 0);
							if (i == 0) {
								if (pos > 0) pos = (pos - 1) % 5;
								else pos = 4;
							}
							else {
								pos = (pos + 1) % 5;
							}
						}
					}
				}
				if (x >= backButton.getX() && x <= backButton.getX() + backButton.getW()
					&& y >= backButton.getY() && y <= backButton.getY() + backButton.getH()) {
					Mix_PlayChannel(-1, managerResour.buttonSound, 0);
					return false;
				}
				if (x >= playButton.getX() && x <= playButton.getX() + playButton.getW()
					&& y >= playButton.getY() && y <= playButton.getY() + playButton.getH()) {
					Mix_PlayChannel(-1, managerResour.buttonSound, 0);
					if (pos == 0 || highScore >= pos * 25 + (pos - 1) * (pos - 1) * 5) {
						player.setTexture(managerResour.getShipModel(pos));
						playerBullet.setTexture(managerResour.getBulletModel(pos));

						if (pos < 2) player.setDamageRate(1);
						else if (pos < 4) player.setDamageRate(3);
						else player.setDamageRate(5);

						if (pos % 2 == 0) player.setReloadRate(5);
						else player.setReloadRate(4);
						if (pos == 4) player.setReloadRate(3);

						gPlaying = true;
						player.restoreInitPlayer();
						return true;
					}
				}

				break;
			}
		}
	}

}

void GameUI::Guide(int& realHealth, int& fightingBoss, int& enemySpawnTimer, bool& gPlaying) {
	SDL_Event e;
	backButton.setY(600);
	backButton.setX(450);
	backButton.setTexture(managerResour.getButton(10));
	int w, h;
	SDL_QueryTexture(backButton.getTexture(), NULL, NULL, &w, &h);
	backButton.setW(w);
	backButton.setH(h);


	int x, y;
	while (true) {
		managerRender.drawBackground();
		managerRender.draw(backButton.getTexture(), backButton.getX(), backButton.getY());
		managerRender.draw(guide, 0, 25);
		presentScene(realHealth, fightingBoss, enemySpawnTimer,gPlaying);

		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					return;
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_M) {
					if (Mix_PausedMusic()) {
						Mix_ResumeMusic();
					}
					else {
						Mix_PauseMusic();
					}
				}
				break;
			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&x, &y);
				if (x >= backButton.getX() && x <= backButton.getX() + backButton.getW()
				&& y >= backButton.getY() && y <= backButton.getY() + backButton.getH()) {
					if (!backButton.isHovered()) {
						backButton.setHovered(true);
						backButton.setTexture(managerResour.getButton(11));
					}
				} else if (backButton.isHovered()) {
					backButton.setHovered(false);
					backButton.setTexture(managerResour.getButton(10));
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&x, &y);
				if (x >= backButton.getX() && x <= backButton.getX() + backButton.getW()
				&& y >= backButton.getY() && y <= backButton.getY() + backButton.getH()) {
					Mix_PlayChannel(-1, managerResour.buttonSound, 0);
					return;
				}
				break;
			}
		}
	}
}

void GameUI::endScreen(int& realHealth, int& fightingBoss, int& enemySpawnTimer, bool& gPlaying, bool& playAgain) {
	SDL_Event e;
	std::string gameover = "GAME OVER";
	std::string endScore = "Your score: " + std::to_string(score);
	std::string congrat = "Congrat. You achieve new high score!";

	SDL_Surface* endSf = TTF_RenderText_Solid(managerResour.titleFont, gameover.c_str(), { 255, 255, 255, 0 });
	SDL_Texture* endTXT = SDL_CreateTextureFromSurface(gRenderer, endSf);

	SDL_Surface* scoreSf = TTF_RenderText_Solid(managerResour.font, endScore.c_str(), { 255, 255, 255, 0 });
	SDL_Texture* scoreTXT = SDL_CreateTextureFromSurface(gRenderer, scoreSf);

	SDL_Surface* congratSf = TTF_RenderText_Solid(managerResour.font, congrat.c_str(), { 232, 46, 46, 0 });
	SDL_Texture* congratTXT = SDL_CreateTextureFromSurface(gRenderer, congratSf);

	if (surpass) {
		std::ofstream fout("./data/scores.txt");
		fout << highScore << std::endl;
		fout.close();
	}

	while (true) {
		managerRender.drawBackground();
		managerRender.draw(endTXT, SCREEN_WIDTH / 4, 200);
		managerRender.draw(scoreTXT, SCREEN_WIDTH / 4 + 225, 350);
		if (surpass) {
			managerRender.draw(congratTXT, SCREEN_WIDTH / 4 + 100, 400);
		}

		for (int i = 0; i < 2; i++) {
			managerRender.draw(endButton[i].getTexture(), endButton[i].getX(), endButton[i].getY());
		}
		presentScene(realHealth, fightingBoss, enemySpawnTimer,gPlaying);

		int x, y;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT:
					exit(0);
					break;

				case SDL_KEYDOWN:
					if (e.key.keysym.scancode == SDL_SCANCODE_SPACE) {
						enemySpawnTimer = 60;
						surpass = 0;
						score = 0;
						playAgain = true;
						gPlaying = true;
						player.restoreInitPlayer();
						return;
					}
					if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
						exit(0);
					}
					if (e.key.keysym.scancode == SDL_SCANCODE_M) {
						if (Mix_PausedMusic()) {
							Mix_ResumeMusic();
						}
						else {
							Mix_PauseMusic();
						}
					}
					break;

				case SDL_MOUSEMOTION:
					SDL_GetMouseState(&x, &y);
					for (int i = 0; i < 2; i++) {
						if (x >= endButton[i].getX() && x <= endButton[i].getX() + endButton[i].getW()
							&& y >= endButton[i].getY() && y <= endButton[i].getY() + endButton[i].getH()) {
							if (!endButton[i].isHovered()) {
								endButton[i].setHovered(true);
								if (i == 0) endButton[i].setTexture(managerResour.getButton(13));
								if (i == 1) endButton[i].setTexture(managerResour.getButton(15));
							}
						}
						else if (endButton[i].isHovered()) {
							endButton[i].setHovered(false);
							if (i == 0) endButton[i].setTexture(managerResour.getButton(12));
							else
							if (i == 1) endButton[i].setTexture(managerResour.getButton(14));
						}
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					SDL_GetMouseState(&x, &y);
					for (int i = 0; i < 2; i++) {
						if (x >= endButton[i].getX() && x <= endButton[i].getX() + endButton[i].getW()
							&& y >= endButton[i].getY() && y <= endButton[i].getY() + endButton[i].getH()) {
							    Mix_PlayChannel(-1, managerResour.buttonSound, 0);
								enemySpawnTimer = 60;
								surpass = 0;
								score = 0;
								if (i == 0) {
									playAgain = false;
								} else {
									playAgain = true;
									gPlaying = true;
									player.restoreInitPlayer();
								}
								return;
						}
					}
					break;

			}
		}

	}

}

void GameUI::presentScene(int& realHealth, int& fightingBoss, int& enemySpawnTimer, bool& gPlaying){
	if (enemySpawnTimer > 0) {
	enemySpawnTimer--;	
	}

	if (player.getReload() > 0) {
	player.setReload(player.getReload() - 1);
	}

	if (player.getEnhance() > 0) {
	player.setEnhance(player.getEnhance() - 1);
	}

	if (Explosion.empty() && player.getHealth() <= 0 && gPlaying) {
		Bullets.clear();
		Enemies.clear();
		powerUps.clear();
		SDL_SetTextureColorMod(player.getTexture(), 255, 255, 255);
		fightingBoss = 0;
		realHealth = 0;
		gPlaying = false;
	}
	scoreText.str(std::string());
	hScoreText.str(std::string());
	LPText.str(std::string());
	SDL_RenderPresent(gRenderer);
	SDL_Delay(16);
}


Player GameUI :: getPlayer(){
	return player;
}

