#include "stdafx.h"
#include "Factory.h"

#define PI 3.141592635

Factory::Factory(Player& p, int i) {
	id = i;
	speed = 1;
	float randX = rand() % (int)(globalBounds.x - 50) + 50;
	float randY = rand() % (int)(globalBounds.y - 50) + 50;
	//float randX = 200;
	//float randY = 200;
	position = Pvector(randX, randY);
	maxSpeed = 1.5;
	maxForce = 0.5;
	velocity = Pvector(rand() % 3 - 1, rand() % 3 - 1);
	loadResources(); 
	player = &p;
	spawnTimer = 60;
	maxMissiles = 0;
	missileTimer = 600;
	currentState = State::SEARCH;
}

void Factory::loadResources() {
	if (!uitexture.loadFromFile("Sprites/blip.png"))
	{
		cout << "cant find image";
	}

	uitexture.setSmooth(true);
	uisprite.setTexture(uitexture);
	uisprite.setOrigin(uitexture.getSize().x / 2, uitexture.getSize().y / 2);
	uisprite.setScale(0.14, 0.14);
	uisprite.setColor(Color::Green);

	if (!texture.loadFromFile("Sprites/FactoryPart2.png"))
	{
		std::cout << "cant find image";
	}

	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(400, 400);
	sprite.setScale(0.35f, 0.35f);
	sprite.setPosition(position.x, position.y);

	if (!texture2.loadFromFile("Sprites/FactoryPart2.png"))
	{
		std::cout << "cant find image";
	}

	texture2.setSmooth(true);
	sprite2.setTexture(texture);
	sprite2.setOrigin(400, 400);
	sprite2.setScale(0.35f, 0.35f);
	sprite2.setPosition(position.x, position.y);

	radius = 400 * sprite.getScale().x;

	health = 1000;
}

void Factory::Draw() {
	window->draw(sprite);
	window->draw(sprite2);
}

void Factory::drawui()
{
	View tempView = window->getView();
	Vector2f viewPos = tempView.getCenter();

	float d = position.distance(player->getPosition());
	if (d < 1300) {
		Pvector uiPos = position - player->getPosition();

		uiPos.divScalar(20);

		uisprite.setPosition(Vector2f(viewPos.x + 520 + uiPos.x, viewPos.y - 270 + uiPos.y));

		window->draw(uisprite);
	}
}

void Factory::Update(vector<Factory*> v) {

	WrapAround();
	Spawn();

	shootTimer--;
	if (maxMissiles > 0)
	{
		missileTimer--;
	}

	//"FINITE STATE MACHINE"
	switch (currentState) {

	case(State::SEARCH) :
		run(v);
		if (search()) {
			currentState = State::RETREAT;
		}
		else if (checkAsteroids()) {
			currentState = State::AVOIDASTEROID;
		}
		break;

	case(State::AVOIDASTEROID) :
		avoid(v);
		if (!checkAsteroids()) {
			currentState = State::SEARCH;
		}
		break;

	case(State::RETREAT) :
		flee(v);
		if (lost()) {
			currentState = State::SEARCH;
		}
		else if (checkAsteroids()) {
			currentState = State::AVOIDASTEROID;
		}
		break;
	}

	//To make the slow down not as abrupt
	acceleration.mulScalar(.4);
	// Update velocity
	velocity.addVector(acceleration);
	// Limit speed
	velocity.limit(maxSpeed);
	position.addVector(velocity);
	// Reset accelertion to 0 each cycle
	acceleration.mulScalar(0);

	sprite.setRotation(sprite.getRotation() + 0.1);
	sprite2.setRotation(sprite2.getRotation() - 0.1);

	sprite.setPosition(Vector2f(position.x, position.y));
}

Pvector Factory::Separation(vector<Factory*> v)
{
	// Distance of field of vision for separation between boids
	float desiredseparation = 100;

	Pvector steer(0, 0);
	int count = 0;
	// For every boid in the system, check if it's too close
	for (int i = 0; i < v.size(); i++)
	{
		// Calculate distance from current boid to boid we're looking at
		float d = position.distance(v[i]->position);
		// If this is a fellow boid and it's too close, move away from it
		if ((d > 0) && (d < desiredseparation))
		{
			Pvector diff(0, 0);
			diff = diff.subTwoVector(position, v[i]->position);
			diff.normalize();
			diff.divScalar(d);      // Weight by distance
			steer.addVector(diff);
			count++;
		}
	}
	// Adds average difference of position to acceleration
	if (count > 0)
		steer.divScalar((float)count);
	if (steer.magnitude() > 0)
	{
		// Steering = Desired - Velocity
		steer.normalize();
		steer.mulScalar(maxSpeed);
		steer.subVector(velocity);
		steer.limit(maxForce);
	}
	return steer;
}

