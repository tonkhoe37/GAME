#pragma once
#include "header/common.h"

class ManagerRender {
private:
    int backgroundX;
public:
    ManagerRender();
    void draw(SDL_Texture*, int, int);
    void drawRect(SDL_Texture*, SDL_Rect*, int, int);
    void drawBackground();
    void prepareScene();
    SDL_Texture* loadTexture(std::string);
    void setBackgroundX(int x);
    int getBackgroundX();   
};