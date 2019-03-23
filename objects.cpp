#include "stdafx.h"

Point::Point() {

}

Camera::Camera() {
	x = 0;
	y = 0;
}

void Camera::move(int x2, int y2) {
	x = x2 - screenWidth / 2;
	y = y2 - screenHeight / 2;
	screen.x = x;
	screen.y = y;
}

Background::Background(SDL_Renderer* renderer) {
	int dist;
	int angle;
	x = 0;
	y = 0;
	star = render.loadTexture("Assets/Images/Star.png", renderer);
	for (int i = 0; i < points; i++) {
		dist = rand() % (2 * radius) - radius;
		angle = rand() % 360;
		stars[i].x = dist * cos(angle);
		stars[i].y = dist * sin(angle);
		srand(dist - angle);
	}
}

void Background::move(int x2, int y2) {

}

void Background::draw(SDL_Renderer* renderer, Camera camera) {
	for (int i = 0; i < points; i++) {
		if (collision.pointVsRect(stars[i].x, stars[i].y, screen)) {
			render.render(renderer, star, stars[i].x - camera.x, stars[i].y - camera.y, 9, 13);
		}
	}
}

Node::Node() {
	x, y, velX, velY, forceX, forceY = 0;
	mass = 1;
}

Line::Line() {

}

Asteroid::Asteroid(float x, float y) {
	node.x = x;
	node.y = y;
	node.velX = 0;
	node.velY = 0;
	node.forceX = 0;
	node.forceY = 0;
	node.mass = 100;
	float dist;
	float radians;
	srand(time(nullptr));
	int random = rand() % 51 - 25;
	for (int i = 0; i < points; i++) {
		radians = i * (TAU / points);
		dist = (noise.noise(radians + random) / 5) + random;

		nodes[i].x = (dist * cos(radians)) + node.x;
		nodes[i].y = (dist * sin(radians)) + node.y;
		nodes[i].velX = 0;
		nodes[i].velY = 0;
		nodes[i].forceX = 0;
		nodes[i].forceY = 0;
		lines[i].node1 = &nodes[i];
		lines[i].node2 = &nodes[(i + 1) % points];
	}
	node.velX = (rand() % 100 - 50) / 10;
	srand(time(nullptr));
	node.velY = (rand() % 100 - 50) / 10;
}

void Asteroid::move() {
	for (int i = 0; i < points; i++) {
		lines[i].node1->velX += node.forceX / node.mass;
		lines[i].node1->velY += node.forceY / node.mass;
		lines[i].node1->x += node.velX;
		lines[i].node1->y += node.velY;
		node.forceX = 0;
		node.forceY = 0;
	}
}

void Asteroid::draw(SDL_Renderer* renderer, Camera camera) {
	for (int i = 0; i < points; i++) {
		SDL_RenderDrawLine(renderer, lines[i].node1->x - camera.x, lines[i].node1->y - camera.y, lines[i].node2->x - camera.x, lines[i].node2->y - camera.y);
	}
}

Projectile::Projectile(SDL_Renderer* renderer) {
	radius = 16;

	angle = 0;
	velocity = 12;

	lifeTime = 10;
	timeAlive = 0;

	playerProjectile;
	enemyProjectile = render.loadTexture("Assets/Images/enemyProjectile.png",renderer);
}

void Projectile::move() {
	node.x += node.velX;
	node.y += node.velY;
}

void Projectile::draw(SDL_Renderer* renderer, Camera camera) {
	render.render(renderer, enemyProjectile, node.x - camera.x, node.y - camera.y, 16, 16);
}

Player::Player(SDL_Renderer* renderer) {
	node.x = screenWidth / 2 - 250;
	node.y = screenHeight / 2;
	node.velX, node.velY, node.forceX, node.forceY = 0;

	health = 100;
	invincibility = 3;
	invincible = false;

	node.mass = 1;

	radius = 16;

	angle = 0;

	texture = render.loadTexture("Assets/Images/Player.png", renderer);
	texture2 = render.loadTexture("Assets/Images/PlayerPt2.png", renderer);

}

