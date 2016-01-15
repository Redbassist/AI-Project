#include "AsteroidManager.h"

static bool instanceFlag = false;
static AsteroidManager* instance = NULL;

AsteroidManager* AsteroidManager::GetInstance() {
	if (!instanceFlag) {
		instance = new AsteroidManager();
		instanceFlag = true;
	}
	return instance;
}

AsteroidManager::AsteroidManager()
{
	timerLength = 60;
} 

void AsteroidManager::Update()
{
	int size = asteroids.size();

	for (int i = 0; i < size; i++) {
		asteroids[i]->Update();
	}
	SpawnAsteroids();
}

void AsteroidManager::Draw()
{
	int size = asteroids.size();

	for (int i = 0; i < size; i++) {
		asteroids[i]->Draw();
	}
}

void AsteroidManager::SpawnAsteroids()
{
	addTimer++;
	if (addTimer > 60) {
		addTimer = 0;
		asteroids.push_back(new Asteroid(Pvector(0, 0)));
	}
}
