#include "stdafx.h"
#pragma once

class Point {
public:
	int x, y;

	Point();
};

class Camera {
public:
	int x, y;

	void move(int x2, int y2);

	Camera();
};

class Background {
public:
	int x, y;

	int radius = 6553;//arbitrary

	int points = 2000;//how many stars there are
	Point stars[2000];

	void move(int x2, int y2);
	void draw(SDL_Renderer* renderer, Camera camera);

	SDL_Texture* star;

	Background(SDL_Renderer* renderer);
};

class Node {
public:
	float x, y, velX, velY, forceX, forceY;//positional things
	float mass;//how hefty it is

	Node();
};

class Line {
public:
	Node* node1;
	Node* node2;

	Line();
};

class Asteroid {
public:
	Node node;

	int points = 24;//how many points / lines there are

	Node nodes[24];
	Line lines[24];

	void move();
	void draw(SDL_Renderer* renderer, Camera camera);

	Asteroid(float x, float y);
};

class Projectile {
public:
	Node node;//for position, velocities, and forces
	int radius;

	float angle;
	float velocity;

	int OGTime;//when it was made
	int lifeTime;//how long it will stick around
	int timeAlive;//how long it has stuck around

	void move();
	void draw(SDL_Renderer* renderer, Camera camera, SDL_Texture* texture, int diameter);

	Projectile(SDL_Renderer* renderer);
};

class Particle {
public:
	Node node;

	int OGTime;//when it was made
	int lifeTime;//how long it will stick around
	int timeAlive;//how long it has stuck around

	void move();
	void draw(SDL_Renderer* renderer, Camera camera, SDL_Texture* texture);

	Particle();
};

class Player {
public:
	Node node;//for position, velocities, and forces, as well as mass
	int radius = 32;

	float angle;//angle of the 2nd part

	float health;
	bool hit;//if the player was just hit
	int invincibility;//how long (in seconds) the player will be invincible after being hit
	bool invincible;
	bool dying;//if the player is die
	bool died;//it heckin died man
	int frame;//what frame of the animation it is on
	int count;
	int frames;//how many frames there are
	int framesPerFrame;//how fast the animation plays

	int weapon;//what weapon is being used, one is blaster, 
	//two is shotgun, three is laser, four is missiles
	bool fire;//if the player is going to fire
	int fireRate1;//how often it can shoot for weapon 1
	int fireRate2;//how often it can shoot for weapon 2
	int fireRate3;//how often it can shoot for weapon 3
	int lastFire;

	SDL_Texture* texture;
	SDL_Texture* spriteSheet;

	void asteroidCollision(Asteroid &a);
	void projectileCollision(std::vector<Projectile> &projectiles);

	void shoot(std::vector<Projectile> &projectiles, SDL_Renderer* renderer);

	void move(SDL_Event &e, int mX, int mY);
	void draw(SDL_Renderer* renderer, Camera camera);

	Player(SDL_Renderer* renderer);
};

class Enemy {
public:
	Node node;//for position, velocities, and forces, as well as mass
	SDL_Rect collider;//for collision
	float acceleration;
	float maxSpeed;

	float optimalAngle;//where it should be pointing
	float angle;//which direction it's pointing
	float turnSpeed;//degrees/radians per second

	Point target;//where it will attack

	int health;
	int value;

	int lastShotTime;//the time boi last shot
	int fireRate;//how many seconds between each shot

	bool shooting;//if enemy2 is in the animation of shooting
	bool shot;//if the animation finished

	int framesPerFrame;//how fast the shooting animation runs
	int frame;//what frame its on
	int frames;

	int count;

	int enemyType;//1 is melee, 2 is ranged, 3 is shield
	
	SDL_Texture* texture;
	SDL_Texture* spriteSheet;

	void move(Player player);
	void playerCollision(Player &player);
	void avoidEnemies(std::vector<Enemy> &enemies);
	bool projectileCollision(Projectile &projectile);
	void shoot(std::vector<Projectile> &projectiles, SDL_Renderer* renderer);
	void draw(SDL_Renderer* renderer, Camera camera);

	Enemy(SDL_Renderer* renderer, int x, int y, int type);
};