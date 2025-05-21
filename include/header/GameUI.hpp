#pragma once
#include "header/common.h"
#include "header/ManagerRender.hpp"
#include "header/Player.hpp"    
#include "header/ManagerResour.hpp"
#include "header/Enemy.hpp"
#include "header/Button.hpp"
#include "header/Entity.hpp"
#include "header/Effect.hpp"

class GameUI {
public:
    SDL_Texture* guide;
    SDL_Texture* pauseIcon;
    SDL_Texture* lock;
    SDL_Texture* healthbar;
    SDL_Texture* bar;
    SDL_Texture* powerUp1;
    SDL_Texture* powerUp2;

    Button menuButton[3];
	Button endButton[2]; 
	Button arrowButton[2];
	Button backButton;
	Button playButton;

    std::vector <Enemy*>& Enemies;

    //score
	int score, highScore;
	bool surpass = 0;
	SDL_Texture* scoreTXT = NULL;
	SDL_Texture* hsTXT = NULL;
	SDL_Texture* LPTXT =  NULL;
	SDL_Texture* stuntTXT = NULL;
	SDL_Texture* requireTXT = NULL;
	SDL_Texture* currTXT = NULL;
	SDL_Texture* pauseTXT = NULL;
    std::vector<Entity>& powerUps;
	SDL_Rect srclock;
	std::stringstream scoreText, hScoreText, LPText;

    std::vector <std::vector<Effect>>& Explosion;

    std::vector <Entity>& Bullets;
    Entity& playerBullet;
    ManagerRender& managerRender;
    Player& player;
    ManagerResour& managerResour;
    GameUI(ManagerRender& render,ManagerResour& resources, Player& player,Entity& playerBullet,std::vector <Entity>& Bullets_,std::vector <std::vector<Effect>> &Explosion,std::vector<Entity> &powerUps,std::vector <Enemy*>& Enemies);
    ~GameUI() {
        SDL_DestroyTexture(scoreTXT);
        SDL_DestroyTexture(hsTXT);
        SDL_DestroyTexture(LPTXT);
        SDL_DestroyTexture(stuntTXT);
        SDL_DestroyTexture(requireTXT);
        SDL_DestroyTexture(currTXT);
        SDL_DestroyTexture(pauseTXT);
    }
    void setScore(int score) { this->score = score; }
    void titleScreen(int& realHealth, int& fightingBoss, int& enemySpawnTimer, bool& gPlaying);
    void setUpMenu();
	bool chooseSpaceship(int& realHealth, int& fightingBoss, int& enemySpawnTimer, bool& gPlaying);
	void Guide(int& realHealth, int& fightingBoss, int& enemySpawnTimer, bool& gPlaying);
    void HUD(double& currbossHP, double& bossHP,int & fightingBoss);
    void endScreen(int& realHealth, int& fightingBoss, int& enemySpawnTimer, bool& gPlaying, bool& playAgain);
    void presentScene(int& realHealth, int& fightingBoss, int& enemySpawnTimer, bool& gPlaying);
    Player getPlayer();
};