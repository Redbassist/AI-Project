#include "BoidManager.h"

static bool instanceFlag = false;
static BoidManager* instance = NULL;

BoidManager::BoidManager()
{
}

BoidManager * BoidManager::GetInstance()
{ 
	if (!instanceFlag) {
		instance = new BoidManager();
		instanceFlag = true;
	}
	return instance;
}

void BoidManager::AddBoid(Boid * boid)
{
	boids.push_back(boid);
}

void BoidManager::Update()
{
	for (int i = 0; i < boids.size(); i++) {
		boids[i]->update(boids);
		//checking if the boid is to be removed
	}
}

void BoidManager::Draw()
{
	for (int i = 0; i < boids.size(); i++) {
		boids[i]->draw(); 
	}
}