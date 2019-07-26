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

	level = Level("Map 1", Vector2(100, 100), gfx);
	player = Player(gfx, level.getPlayerSpawnPoint());

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
		else if (input.isKeyHeld(SDL_SCANCODE_LEFT)) {
			player.moveLeft();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
			player.moveRight();
		}
		else {
			player.stopMoving();
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
	level.draw(graphics);
	player.draw(graphics);
	graphics.flip();
}

void Game::update(float deltaTime) {
	player.update(deltaTime);
	level.update(deltaTime);

	// Check collisions
	std::vector<Rectangle> others;
	if ((others = level.checkTileCollisions(player.getBoundingBox())).size() > 0) {
		// Player collided with at least one tile. Handle it.
		player.handleTileCollision(others);
	}
}
