#include "stdafx.h"

int main(int argc, char args[]) {
	SDL_Texture* texture = nullptr;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);// add in " | SDL_RENDERER_PRESENTVSYNC" after SDL_RENDERER_ACCELERATED for vsync
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_Event e;

	Player player(renderer);
	std::vector<Asteroid> asteroids;
	std::vector<Enemy> enemies;
	std::vector<Projectile> enemyProjectiles;
	std::vector<Projectile> playerProjectiles;

	bool spawned = false;

	//Asteroid test(player.node.x, player.node.y);
	//asteroids.push_back(test);
	Enemy testboi(renderer, 0, 0, 1);

	Background background(renderer);
	Gui gui(renderer);
	Camera camera;

	events.quit = false;
	events.inGame = false;

	long int lastTime = 0;
	long int lastTime2 = 0;

	while (!events.quit) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

		events.update(e);

		menu.updateMainMenu(events.mouseX, events.mouseY, events.mouse1, events.quit, events.inGame);
		menu.drawMainMenu(renderer);

		SDL_RenderPresent(renderer);
		while (events.inGame) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

			events.update(e);

			if (player.hit) {
				lastTime2 = time(nullptr);
				player.invincible = true;
			} else if (lastTime2 == time(nullptr) - player.invincibility) {
				player.invincible = false;
			}


			if (!spawned && time(nullptr) % 5 == 0) {
				int enemyCount = rand() % 3 + 3;//between 3 and 5
				for (int i = 0; i < enemyCount; i++) {
					Enemy newEnemy(renderer, 0 + i, 0 + i, 2);
					newEnemy.shooting = true;
					enemies.push_back(newEnemy);
				}
				lastTime = time(nullptr);
				spawned = true;
			} else if (spawned && time(nullptr) == lastTime + 1) {
				spawned = false;
			}

			player.move(e, events.mouseX + camera.x, events.mouseY + camera.y);
			background.draw(renderer, camera);

			for (int i = 0; i < std::size(asteroids); i++) {
				asteroids[i].move();
				if (!player.invincible) player.asteroidCollision(asteroids[i]);
				asteroids[i].draw(renderer, camera);
			}

			for (int i = 0; i < std::size(enemies); i++) {
				enemies[i].move(player);
				enemies[i].draw(renderer, camera);
				if (enemies[i].enemyType == 2) {
					if (enemies[i].lastShotTime + enemies[i].fireRate == time(nullptr)) {
						enemies[i].shooting = true;
						enemies[i].lastShotTime = time(nullptr);
					}
					if (enemies[i].shot) {
						enemies[i].shoot(enemyProjectiles, renderer);
						enemies[i].frame = 0;

					}
				}
				if (!player.invincible) {
					enemies[i].playerCollision(player);
					player.projectileCollision(enemyProjectiles);
				}
			}
			testboi.avoidEnemies(enemies);

			for (int i = 0; i < std::size(enemyProjectiles); i++) {
				enemyProjectiles[i].move();
				enemyProjectiles[i].draw(renderer, camera);
			}
			
			camera.move(player.node.x, player.node.y);
			player.draw(renderer, camera);

			gui.drawHealth(renderer, player.health);
			gui.drawProgression(renderer, 3, 60);

			SDL_RenderPresent(renderer);
		}
	}
}