Pvector Factory::AvoidAsteroids()
{
	// Distance of field of vision for separation between boid and asteroids
	float desiredseparation;

	Pvector steer(0, 0);
	int count = 0;
	vector<Asteroid*> asts = AsteroidManager::GetInstance()->asteroids;
	int size = asts.size();
	// For every boid in the system, check if it's too close
	for (int i = 0; i < size; i++)
	{
		desiredseparation = asts[i]->getRadius() + 100;
		// Calculate distance from current boid to boid we're looking at
		float d = position.distance(asts[i]->getPos());
		// If this is a fellow boid and it's too close, move away from it
		if ((d > 0) && (d < desiredseparation))
		{
			Pvector diff(0, 0);
			diff = diff.subTwoVector(position, asts[i]->getPos());
			diff.normalize();
			diff.divScalar(d);      // Weight by distance
			steer.addVector(diff);
			count++;
		}
	}
	// Adds average difference of position to acceleration
	if (count > 0)
		steer.divScalar((float)count);
	if (steer.magnitude() > 0)
	{
		// Steering = Desired - Velocity
		steer.normalize();
		steer.mulScalar(maxSpeed);
		steer.subVector(velocity);
		steer.limit(maxForce);
	}
	return steer;
}

// Alignment calculates the average velocity in the field of view and 
// manipulates the velocity of the Boid passed as parameter to adjust to that
// of nearby boids.
Pvector Factory::Alignment(vector<Factory*> v)
{
	float neighbordist = 500;

	Pvector sum(0, 0);
	int count = 0;
	for (int i = 0; i < v.size(); i++)
	{
		float d = position.distance(v[i]->position);
		if ((d > 0) && (d < neighbordist)) // 0 < d < 50
		{
			sum.addVector(v[i]->velocity);
			count++;
		}
	}
	// If there are boids close enough for alignment...
	if (count > 0)
	{
		sum.divScalar((float)count);// Divide sum by the number of close boids (average of velocity)
		sum.normalize();	   		// Turn sum into a unit vector, and
		sum.mulScalar(maxSpeed);    // Multiply by maxSpeed
									// Steer = Desired - Velocity
		Pvector steer;
		steer = steer.subTwoVector(sum, velocity); //sum = desired(average)  
		steer.limit(maxForce);
		return steer;
	}
	else {
		Pvector temp(0, 0);
		return temp;
	}
}

// Cohesion finds the average position of nearby boids and manipulates the 
// steering force to move in that direction.
Pvector Factory::Cohesion(vector<Factory*> v)
{
	float neighbordist = 160;

	Pvector sum(0, 0);
	int count = 0;
	for (int i = 0; i < v.size(); i++)
	{
		float d = position.distance(v[i]->position);
		if ((d > 0) && (d < neighbordist))
		{
			sum.addVector(v[i]->position);
			count++;
		}
	}
	if (count > 0)
	{
		sum.divScalar(count);
		return seek(sum);
	}
	else {
		Pvector temp(0, 0);
		return temp;
	}
}

Pvector Factory::seek(Pvector v)
{
	Pvector desired;
	desired = position;
	desired.subVector(v);  // A vector pointing from the position to the target
						   // Normalize desired and scale to maximum speed
	desired.normalize();
	desired.mulScalar(maxSpeed);
	// Steering = Desired minus Velocity
	//	acceleration = acceleration.subTwoVector(desired, velocity);
	acceleration = desired;
	acceleration.limit(maxForce);  // Limit to maximum steering force
	return acceleration;
}

void Factory::applyForce(Pvector force)
{
	acceleration.addVector(force);
}

Pvector Factory::RunFromPlayer()
{
	Pvector chaseVect;
	chaseVect = position - player->getPosition();

	chaseVect.normalize();	   		// Turn sum into a unit vector, and
	chaseVect.mulScalar(maxSpeed);    // Multiply by maxSpeed
									  // Steer = Desired - Velocity
	Pvector steer;
	steer = steer.subTwoVector(chaseVect, velocity); //sum = desired(average)  
	steer.limit(maxForce);
	return steer;
}

//Run runs flock on the flock of boids for each boid.
//Which applies the three rules, modifies accordingly, updates data, checks is data is
//out of range, fixes that for SFML, and renders it on the window.
void Factory::run(vector<Factory*> v)
{
	flock(v);
}

//Applies all three laws for the flock of boids and modifies to keep them from
//breaking the laws.
void Factory::flock(vector<Factory*> v)
{
	Pvector sep = Separation(v);
	Pvector ali = Alignment(FactoryManager::GetInstance()->factories);
	Pvector coh = Cohesion(FactoryManager::GetInstance()->factories);
	// Arbitrarily weight these forces
	sep.mulScalar(1.5);
	ali.mulScalar(1.0);
	coh.mulScalar(1.0);
	// Add the force vectors to acceleration
	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
}