void Player::asteroidCollision(Asteroid &a) {
	float dist;
	for (int i = 0; i < a.points; i++) {
		dist = sqrt(pow(a.lines[i].node1->x - node.x, 2) + pow(a.lines[i].node1->y - node.y, 2));
		if (dist < radius) {
			health = 0;//die
		}
	}
}

void Player::projectileCollision(std::vector<Projectile> &projectiles) {
	for (int i = 0; i < std::size(projectiles); i++) {
		if (collision.circleVsCircle(node.x, node.y, radius, projectiles[i].node.x, projectiles[i].node.y, 8)) {
			health -= 10;
			hit = true;
			projectiles[i].timeAlive = projectiles[i].timeAlive;
		}
	}
}

void Player::move(SDL_Event &e, int mX, int mY) {
	if (health < 1) {
		events.inGame = false;
	}

	//key presses
	SDL_PollEvent(&e);
	events.keyboard_state_array = SDL_GetKeyboardState(NULL);

	if (events.keyboard_state_array[SDL_SCANCODE_W]) {
		node.forceX += cos(angle - 90 * PI / 180);
		node.forceY += sin(angle - 90 * PI / 180);
	}
	if (events.keyboard_state_array[SDL_SCANCODE_A]) angle -= .1;
	if (events.keyboard_state_array[SDL_SCANCODE_S]) {
		node.forceX -= cos(angle - 90 * PI / 180);
		node.forceY -= sin(angle - 90 * PI / 180);
	}
	if (events.keyboard_state_array[SDL_SCANCODE_D]) angle += .1;

	if (events.keyboard_state_array[SDL_SCANCODE_SPACE]) {
		node.velX *= .9;
		node.velY *= .9;
	}

	if (node.velX > 8) {
		node.velX = 8;
	} else if (node.velX < -8) {
		node.velX = -8;
	}
	if (node.velY > 8) {
		node.velY = 8;
	} else if (node.velY < -8) {
		node.velY = -8;
	}
	//angle = atan2(node.y - mY, node.x - mX) - 90 * PI / 180;

	node.velX += node.forceX / node.mass;
	node.velY += node.forceY / node.mass;
	node.x += node.velX;
	node.y += node.velY;

	node.forceX = 0;
	node.forceY = 0;

	hit = false;
}

void Player::draw(SDL_Renderer* renderer, Camera camera) {
	render.render(renderer, texture, node.x - camera.x - radius, node.y - camera.y - radius, radius * 2, radius * 2);
	render.renderEx(renderer, texture2, node.x - camera.x + 8 - radius, node.y - 16 - camera.y - radius, 16, 64, angle * 180 / PI);
}

Enemy::Enemy(SDL_Renderer* renderer, int x, int y, int type) {
	enemyType = type;
	if (enemyType == 1) {
		texture = render.loadTexture("Assets/Images/Enemy1.png", renderer);
		collider.w = 32;
		collider.h = 16;
		acceleration = .75;
		maxSpeed = 12;
		turnSpeed = 7.5;
		health = 100;
	} else if (enemyType == 2) {
		texture = render.loadTexture("Assets/Images/Enemy2.png", renderer);
		spriteSheet = render.loadTexture("Assets/Animations/Enemy2/Enemy2.png", renderer);
		collider.w = 32;
		collider.h = 32;
		acceleration = .75;
		maxSpeed = 6;
		turnSpeed = 6.66;
		health = 50;
		framesPerFrame = 5;
		frames = 7;
		lastShotTime = time(nullptr);
		fireRate = 3;
	} else {
		texture = render.loadTexture("Assets/Images/Enemy3.png", renderer);
		collider.w = 32;
		collider.h = 32;
		acceleration = .5;
		maxSpeed = 6;
		turnSpeed = 5;
		health = 250;
	}
	srand(time(nullptr));
	x += rand() % 100 - 50;
	y += rand() % 100 - 50;
	node.x = x;
	node.y = y;
	collider.x = node.x;
	collider.y = node.y;
}

