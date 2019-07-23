#pragma once

#include <map>
#include <string>

struct SDL_Window;
struct SDL_Renderer;

class Graphics {
public:
    Graphics();

    ~Graphics();

    /// Loads an image into the spriteSheets map if it doesn't already exists.
    /// As a result, each image will only be loaded once.
    /// \return Returns the image from the map regradless of weather or not it was just loaded
    SDL_Surface *loadImage(const std::string &filePath);

    /// Draws the texture to a certain part of the screen.
    /// \param texture
    /// \param srcRect part of texture to draw
    /// \param dstRect rectangle where on screen to draw texture
    void blitSurface(SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect);

    /// Renders everything to the screen.
    void flip();

    /// Clears the screen.
    void clear();

    /// Returns the renderer
    SDL_Renderer *getRenderer() const;

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::map<std::string, SDL_Surface *> spriteSheets;
};