void Factory::flee(vector<Factory*> v)
{
	Pvector sep = Separation(v);
	Pvector ali = Alignment(FactoryManager::GetInstance()->factories);
	Pvector coh = Cohesion(FactoryManager::GetInstance()->factories);
	Pvector run = RunFromPlayer();
	sep.mulScalar(1.0);
	ali.mulScalar(1.5);
	coh.mulScalar(0.2);
	run.mulScalar(1.5);
	applyForce(run);
	applyForce(sep);
	applyForce(ali);
	applyForce(coh);

	float distancePlayer = position.distance(player->getPosition());
	if (distancePlayer < 450) {
		Shoot();
	}
}

void Factory::avoid(vector<Factory*> v)
{
	Pvector sep = Separation(v);
	Pvector astAvoid = AvoidAsteroids();
	sep.mulScalar(1.0);
	astAvoid.mulScalar(1.7);
	applyForce(sep);
	applyForce(astAvoid);
}

// Calculates the angle for the velocity of a boid which allows the visual 
// image to rotate in the direction that it is going in.
float Factory::angle(Pvector v)
{
	// From the definition of the dot product
	float angle = (float)(atan2(v.x, -v.y) * 180 / PI);
	return angle;
}

bool Factory::checkAsteroids()
{
	// Distance of field of vision for separation between boid and asteroids
	float desiredseparation;

	vector<Asteroid*> asts = AsteroidManager::GetInstance()->asteroids;
	int size = asts.size();
	// For every boid in the system, check if it's too close
	for (int i = 0; i < size; i++)
	{
		desiredseparation = asts[i]->getRadius() + 100;
		// Calculate distance from current boid to asteroid we're looking at
		float d = position.distance(asts[i]->getPos());
		// If this is a fellow boid and it's too close, move away from it
		if ((d > 0) && (d < desiredseparation))
		{
			return true;
		}
	}
	return false;
}

bool Factory::checkPowerUps()
{
	float reactionDistance;

	vector<PowerUp*> powUps = PowerUpManager::GetInstance()->powerups;
	int size = powUps.size();
	// For every boid in the system, check if it's too close
	for (int i = 0; i < size; i++)
	{
		reactionDistance = 400;
		// Calculate distance from current boid to asteroid we're looking at
		float d = position.distance(powUps[i]->getPos());
		// If this is a fellow boid and it's too close, move away from it
		if ((d > 0) && (d < reactionDistance))
		{
			return true;
		}
	}
	return false;
}

bool Factory::search()
{
	if (player->getPosition().distance(position) < 400) {
		return true;
	}
	return false;
}

bool Factory::lost()
{
	if (player->getPosition().distance(position) > 450) {
		return true;
	}
	return false;
}

float Factory::getRadius()
{
	return radius;
}

sf::Sprite Factory::getSprite()
{
	return sprite;
}

Pvector Factory::getDirection()
{
	return direction;
}

float Factory::getHealth()
{
	return health;
}

void Factory::setHealth(float h)
{
	health = h;
}

bool Factory::LifeCheck()
{
	bool destroy = false;

	if (health <= 0)
	{
		destroy = true;
	}

	return destroy;
}

void Factory::WrapAround()
{
	//checking if the player has moved off the side of the screen and moving it
	if (position.x > globalBounds.x + 150)
		position.x = -120;
	else if (position.x < -150)
		position.x = globalBounds.x + 120;
	if (position.y > globalBounds.y + 150)
		position.y = -100;
	else if (position.y < -150)
		position.y = globalBounds.y + 120;

	sprite.setPosition(sf::Vector2f(position.x, position.y));
	sprite2.setPosition(sprite.getPosition());
}

void Factory::Shoot()
{
	if (shootTimer < 0 && maxMissiles < 5)
	{
		MissileManager::GetInstance()->AddMissile(new Missile(position, *player));
		shootTimer = 120;
		maxMissiles++;
	}

	if (missileTimer <= 0 && maxMissiles > 0)
	{
		missileTimer = 600;
		maxMissiles--;
	}
}

void Factory::Spawn()
{
	int numberOfBoids = 10;
	int boidCount = BoidManager::GetInstance()->flocks[id].size();
	if (spawnTimer < 0 && boidCount < numberOfBoids && boidCount <= 5)
	{
		BoidManager::GetInstance()->AddBoid(new Predator(position.x, position.y, player, id));
		BoidManager::GetInstance()->AddBoid(new Predator(position.x, position.y, player, id));
		BoidManager::GetInstance()->AddBoid(new Predator(position.x, position.y, player, id));
		BoidManager::GetInstance()->AddBoid(new Predator(position.x, position.y, player, id));
		BoidManager::GetInstance()->AddBoid(new Predator(position.x, position.y, player, id));
		spawnTimer = 120;
	}
	spawnTimer--;
}

Pvector Factory::getPosition() {
	return position;
}

float Factory::Distance(Pvector pos1, Pvector pos2)
{
	return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}

Pvector Factory::normalize(Pvector source)
{
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}