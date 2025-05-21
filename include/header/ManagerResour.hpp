#pragma once
#include "header/common.h"
#include "header/ManagerRender.hpp"

class ManagerResour {
    public:
    ManagerResour(ManagerRender& render);
    ManagerRender managerRender;
    SDL_Texture* shipModel[5];
	SDL_Texture* bulletModel[5];
	SDL_Texture* enemyBoss[3];
	SDL_Texture* bossBullet[3];
    SDL_Texture* Buttons[18];

    //sound and music
	Mix_Chunk* explosionSound = NULL;
	Mix_Chunk* playerExplosionSound = NULL;
	Mix_Chunk* bulletSound = NULL;
	Mix_Chunk* buttonSound =  NULL;
	Mix_Chunk* powerupSound = NULL;
	Mix_Music* music = NULL;

	//font
	TTF_Font* font = NULL;
	TTF_Font* titleFont = NULL;

    void loadResources();
    void freeResources() {
        for (int i = 0; i < 5; ++i) {
            SDL_DestroyTexture(shipModel[i]);
            SDL_DestroyTexture(bulletModel[i]);
        }
        for (int i = 0; i < 3; ++i) {
            SDL_DestroyTexture(enemyBoss[i]);
            SDL_DestroyTexture(bossBullet[i]);
        }
        for (int i = 0; i < 18; ++i) {
            SDL_DestroyTexture(Buttons[i]);
        }
        Mix_FreeChunk(explosionSound);
        Mix_FreeChunk(playerExplosionSound);
        Mix_FreeChunk(bulletSound);
        Mix_FreeChunk(buttonSound);
        Mix_FreeChunk(powerupSound);
        Mix_FreeMusic(music);
        TTF_CloseFont(font);
    }
    SDL_Texture* getShipModel(int i) {
        return shipModel[i];
    }
    SDL_Texture* getBulletModel(int i) {
        return bulletModel[i];
    }
    SDL_Texture* getEnemyBoss(int i) {
        return enemyBoss[i];
    }
    SDL_Texture* getBossBullet(int i) {
        return bossBullet[i];
    }
    SDL_Texture* getButton(int i) {
        return Buttons[i];
    }
    
};