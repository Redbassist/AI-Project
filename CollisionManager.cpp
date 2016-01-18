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
}

void CollisionManager::CheckCollisions()
{
	for (int i = 0; i < asteroids->size(); i++)
	{
		float distance = sqrt(((player->getSprite().getPosition().x - asteroids->at(i)->getPos().x) * (player->getSprite().getPosition().x - asteroids->at(i)->getPos().x)) +
			((player->getSprite().getPosition().y - asteroids->at(i)->getPos().y) * (player->getSprite().getPosition().y - asteroids->at(i)->getPos().y)));
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
			player->setHealth(player->getHealth() - 10);
		}
	}
}

void CollisionManager::setPlayer(Player &p)
{
	player = &p;
}