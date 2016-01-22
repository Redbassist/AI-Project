#pragma once  
#include "stdafx.h"
#include "Globals.h"
#include <math.h>
#include "Player.h"
#include "BoidManager.h"
#include "Predator.h"
#include "MissileManager.h"
#include "FactoryManager.h"

class FactoryManger;

class Factory{
private:
	int id;   
	Player* player;
	int width, height;
	Pvector position;
	Pvector direction;
	float speed;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Texture texture2;
	sf::Sprite sprite2;
	Texture uitexture;
	Sprite uisprite;
	float radius;
	float health;  
	int spawnTimer;
	int shootTimer;
	int maxMissiles;
	int missileTimer; 
	Pvector velocity;
	Pvector acceleration;
	float maxSpeed;
	float maxForce; 
public:

	State currentState;

	Factory(Player& p, int);

	void loadResources();

	void Draw();

	void drawui();

	void Update(vector<Factory*> Factories);

	void WrapAround();

	void Shoot(); 

	Pvector normalize(Pvector source); 

	Pvector getPosition();

	float getRadius();

	sf::Sprite getSprite();

	Pvector getDirection();

	float getHealth();

	void setHealth(float h);

	float Distance(Pvector, Pvector); 

	void Spawn();

	bool LifeCheck();

	void applyForce(Pvector force);
	
	Pvector Separation(vector<Factory*> Factories);

	Pvector AvoidAsteroids();

	Pvector Alignment(vector<Factory*> Factories);

	Pvector Cohesion(vector<Factory*> Factories);

	Pvector RunFromPlayer(); 

	Pvector seek(Pvector v);
	void run(vector<Factory*> Factories);
	void flock(vector<Factory*> Factories);
	void flee(vector<Factory*> Factories); 
	void avoid(vector<Factory*> Factories);
	float angle(Pvector v);
	bool checkAsteroids();
	bool checkPowerUps();
	bool search();
	bool lost();

};
