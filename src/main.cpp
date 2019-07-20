#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

const int WIDTH = 640, HEIGHT = 360;

int main(int argc, char *argv[]) {
    SDL_Surface *imageSurface = nullptr;
    SDL_Surface *windowSurface = nullptr;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Hello SDL World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH,
                                          HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    windowSurface = SDL_GetWindowSurface(window);

    // Check that the window was successfully created
    if (nullptr == window) {
        // In the case that the window could not be made...
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "Could not create window: " << IMG_GetError() << std::endl;
        return 1;
    }

    SDL_Event windowEvent;

    imageSurface = IMG_Load("../assets/logo.png");

    if (nullptr == imageSurface) {
        std::cout << "SDL could not load image! SDL Error: " << SDL_GetError() << std::endl;
    }

    while (true) {
        if (SDL_PollEvent(&windowEvent)) {
            if (SDL_QUIT == windowEvent.type) {
                break;
            }
        }

        SDL_BlitSurface(imageSurface, nullptr, windowSurface, nullptr);

        SDL_UpdateWindowSurface(window);
    }

    SDL_FreeSurface(imageSurface);
    SDL_FreeSurface(windowSurface);

    imageSurface = nullptr;
    windowSurface = nullptr;

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
