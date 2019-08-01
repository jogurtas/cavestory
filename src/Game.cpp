#include <SDL.h>
#include <algorithm>

#include "Game.h"
#include "Input.h"

namespace {
	const int FPS = 60;
	const int MAX_FRAME_TIME = 100 / FPS;
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

	level = Level("Map 1", gfx);
	player = Player(gfx, level.getPlayerSpawnPoint());
	hud = Hud(gfx, player);

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

		if (input.isKeyHeld(SDL_SCANCODE_UP)) {
			player.lookUp();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_DOWN)) {
			player.lookDown();
		}
		if (input.wasKeyReleased(SDL_SCANCODE_UP)) {
			player.stopLookingUp();
		}
		if (input.wasKeyReleased(SDL_SCANCODE_DOWN)) {
			player.stopLookingDown();
		}

		if (input.wasKeyPressed(SDL_SCANCODE_SPACE)) {
			player.jump();
		}


		const int CURRENT_TIME_MS = SDL_GetTicks();
		const int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
		_gfx = gfx;
		update((float)std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		draw(gfx);
	}
}

void Game::draw(Graphics& graphics) {
	graphics.clear();
	level.draw(graphics);
	player.draw(graphics);
	hud.draw(graphics);
	graphics.flip();
}

void Game::update(float deltaTime) {
	player.update(deltaTime);
	level.update(deltaTime);
	hud.update((int)deltaTime);

	// Check collisions
	std::vector<Rectangle> others;
	if ((others = level.checkTileCollisions(player.getBoundingBox())).size() > 0) {
		// Player collided with at least one tile. Handle it.
		player.handleTileCollision(others);
	}

	std::vector<Slope> otherSlopes;
	if ((otherSlopes = level.checkSlopeCollisions(player.getBoundingBox())).size() > 0) {
		player.handleSlopeCollision(otherSlopes);
	}

	std::vector<Door> otherDoors = level.checkDoorCollision(player.getBoundingBox());
	if (otherDoors.size() > 0) {
		player.handleDoorCollision(otherDoors, level, _gfx);
	}
}
