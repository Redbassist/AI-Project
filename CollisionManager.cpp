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
	boids = &BoidManager::GetInstance()->boids;
	powerups = &PowerUpManager::GetInstance()->powerups;
	missiles = &MissileManager::GetInstance()->missiles;
	factories = &FactoryManager::GetInstance()->factories;
}

void CollisionManager::CheckCollisions()
{
	AsteroidCollisions();
	BulletCollisions();
	BoidCollisions();
	AsteroidPredCollisions();
	PlayerPowerUpCollisions();
	PlayerMissileCollisions();
	BulletFactoryCollisions();
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
			player->dropHealth(4);
		}
	}
}

void CollisionManager::BulletCollisions()
{
	float distance;
	float collisionDistance;
	for (int j = 0; j < bullets->size(); j++)
	{
		for (int i = 0; i < asteroids->size(); i++)
		{
			distance = Distance(bullets->at(j)->getPos(), asteroids->at(i)->getPos());
			collisionDistance = bullets->at(j)->getRadius() + asteroids->at(i)->getRadius();

			if (distance < collisionDistance)
			{
				bullets->at(j)->setDestroyed(true);
				if (bullets->at(j)->superBullet)
				{
					asteroids->at(i)->setHealth(asteroids->at(i)->getHealth() - 50);
				}
				else
				{
					asteroids->at(i)->setHealth(asteroids->at(i)->getHealth() - 25);
				}
			}
		}
		if (!bullets->at(j)->playerBullet) {
			distance = Distance(bullets->at(j)->getPos(), player->getPosition());
			collisionDistance = bullets->at(j)->getRadius() + player->getRadius();

			if (distance < collisionDistance)
			{
				bullets->at(j)->setDestroyed(true);
				if (bullets->at(j)->superBullet) {
					player->dropHealth(100);
				}
				else {
					player->dropHealth(50);
				}
			}
		}
	}
}

void CollisionManager::BoidCollisions()
{
	float distance;
	float collisionDistance;

	for (int i = 0; i < boids->size(); i++)
	{
		for (int j = 0; j < bullets->size(); j++)
		{
			if (bullets->at(j)->playerBullet) {
				distance = Distance(bullets->at(j)->getPos(), boids->at(i)->getPos());
				collisionDistance = bullets->at(j)->getRadius() + boids->at(i)->getRadius();

				if (distance < collisionDistance)
				{
					if (bullets->at(j)->superBullet)
					{
						boids->at(i)->setHealth(boids->at(i)->getHealth() - 1000);
					}
					else
					{
						bullets->at(j)->setDestroyed(true);
						boids->at(i)->setHealth(boids->at(i)->getHealth() - 500);
					}
				}
			}
		}

		for (int j = 0; j < powerups->size(); j++)
		{
			distance = Distance(powerups->at(j)->getPos(), boids->at(i)->getPos());
			collisionDistance = powerups->at(j)->getRadius() + boids->at(i)->getRadius();

			if (distance < collisionDistance)
			{
				if (powerups->at(j)->type == 0)
				{
					powerups->at(j)->destroy = true;
					boids->at(i)->speedTimer = 300;
					break;
				}
				else
				{
 					powerups->at(j)->destroy = true;
					boids->at(i)->powerTimer = 300;
					break;
				}
			}
		}

		distance = Distance(boids->at(i)->getPos(), player->getPosition());
		collisionDistance = boids->at(i)->getRadius() + player->getRadius();

		if (distance < collisionDistance)
		{
			player->dropHealth(50);
			boids->at(i)->setHealth(boids->at(i)->getHealth() - 1000);
		}
	}
}

void CollisionManager::AsteroidPredCollisions()
{
	for (int i = 0; i < boids->size(); i++)
	{
		for (int j = 0; j < asteroids->size(); j++)
		{
			float distance = Distance(asteroids->at(j)->getPos(), boids->at(i)->getPos());
			float collisionDistance = asteroids->at(j)->getRadius() + boids->at(i)->getRadius();

			if (distance < collisionDistance)
			{
				boids->at(i)->setHealth(boids->at(i)->getHealth() - 1000);
			}
		}
	}
}

void CollisionManager::PlayerPowerUpCollisions()
{
	for (int i = 0; i < powerups->size(); i++)
	{
		float distance = Distance(player->getPosition(), powerups->at(i)->getPos());
		float collisionDistance = player->getRadius() + powerups->at(i)->getRadius();

		if (distance < collisionDistance)
		{
			if (powerups->at(i)->type == 1)
			{
				player->ActivateSpeed();
				powerups->at(i)->destroy = true;
			}
			else
			{
				player->ActivatePower();
				powerups->at(i)->destroy = true;
			}
		}
	}
}

void CollisionManager::PlayerMissileCollisions()
{
	for (int i = 0; i < missiles->size(); i++)
	{
		float distance = Distance(player->getPosition(), missiles->at(i)->getPos());
		float collisionDistance = player->getRadius() + missiles->at(i)->getRadius();

		if (distance < collisionDistance)
		{
			missiles->at(i)->setDestroyed(true);
			player->dropHealth(100);
		}
	}
}

void CollisionManager::BulletFactoryCollisions()
{
	for (int i = 0; i < factories->size(); i++)
	{
		for (int j = 0; j < bullets->size(); j++)
		{
			float distance = Distance(bullets->at(j)->getPos(), factories->at(i)->getPosition());
			float collisionDistance = bullets->at(j)->getRadius() + factories->at(i)->getRadius();

			if (distance < collisionDistance)
			{
				if (bullets->at(j)->superBullet)
					{
						factories->at(i)->setHealth(factories->at(i)->getHealth() - 40);
					}
					else
					{
						bullets->at(j)->setDestroyed(true);
						factories->at(i)->setHealth(factories->at(i)->getHealth() - 20);
					}
			}
		}
	}
}

void CollisionManager::setPlayer(Player &p)
{
	player = &p;
}

float CollisionManager::Distance(Pvector pos1, Pvector pos2)
{
	return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}

