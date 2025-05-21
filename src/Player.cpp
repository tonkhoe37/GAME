#include "header/common.h"
#include "header/Player.hpp"

Player::Player() {
	up = left = down = right = fire = reload = enhance = 0;
	RGBTimer = stunt = decay = 0;
	setX(SCREEN_WIDTH / 2);
	setY(SCREEN_HEIGHT / 2 - 50);
	setHealth(10);
	setEnhance(0);
	setDecay(0);
	setStunt(0);
	resetInput(); 
}

void Player:: restoreInitPlayer(){
	setX(SCREEN_WIDTH / 2);
	setY(SCREEN_HEIGHT / 2 - 50);
	setHealth(1);
	setEnhance(0);
	setDecay(0);
	setStunt(0);
	resetInput();
}

void Player::move() {
	if (up && left) {
		dy = -PLAYER_SPEED;
		dx = -PLAYER_SPEED;
	} else if (up && right) {
		dy = -PLAYER_SPEED;
		dx = PLAYER_SPEED;
	} else if (down && left) {
		dy = PLAYER_SPEED;
		dx = -PLAYER_SPEED;
	} else if (down && right) {
		dy = PLAYER_SPEED;
		dx = PLAYER_SPEED;
	} else if (up) {
		dy = -PLAYER_SPEED;
	} else if (down) {
		dy = PLAYER_SPEED;
	}
	else if (left) {
		dx = -PLAYER_SPEED;
	}
	else if (right) {
		dx = PLAYER_SPEED;
	}

	x = (x + dx < 0 ? 0 : (x + dx > 1200 ? 1200 : x + dx));
	y = (y + dy < 0 ? 0 : (y + dy > 640 ? 640 : y + dy));
	dx = dy = 0;
}

bool Player::Fire() {
	return fire;
}

int Player::getReload() {
	return reload;
}

void Player::setReload(int reload) {
	this->reload = reload;
}

int Player::getEnhance() {
	return enhance;
}

void Player::setEnhance(int enhance) {
	this->enhance = enhance;
}

void Player::keyDown(SDL_KeyboardEvent* event) {
	if (!(event->repeat)) {
		switch (event->keysym.scancode) {
		case SDL_SCANCODE_UP:
			up = true;
			break;
		case SDL_SCANCODE_DOWN:
			down = true;
			break;
		case SDL_SCANCODE_LEFT:
			left = true;
			break;
		case SDL_SCANCODE_RIGHT:
			right = true;
			break;
		case SDL_SCANCODE_LCTRL:
			fire = true;
			break;
		}
	}
}

void Player::keyUp(SDL_KeyboardEvent* event) {
	if (!(event->repeat)) {
		switch (event->keysym.scancode) {
		case SDL_SCANCODE_UP:
			up = false;
			break;
		case SDL_SCANCODE_DOWN:
			down = false;
			break;
		case SDL_SCANCODE_LEFT:
			left = false;
			break;
		case SDL_SCANCODE_RIGHT:
			right = false;
			break;
		case SDL_SCANCODE_LCTRL:
			fire = false;
			break;
		}
	}
}

int Player::getRGBTimer() {
	return this->RGBTimer;
}
void Player::setRGBTimer(int time) {
	this->RGBTimer = time;
}


void Player::resetInput() {
	left = up = right = down = fire =  enhance = RGBTimer = false;
}

int Player::getReloadRate() {
	return reloadRate;
}
void Player::setReloadRate(int reloadRate) {
	this->reloadRate = reloadRate;
}

int Player::getDamageRate() {
	return damageRate;
}
void Player::setDamageRate(int damageRate) {
	this->damageRate = damageRate;
}

int Player::getStunt() {
	return stunt;
}
void Player::setStunt(int stunt) {
	this->stunt = stunt;
}
int Player::getDecay() {
	return decay;
}
void Player::setDecay(int decay) {
	this->decay = decay;
}

