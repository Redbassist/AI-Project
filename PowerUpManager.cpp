#include "PowerUpManager.h"

static bool instanceFlag = false;
static PowerUpManager* instance = NULL;

PowerUpManager* PowerUpManager::GetInstance() {
	if (!instanceFlag) {
		instance = new PowerUpManager();
		instanceFlag = true;
	}
	return instance;
}

PowerUpManager::PowerUpManager()
{

}

void PowerUpManager::Update()
{
	for (int i = 0; i < powerups.size(); i++) {
		powerups[i]->Update();
		//checking if the bullet is to be removed
		if (powerups[i]->LifeCheck()) {
			powerups.erase(powerups.begin() + i);
			i--;
		}
	}
}


void PowerUpManager::Draw()
{
	int size = powerups.size();

	for (int i = 0; i < size; i++) {
		powerups[i]->Draw();
	}
}

void PowerUpManager::SpawnPowerUp(PowerUp * powerup)
{
	powerups.push_back(powerup);
}