void Enemy::move(Player player) {
	shot = false;
	target.x = player.node.x;
	target.y = player.node.y;
	optimalAngle = atan2(node.y - target.y, node.x - target.x) - 90 * PI / 180;

	float difference = angle - optimalAngle;

	if (difference > PI) {
		difference -= TAU;
	} else if (difference < -PI) {
		difference += TAU;
	}

	if (difference < 0) {
		angle += turnSpeed * PI / 180;
	} else if (difference > 0) {
		angle -= turnSpeed * PI / 180;
	}
	if (enemyType == 1 || enemyType == 3) {
		node.velX += acceleration * cos(angle - 90 * PI / 180);
		node.velY += acceleration * sin(angle - 90 * PI / 180);
	} else {
		if (pow(node.x - player.node.x, 2) + pow(node.y - player.node.y, 2) < 256 * 256) {
			node.velX += acceleration * cos(angle);
			node.velY += acceleration * sin(angle);
		}
		node.velX += acceleration * cos(angle - 90 * PI / 180);
		node.velY += acceleration * sin(angle - 90 * PI / 180);
	}
	if (node.velX > maxSpeed) {
		node.velX = maxSpeed;
	} else if (node.velX < -maxSpeed) {
		node.velX = -maxSpeed;
	}
	if (node.velY > maxSpeed) {
		node.velY = maxSpeed;
	} else if (node.velY < -maxSpeed) {
		node.velY = -maxSpeed;
	}
	node.x += node.velX;
	node.y += node.velY;
	collider.x = node.x;
	collider.y = node.y;
}

void Enemy::playerCollision(Player &player) {
	if (collision.circleVsRect(player.node.x, player.node.y, player.radius, collider)) {
		player.health -= 25;
		player.hit = true;
	}
}

void Enemy::avoidEnemies(std::vector<Enemy> &enemies) {
	float diffX;
	float diffY;
	for (int i = 0; i < std::size(enemies); i++) {
		for (int j = i + 1; j < std::size(enemies); j++) {
			diffX = enemies[i].node.x - enemies[j].node.x;
			diffY = enemies[i].node.y - enemies[j].node.y;
			if ((diffX * diffX) + (diffY * diffY) <= 9216) {//9216 is 96 squared
				enemies[i].node.velX += diffX / 75;
				enemies[i].node.velY += diffY / 75;

				enemies[j].node.velX -= diffX / 75;
				enemies[j].node.velY -= diffY / 75;
			}
		}
	}
}

void Enemy::shoot(std::vector<Projectile> &projectiles, SDL_Renderer* renderer) {
	Projectile newProjectile(renderer);
	newProjectile.node.x = node.x;
	newProjectile.node.y = node.y;
	newProjectile.node.velX = newProjectile.velocity * cos(angle - 90 * PI / 180);
	newProjectile.node.velY = newProjectile.velocity * sin(angle - 90 * PI / 180);
	projectiles.push_back(newProjectile);
}

void Enemy::draw(SDL_Renderer* renderer, Camera camera) {
	if (enemyType == 1) {
		render.renderEx(renderer, texture, node.x - camera.x, node.y - camera.y, 32, 16, angle * (180 / PI));
	} else if (enemyType == 2) {
		if (!shooting) {
			render.renderEx(renderer, texture, node.x - camera.x, node.y - camera.y, 32, 32, angle * (180 / PI));
		} else {
			if (count == framesPerFrame) {
				count = 0;
				if (frame >= frames) {
					shooting = false;
					shot = true;
				}
				frame++;
			} else {
				count++;
			}
			render.renderSheetEx(renderer, spriteSheet, node.x - camera.x, node.y - camera.y, 32 * (frame % 3), 32 * floor(frame / 3), 32, 32, 32, 32, angle * (180 / PI));
		}
	} else {
		render.renderEx(renderer, texture, node.x - camera.x, node.y - camera.y, 32, 32, angle * (180 / PI));
	}
}