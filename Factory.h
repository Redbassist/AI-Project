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
	float prevRotation;
	float rotation;
	float targetRotation;
	Player* player;
	int width, height;
	Pvector position;
	Pvector direction;
	float speed;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Texture texture2;
	sf::Sprite sprite2;
	float radius;
	float health;
	bool travelling;
	Pvector destination;
	bool scared;
	int spawnTimer;
	int shootTimer;
	int maxMissiles;
	int missileTimer;
	Pvector location;
	Pvector velocity;
	Pvector acceleration;
	float maxSpeed;
	float maxForce;
	int currentFlock;
public:

	State currentState;

	Factory(Player& p, int);

	void loadResources();

	void Draw();

	void Update(vector<Factory*> Factories);

	void Movement();

	void WrapAround();

	void Shoot();

	//these are the code from the gods that fix the rotation loop around 2 pi
	float CurveAngle(float from, float to, float step);

	Pvector Slerp(Pvector from, Pvector to, float step);

	Pvector normalize(Pvector source);

	float degreeToRadian(float angle);

	float radiansToDegrees(float angle);

	float dotProduct(Pvector v1, Pvector v2);

	Pvector getPosition();

	float getRadius();

	sf::Sprite getSprite();

	Pvector getDirection();

	float getHealth();

	void setHealth(float h);

	float Distance(Pvector, Pvector);

	void State();

	void Spawn();

	bool LifeCheck();

	void applyForce(Pvector force);
	
	Pvector Separation(vector<Factory*> Factories);

	Pvector AvoidAsteroids();

	Pvector Alignment(vector<Factory*> Factories);

	Pvector Cohesion(vector<Factory*> Factories);

	Pvector RunFromPlayer();
	
	void LimitAcceleration();
	Pvector seek(Pvector v);
	void run(vector<Factory*> Factories);
	void flock(vector<Factory*> Factories);
	void chase(vector<Factory*> Factories);
	void collect(vector<Factory*> Factories);
	void avoid(vector<Factory*> Factories);
	float angle(Pvector v);
	bool checkAsteroids();
	bool checkPowerUps();
	bool search();
	bool lost();

};
