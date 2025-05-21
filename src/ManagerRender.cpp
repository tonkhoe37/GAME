#include <header/ManagerRender.hpp>

ManagerRender ::ManagerRender() {
    backgroundX = 0;
}

SDL_Texture* ManagerRender::loadTexture(std::string path) {
	SDL_Texture* texture = IMG_LoadTexture(gRenderer, path.c_str());
	if (texture == NULL) {
		std :: cout << "Error loading image : " << IMG_GetError() << std :: endl;
		exit(-1);
	}
	return texture;
}


void ManagerRender::draw(SDL_Texture* texture, int x, int y) {
	SDL_Rect target;
	target.x = x;
	target.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &target.w, &target.h);
	SDL_RenderCopy(gRenderer, texture, NULL, &target);
}

void ManagerRender::drawRect(SDL_Texture* texture, SDL_Rect* src, int x, int y) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = src->w;
	dest.h = src->h;

	SDL_RenderCopy(gRenderer, texture, NULL, &dest);
}

void ManagerRender ::drawBackground() {
	int w, h;
	SDL_QueryTexture(gBackground, NULL, NULL, &w, &h);
	if (-w > --backgroundX) {
		backgroundX = 0;
	}
	draw(gBackground, backgroundX, 0);
	draw(gBackground, backgroundX + w, 0);
}

void ManagerRender ::prepareScene() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);
}

void ManagerRender::setBackgroundX(int x) {
    backgroundX = x;
}

int ManagerRender::getBackgroundX() {
    return backgroundX;
}