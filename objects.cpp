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
		stars[i].x = (int)(dist * cos(angle));
		stars[i].y = (int)(dist * sin(angle));
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
	srand((unsigned int)time(nullptr));
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
	node.velX = (float)(rand() % 100 - 50) / 10;
	srand((unsigned int)time(nullptr));
	node.velY = (float)(rand() % 100 - 50) / 10;
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
		SDL_RenderDrawLine(renderer, (int)(lines[i].node1->x - camera.x), (int)(lines[i].node1->y - camera.y), (int)(lines[i].node2->x - camera.x), (int)(lines[i].node2->y - camera.y));
	}
}

Projectile::Projectile(SDL_Renderer* renderer) {
	radius = 16;

	angle = 0;
	velocity = 12;

	OGTime = (int)time(nullptr);
	lifeTime = 10;
	timeAlive = 0;
}

void Projectile::move() {
	node.x += node.velX;
	node.y += node.velY;

	timeAlive = (int)time(nullptr) - OGTime;
}

void Projectile::draw(SDL_Renderer* renderer, Camera camera, SDL_Texture* texture, int diameter) {
	render.renderEx(renderer, texture, (int)(node.x - camera.x), (int)(node.y - camera.y), diameter, diameter, (int)(angle * 180 / PI));
}

Particle::Particle() {
	OGTime = (int)time(nullptr);
	lifeTime = 3;
	timeAlive = 0;
}

void Particle::move() {
	node.x += node.velX;
	node.y += node.velY;
}

void Particle::draw(SDL_Renderer* renderer, Camera camera, SDL_Texture* texture) {
	render.render(renderer, texture, (int)(node.x - camera.x), (int)(node.y - camera.y), 4, 4);
}

Player::Player(SDL_Renderer* renderer) {
	node.x = screenWidth / 2 - 250;
	node.y = screenHeight / 2;
	node.velX, node.velY, node.forceX, node.forceY = 0;

	health = 100;
	invincibility = 1;
	invincible = false;
	dying = false;
	died = false;
	frame = 0;
	count = 0;
	frames = 8;
	framesPerFrame = 120;

	node.mass = 1;

	radius = 16;

	angle = 0;

	weapon = 2;
	lastFire = 0;
	fireRate1 = 10;
	fireRate2 = 50;
	fireRate3 = 1;

	texture = render.loadTexture("Assets/Images/Player.png", renderer);
	spriteSheet = render.loadTexture("Assets/Images/PlayerDeath.png", renderer);
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
	for (unsigned int i = 0; i < std::size(projectiles); i++) {
		if (collision.circleVsCircle((int)node.x, (int)node.y, radius, (int)projectiles[i].node.x, (int)projectiles[i].node.y, 8)) {
			health -= 10;
			hit = true;
			projectiles[i].timeAlive = projectiles[i].timeAlive;
		}
	}
}

void Player::shoot(std::vector<Projectile> &projectiles, SDL_Renderer* renderer) {
	if (weapon == 1) {//blaster
		if (lastFire >= fireRate1) {
			Projectile newProjectile(renderer);
			newProjectile.node.x = node.x;
			newProjectile.node.y = node.y;
			newProjectile.node.velX = node.velX + newProjectile.velocity * cos(angle - (90 + rand() % 10 - 4) * PI / 180);
			newProjectile.node.velY = node.velY + newProjectile.velocity * sin(angle - (90 + rand() % 10 - 4) * PI / 180);
			newProjectile.angle = angle;
			projectiles.push_back(newProjectile);
			lastFire = 0;
		} else {
			lastFire++;
		}
	} else if (weapon == 2) {//shotgun
		if (lastFire >= fireRate2) {
			for (int i = 0; i < 6; i++) {
				Projectile newProjectile(renderer);
				newProjectile.node.x = node.x;
				newProjectile.node.y = node.y;
				newProjectile.node.velX = node.velX + newProjectile.velocity * cos(angle - (90 + 5 * (i - 3)) * PI / 180);
				newProjectile.node.velY = node.velY + newProjectile.velocity * sin(angle - (90 + 5 * (i - 3)) * PI / 180);
				newProjectile.angle = angle;
				projectiles.push_back(newProjectile);
				lastFire = 0;
			}
		}
		else {
			lastFire++;
		}
	} else if (weapon == 3) {//laser

	} else {//missiles

	}
}

