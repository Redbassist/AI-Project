#include "CollisionManager.h"

static bool instanceFlag = false;
static CollisionManager* instance = NULL;

CollisionManager* CollisionManager::GetInstance() {
	if (!instanceFlag) {
		instance = new CollisionManager();
		instanceFlag = true;
	}
	return instance;
}

CollisionManager::CollisionManager()
{
	asteroids = &AsteroidManager::GetInstance()->asteroids;
	bullets = &BulletManager::GetInstance()->bullets;
}

void CollisionManager::CheckCollisions()
{
	AsteroidCollisions();
	BulletCollisions();
}

void CollisionManager::AsteroidCollisions()
{
	for (int i = 0; i < asteroids->size(); i++)
	{
		float distance = Distance(player->getPosition(), asteroids->at(i)->getPos());
		float collisionDistance = player->getRadius() + asteroids->at(i)->getRadius();

		if (distance < collisionDistance)
		{
			Pvector response;
			response.x = asteroids->at(i)->getPos().x - player->getPosition().x;
			response.y = asteroids->at(i)->getPos().y - player->getPosition().y;
			response.normalize();
			response.mulScalar(asteroids->at(i)->getSpeed());
			//asteroids->at(i)->setDirection(Pvector(player->getDirection().x, player->getDirection().y));
			asteroids->at(i)->setDirection(response);
			//player->setHealth(player->getHealth() - 10);
		}
	}
}

void CollisionManager::BulletCollisions()
{

}

void CollisionManager::setPlayer(Player &p)
{
	player = &p;
}

float CollisionManager::Distance(Pvector pos1, Pvector pos2)
{
	return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}

