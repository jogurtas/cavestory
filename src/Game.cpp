#include <SDL.h>
#include <iostream>
#include <algorithm>

#include "Game.h"
#include "Graphics.h"
#include "Input.h"

namespace {
	const int FPS = 60;
	const int MAX_FRAME_TIME = 6 * 100 / FPS;
}

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	gameLoop();
}

Game::~Game() {}

void Game::gameLoop() {
	Graphics gfx;
	Input input;
	SDL_Event event;

	player = AnimatedSprite(gfx, "../assets/sprites/MyChar.png", 0, 0, 16, 16, 100, 100, 100);
	player.setupAnimations();
	player.playAnimation("RunLeft");

	int LAST_UPDATE_TIME = SDL_GetTicks();
	while (true) {
		input.beginNewFrame();

		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT) {
				return;
			}
		}

		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
			return;
		}

		const int CURRENT_TIME_MS = SDL_GetTicks();
		const int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
		update((float)std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		draw(gfx);
	}
}

void Game::draw(Graphics& graphics) {
	graphics.clear();
	player.draw(graphics, 100, 100);
	graphics.flip();
}

void Game::update(float deltaTime) {
	player.update(deltaTime);
}