void Player::move(SDL_Event &e, int mX, int mY) {
	if (health < 1) {
		dying = true;
	}

	if (dying) {
		if (count = framesPerFrame) {

		} else {
			count++;
		}

	}

	//key presses
	SDL_PollEvent(&e);
	events.keyboard_state_array = SDL_GetKeyboardState(NULL);

	if (events.keyboard_state_array[SDL_SCANCODE_W]) {
		node.forceX += (float)cos(angle - 90 * PI / 180);
		node.forceY += (float)sin(angle - 90 * PI / 180);
	}
	if (events.keyboard_state_array[SDL_SCANCODE_A]) angle -= (float).1;
	if (events.keyboard_state_array[SDL_SCANCODE_S]) {
		node.forceX -= (float)cos(angle - 90 * PI / 180) / 5;
		node.forceY -= (float)sin(angle - 90 * PI / 180) / 5;
	}
	if (events.keyboard_state_array[SDL_SCANCODE_D]) angle += (float).1;

	fire = false;
	if (events.keyboard_state_array[SDL_SCANCODE_SPACE]) {
		fire = true;
	} else if (health < 100) {
		health += (float).2;
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

	node.velX *= (float).99;
	node.velY *= (float).99;

	node.velX += (float)node.forceX / node.mass;
	node.velY += (float)node.forceY / node.mass;
	node.x += node.velX;
	node.y += node.velY;

	node.forceX = 0;
	node.forceY = 0;

	hit = false;
}

void Player::draw(SDL_Renderer* renderer, Camera camera) {
	if (!dying) {
		render.renderEx(renderer, texture, (int)(node.x - camera.x - radius), (int)(node.y - camera.y - radius), radius * 2, radius * 2, (int)(angle * 180 / PI));
	} else {
		if (count >= framesPerFrame) {
			count -= framesPerFrame;
			if (frame >= frames) {
				dying = false;
				died = true;
			}
			frame++;
		} else {
			count++;
		}
		render.renderSheetEx(renderer, spriteSheet, (int)(node.x - camera.x - radius), (int)(node.y - camera.y - radius), 32 * frame, 0, 32, 32, 32, 32, (int)(angle * (180 / PI)));
	}
}

Enemy::Enemy(SDL_Renderer* renderer, int x, int y, int type) {
	enemyType = type;
	if (enemyType == 1) {
		texture = render.loadTexture("Assets/Images/Enemy1.png", renderer);
		collider.w = 32;
		collider.h = 16;
		acceleration = (float).75;
		maxSpeed = 12;
		turnSpeed = (float)7.5;
		health = 100;
		value = 5;
	} else if (enemyType == 2) {
		texture = render.loadTexture("Assets/Images/Enemy2.png", renderer);
		spriteSheet = render.loadTexture("Assets/Animations/Enemy2/Enemy2.png", renderer);
		collider.w = 32;
		collider.h = 32;
		acceleration = (float).75;
		maxSpeed = 6;
		turnSpeed = (float)6.66;
		health = 50;
		framesPerFrame = 5;
		frames = 7;
		lastShotTime = (int)time(nullptr);
		fireRate = 3;
		value = 7;
	} else {
		texture = render.loadTexture("Assets/Images/Enemy3.png", renderer);
		collider.w = 32;
		collider.h = 32;
		acceleration = (float).5;
		maxSpeed = 6;
		turnSpeed = 5;
		health = 250;
		value = 7;
	}
	srand((unsigned int)time(nullptr));
	x += rand() % 100 - 50;
	y += rand() % 100 - 50;
	node.x = (float)x;
	node.y = (float)y;
	collider.x = (int)node.x;
	collider.y = (int)node.y;
}

void Enemy::move(Player player) {
	shot = false;
	target.x = (int)player.node.x;
	target.y = (int)player.node.y;
	optimalAngle = atan2(node.y - target.y, node.x - target.x) - 90 * PI / 180;

	float difference = angle - optimalAngle;

	if (difference > PI) {
		difference -= TAU;
	} else if (difference < -PI) {
		difference += TAU;
	}

	if (difference < 0) {
		angle += (float)turnSpeed * PI / 180;
	} else if (difference > 0) {
		angle -= (float)turnSpeed * PI / 180;
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
	collider.x = (int)node.x;
	collider.y = (int)node.y;
}

void Enemy::playerCollision(Player &player) {
	if (collision.circleVsRect((int)(player.node.x), (int)(player.node.y), (int)(player.radius), collider)) {
		player.health -= 25;
		player.hit = true;
	}
}

void Enemy::avoidEnemies(std::vector<Enemy> &enemies) {
	float diffX;
	float diffY;
	for (unsigned int i = 0; i < std::size(enemies); i++) {
		for (unsigned int j = i + 1; j < std::size(enemies); j++) {
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

bool Enemy::projectileCollision(Projectile &projectile) {
	if (collision.circleVsRect((int)(projectile.node.x), (int)(projectile.node.y), 32, collider)) {
		health -= 100;
		return true;
	}
	return false;
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
		render.renderEx(renderer, texture, (int)(node.x - camera.x), (int)(node.y - camera.y), 32, 16, (int)(angle * (180 / PI)));
	} else if (enemyType == 2) {
		if (!shooting) {
			render.renderEx(renderer, texture, (int)(node.x - camera.x), (int)(node.y - camera.y), 32, 32, (int)(angle * (180 / PI)));
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
			render.renderSheetEx(renderer, spriteSheet, (int)(node.x - camera.x), (int)(node.y - camera.y), (int)(32 * (frame % 3)), (int)(32 * floor(frame / 3)), 32, 32, 32, 32, (int)(angle * (180 / PI)));
		}
	} else {
		render.renderEx(renderer, texture, (int)(node.x - camera.x), (int)(node.y - camera.y), 32, 32, (int)(angle * (180 / PI)));
	}
}