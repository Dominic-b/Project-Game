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
	std::vector<Particle> particles;

	bool spawned = false;

	//Asteroid test(player.node.x, player.node.y);
	//asteroids.push_back(test);
	Enemy testboi(renderer, 0, 0, 1);

	SDL_Texture* playerProjectile = render.loadTexture("Assets/Images/playerProjectile.png", renderer);
	SDL_Texture* enemyProjectile = render.loadTexture("Assets/Images/enemyProjectile.png", renderer);
	SDL_Texture* particle = render.loadTexture("Assets/Images/particle.png", renderer);

	Background background(renderer);
	Gui gui(renderer);
	Camera camera;

	events.quit = false;
	events.inGame = false;

	int enemyType = 1;

	int lastTime = 0;
	int lastTime2 = 0;

	int progression = 0;
	int level = 0;

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

			if (progression > 100) {
				level++;
				progression -= 100;
			}

			if (player.hit) {
				lastTime2 = (int)time(nullptr);
				player.invincible = true;
			} else if (lastTime2 == (int)time(nullptr) - player.invincibility) {
				player.invincible = false;
			}


			if (!spawned && time(nullptr) % 5 == 0) {
				if (level > 1) {
					enemyType = rand() % 3 + 1;
				}
				int enemyCount = rand() % 3 + 3;//between 3 and 5
				for (int i = 0; i < enemyCount; i++) {
					Enemy newEnemy(renderer, 0 + i, 0 + i, enemyType);
					newEnemy.shooting = true;
					enemies.push_back(newEnemy);
				}
				lastTime = (int)time(nullptr);
				spawned = true;
			} else if (spawned && time(nullptr) == lastTime + 1) {
				spawned = false;
			}

			player.move(e, events.mouseX + camera.x, events.mouseY + camera.y);
			if (player.fire) {
				player.shoot(playerProjectiles, renderer);
			}
			background.draw(renderer, camera);

			for (unsigned int i = 0; i < std::size(asteroids); i++) {
				asteroids[i].move();
				if (!player.invincible) player.asteroidCollision(asteroids[i]);
				asteroids[i].draw(renderer, camera);
			}

			for (unsigned int i = 0; i < std::size(enemies); i++) {
				enemies[i].move(player);
				enemies[i].draw(renderer, camera);
				for (unsigned int j = 0; j < std::size(playerProjectiles); j++) {
					if (enemies[i].projectileCollision(playerProjectiles[j])) {
						playerProjectiles.erase(playerProjectiles.begin() + j);
						j--;
						for (int e = 0; e < 4; e++) {
							Particle newParticle;
							newParticle.node.x = enemies[i].node.x;
							newParticle.node.y = enemies[i].node.y;
							newParticle.node.velX = (enemies[i].node.velX / 3) + rand() % 2 - 1;
							newParticle.node.velY = (enemies[i].node.velY / 3) + rand() % 2 - 1;
							particles.push_back(newParticle);
						}
					}
				}
				if (enemies[i].enemyType == 2) {
					if (enemies[i].lastShotTime + enemies[i].fireRate == (int)time(nullptr)) {
						enemies[i].shooting = true;
						enemies[i].lastShotTime = (int)time(nullptr);
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
				if (enemies[i].health <= 0) {
					progression += enemies[i].value;
					enemies.erase(enemies.begin() + i);
					i--;
				}
			}
			testboi.avoidEnemies(enemies);

			for (unsigned int i = 0; i < std::size(enemyProjectiles); i++) {
				enemyProjectiles[i].move();
				enemyProjectiles[i].draw(renderer, camera, enemyProjectile, 16);
				if (enemyProjectiles[i].timeAlive >= enemyProjectiles[i].lifeTime) {
					enemyProjectiles.erase(enemyProjectiles.begin() + i);
					i--;
				}
			}

			for (unsigned int i = 0; i < std::size(playerProjectiles); i++) {
				playerProjectiles[i].move();
				playerProjectiles[i].draw(renderer, camera, playerProjectile, 32);
				if (playerProjectiles[i].timeAlive >= playerProjectiles[i].lifeTime) {
					playerProjectiles.erase(playerProjectiles.begin() + i);
					i--;
				}
			}

			for (unsigned int i = 0; i < std::size(particles); i++) {
				particles[i].move();
				particles[i].draw(renderer, camera, particle);
				particles[i].timeAlive = (int)time(nullptr) - particles[i].OGTime;
				if (particles[i].timeAlive >= particles[i].lifeTime) {
					particles.erase(particles.begin() + i);
					i--;
				}
			}

			if (player.died) {
				events.inGame = false;
			}
			
			camera.move((int)player.node.x, (int)player.node.y);
			player.draw(renderer, camera);

			gui.drawHealth(renderer, (int)player.health);
			gui.drawProgression(renderer, level, progression);

			SDL_RenderPresent(renderer);
		}
	}
}