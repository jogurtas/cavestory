#pragma once

#include <SDL.h>
#include <string>

class Graphics;

class Sprite {
public:
    Sprite();

    Sprite(Graphics &graphics, const std::string &filePath,
           int srcX, int srcY, int width, int height,
           float posX, float posY);

    virtual ~Sprite();

    virtual void update();

    void draw(Graphics &graphics, int x, int w);

protected:
    SDL_Rect srcRect;
    SDL_Texture *spriteSheet;
private:
    float x;
    float y;
};
