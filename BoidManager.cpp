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
	flocks[boid->currentFlock].push_back(boid);
	boids.push_back(boid);
}

void BoidManager::Update()
{
	for (int i = 0; i < boids.size(); i++) {
		boids[i]->update(boids);

		if (boids[i]->LifeCheck()) {
			int currentFlock = boids[i]->currentFlock;
			int size = flocks[currentFlock].size();
			for (int j = 0; j < size; j++) {
				if (flocks[currentFlock][j] == boids[i]) {
					flocks[currentFlock].erase(flocks[currentFlock].begin() + j);
					break;
				}
			}
			boids.erase(boids.begin() + i);
			i--;
		}
	}
}

void BoidManager::Draw()
{
	for (int i = 0; i < boids.size(); i++) {
		boids[i]->draw(); 
	}
}
