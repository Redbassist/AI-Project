#include "BulletManager.h"

static bool instanceFlag = false;
static BulletManager* instance = NULL;

BulletManager::BulletManager()
{
}

BulletManager * BulletManager::GetInstance()
{
	if (!instanceFlag) {
		instance = new BulletManager();
		instanceFlag = true;
	}
	return instance;
}

void BulletManager::AddBullet(Bullet * bullet)
{
	bullets.push_back(bullet);
}

void BulletManager::Update()
{
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->Update();
		//checking if the bullet is to be removed
		if (bullets[i]->LifeCheck()) {
			bullets.erase(bullets.begin() + i);
			i--;
		}
	} 
}

void BulletManager::Draw()
{
	int size = bullets.size();

	for (int i = 0; i < size; i++) {
		bullets[i]->Draw();
	}
}
