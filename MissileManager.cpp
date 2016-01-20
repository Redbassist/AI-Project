#include "MissileManager.h"

static bool instanceFlag = false;
static MissileManager* instance = NULL;

MissileManager::MissileManager()
{
}

MissileManager * MissileManager::GetInstance()
{
	if (!instanceFlag) {
		instance = new MissileManager();
		instanceFlag = true;
	}
	return instance;
}

void MissileManager::AddMissile(Missile * missile)
{
	missiles.push_back(missile);
}

void MissileManager::Update()
{
	for (int i = 0; i < missiles.size(); i++) {
		missiles[i]->Update();
		//checking if the bullet is to be removed
		if (missiles[i]->LifeCheck()) {
			missiles.erase(missiles.begin() + i);
			i--;
		}
	}
}

void MissileManager::Draw()
{
	int size = missiles.size();

	for (int i = 0; i < size; i++) {
		missiles[i]->Draw();
	}
}
