#include "header/ManagerResour.hpp"

ManagerResour::ManagerResour(ManagerRender& render) {
    managerRender = render;
}

void ManagerResour :: loadResources() {

	gBackground = managerRender.loadTexture("./resources/gfx/other/background.png");
    
	shipModel[0] = managerRender.loadTexture("./resources/gfx/spaceship/white.png");
	shipModel[1] = managerRender.loadTexture("./resources/gfx/spaceship/yellow-white.png");
	shipModel[2] = managerRender.loadTexture("./resources/gfx/spaceship/blue.png");
	shipModel[3] = managerRender.loadTexture("./resources/gfx/spaceship/green.png");
	shipModel[4] = managerRender.loadTexture("./resources/gfx/spaceship/rship.png");

	bulletModel[0] = managerRender.loadTexture("./resources/gfx/bulletsprites/white.png");
	bulletModel[1] = managerRender.loadTexture("./resources/gfx/bulletsprites/yellow-white.png");
	bulletModel[2] = managerRender.loadTexture("./resources/gfx/bulletsprites/blue.png");
	bulletModel[3] = managerRender.loadTexture("./resources/gfx/bulletsprites/green.png");
	bulletModel[4] = managerRender.loadTexture("./resources/gfx/bulletsprites/rship.png");

	enemyBoss[0] = managerRender.loadTexture("./resources/gfx/spaceship/enemyBoss1.png");
	enemyBoss[1] = managerRender.loadTexture("./resources/gfx/spaceship/enemyBoss2.png");
	enemyBoss[2] = managerRender.loadTexture("./resources/gfx/spaceship/enemyBoss3.png");

	bossBullet[0] = managerRender.loadTexture("./resources/gfx/bulletsprites/enemyBoss1.png");
	bossBullet[1] = managerRender.loadTexture("./resources/gfx/bulletsprites/enemyBoss2.png");
	bossBullet[2] = managerRender.loadTexture("./resources/gfx/bulletsprites/enemyBoss3.png");

	Buttons[0] = managerRender.loadTexture("./resources/gfx/gui/startButton.png");
	Buttons[2] = managerRender.loadTexture("./resources/gfx/gui/guideButton.png");
	Buttons[4] = managerRender.loadTexture("./resources/gfx/gui/exitButton.png");
	Buttons[6] = managerRender.loadTexture("./resources/gfx/gui/left.png");
	Buttons[8] = managerRender.loadTexture("./resources/gfx/gui/right.png");
	Buttons[10] = managerRender.loadTexture("./resources/gfx/gui/backButton.png");
	Buttons[12] = managerRender.loadTexture("./resources/gfx/gui/menuButton.png");
	Buttons[14] = managerRender.loadTexture("./resources/gfx/gui/playButton.png");
	Buttons[16] = managerRender.loadTexture("./resources/gfx/gui/play.png");

 
	Buttons[1] = managerRender.loadTexture("./resources/gfx/gui/startHoverButton.png");
	Buttons[3] = managerRender.loadTexture("./resources/gfx/gui/guideHoverButton.png");
	Buttons[5] = managerRender.loadTexture("./resources/gfx/gui/exitHoverButton.png");
	Buttons[7] = managerRender.loadTexture("./resources/gfx/gui/hoverleft.png");
	Buttons[9] = managerRender.loadTexture("./resources/gfx/gui/hoverright.png");
	Buttons[11] = managerRender.loadTexture("./resources/gfx/gui/backHoverButton.png");
	Buttons[13] = managerRender.loadTexture("./resources/gfx/gui/menuHoverButton.png");
	Buttons[15] = managerRender.loadTexture("./resources/gfx/gui/playHoverButton.png");
	Buttons[17] = managerRender.loadTexture("./resources/gfx/gui/hoverplay.png");
	//music and sound
	music = Mix_LoadMUS("./resources/music/backgroundMusic.ogg");
	explosionSound = Mix_LoadWAV("./resources/sound/enemyExplosion.ogg");
	playerExplosionSound = Mix_LoadWAV("./resources/sound/explosionSound.wav");
	buttonSound = Mix_LoadWAV("./resources/sound/buttonSound.mp3");
	bulletSound = Mix_LoadWAV("./resources/sound/bulletSound.ogg");
	powerupSound = Mix_LoadWAV("./resources/sound/earnPowerUp.ogg");
	//font
	font = TTF_OpenFont("./resources/font/Goldeneye.ttf", 28);
	titleFont = TTF_OpenFont("./resources/font/Ghost.ttf", 90);
	Mix_PlayMusic(music, -1);

}