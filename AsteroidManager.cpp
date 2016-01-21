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
	for (int i = 0; i < asteroids.size(); i++) {
		asteroids[i]->Update();
		//checking if the bullet is to be removed
		if (asteroids[i]->LifeCheck()) {
			float tempX = asteroids.at(i)->getPos().x;
			float tempY = asteroids.at(i)->getPos().y;
			asteroids.erase(asteroids.begin() + i);
			i--;

			int temp = (rand() % 10) + 1;

			if (temp == 10)
			{
				for (int i = 0; i < 10; i++)
				{
					BoidManager::GetInstance()->AddBoid(new Swarmer(tempX, tempY, player));
				}
			}
		}
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
	if (addTimer > 1) {
		addTimer = 0;

		Pvector spawnPos;
		bool spawnedAsteroid = false;
		if (asteroids.size() < 30) {//number of asteroids in the world
			while (!spawnedAsteroid) {
				spawnPos.x = (rand() % 3840 + 50);
				spawnPos.y = (rand() % 2160 - 50);

				if (CheckSpawnLocation(spawnPos)) {
					asteroids.push_back(new Asteroid(spawnPos));
 					spawnedAsteroid = true;
				}
			}
		}
	}
}

//returns true if the spawn is ok
bool AsteroidManager::CheckSpawnLocation(Pvector pos)
{
	Vector2f viewPos = Vector2f(pos.x, pos.y);
	Vector2f center = window->getView().getCenter();
	Vector2f size = window->getView().getSize();
	float width = size.x;
	float height = size.y;
	Vector2f topLeft = center - sf::Vector2f(size.x / 2, size.y / 2);

	if (viewPos.x > topLeft.x && viewPos.x < topLeft.x + width &&
		viewPos.y > topLeft.y && viewPos.y < topLeft.y + height)
		return false;

	return true;
}
