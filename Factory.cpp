#include "stdafx.h"
#include "Factory.h"

#define PI 3.141592635

Factory::Factory(Player& p, int i){
	id = i;
	speed = 1;
	float randX = rand() % (int)(globalBounds.x - 50) + 50;
	float randY = rand() % (int)(globalBounds.y - 50) + 50;
	//float randX = 200;
	//float randY = 200;
	position = Pvector(randX, randY);
	loadResources();
	scared = false;
	travelling = false;
	player = &p;
	spawnTimer = 60;
	maxMissiles = 0;
	missileTimer = 600;
}

void Factory::loadResources() {
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

void Factory::Update(vector<Factory*> v) {
	//State();
	//Movement();
	WrapAround();
	//Shoot();
	Spawn();


	//"FINITE STATE MACHINE"
	switch (currentState) {

	case(State::SEARCH) :
		run(v);
		if (search()) {
			currentState = State::ATTACK;
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

	case(State::ATTACK) :
		chase(v);
		if (lost()) {
			currentState = State::ATTACK;
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
	location.addVector(velocity);
	// Reset accelertion to 0 each cycle
	acceleration.mulScalar(0);

	sprite.setRotation(angle(velocity));
	sprite.setPosition(Vector2f(location.x, location.y));
}

Pvector Factory::Separation(vector<Factory*> v)
{
	// Distance of field of vision for separation between boids
	float desiredseparation = 20;

	Pvector steer(0, 0);
	int count = 0;
	// For every boid in the system, check if it's too close
	for (int i = 0; i < v.size(); i++)
	{
		// Calculate distance from current boid to boid we're looking at
		float d = location.distance(v[i]->location);
		// If this is a fellow boid and it's too close, move away from it
		if ((d > 0) && (d < desiredseparation))
		{
			Pvector diff(0, 0);
			diff = diff.subTwoVector(location, v[i]->location);
			diff.normalize();
			diff.divScalar(d);      // Weight by distance
			steer.addVector(diff);
			count++;
		}
	}
	// Adds average difference of location to acceleration
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
		float d = location.distance(asts[i]->getPos());
		// If this is a fellow boid and it's too close, move away from it
		if ((d > 0) && (d < desiredseparation))
		{
			Pvector diff(0, 0);
			diff = diff.subTwoVector(location, asts[i]->getPos());
			diff.normalize();
			diff.divScalar(d);      // Weight by distance
			steer.addVector(diff);
			count++;
		}
	}
	// Adds average difference of location to acceleration
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
	float neighbordist = 150;

	Pvector sum(0, 0);
	int count = 0;
	for (int i = 0; i < v.size(); i++)
	{
		float d = location.distance(v[i]->location);
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

// Cohesion finds the average location of nearby boids and manipulates the 
// steering force to move in that direction.
Pvector Factory::Cohesion(vector<Factory*> v)
{
	float neighbordist = 40;

	Pvector sum(0, 0);
	int count = 0;
	for (int i = 0; i < v.size(); i++)
	{
		float d = location.distance(v[i]->location);
		if ((d > 0) && (d < neighbordist))
		{
			sum.addVector(v[i]->location);
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
	desired = location;
	desired.subVector(v);  // A vector pointing from the location to the target
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
	chaseVect = location - player->getPosition();

	chaseVect.normalize();	   		// Turn sum into a unit vector, and
	chaseVect.mulScalar(maxSpeed);    // Multiply by maxSpeed
									  // Steer = Desired - Velocity
	Pvector steer;
	steer = steer.subTwoVector(chaseVect, velocity); //sum = desired(average)  
	steer.limit(maxForce);
	return steer;
}

void Factory::LimitAcceleration()
{
	float stayDistance = 200;
	float slowDistance = 200;
	float distancePlayer = location.distance(player->getPosition());

	if (distancePlayer > stayDistance && distancePlayer < stayDistance + slowDistance) {
		acceleration.mulScalar((distancePlayer - stayDistance) / slowDistance);
		velocity.mulScalar((distancePlayer - stayDistance) / slowDistance);
	}
	else if (distancePlayer < stayDistance) {
		velocity.mulScalar(0);
	}

}
//Run runs flock on the flock of boids for each boid.
//Which applies the three rules, modifies accordingly, updates data, checks is data is
//out of range, fixes that for SFML, and renders it on the window.
void Factory::run(vector<Factory*> v)
{
	flock(v);
	borders();
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

void Factory::chase(vector<Factory*> v)
{
	Pvector sep = Separation(v);
	Pvector ali = Alignment(FactoryManager::GetInstance()->factories);
	Pvector coh = Cohesion(FactoryManager::GetInstance()->factories);
	Pvector chs = RunFromPlayer();
	sep.mulScalar(1.0);
	ali.mulScalar(1.5);
	coh.mulScalar(0.2);
	chs.mulScalar(1.5);
	applyForce(chs);
	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
	LimitAcceleration();
	borders();

	float distancePlayer = location.distance(player->getPosition());
	if (distancePlayer < 400) {
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
	borders();
}

//void Factory::Shoot()
//{
//	if (fireTimer > fireRate) {
//		fireTimer = 0;
//		Bullet* bullet = new Bullet((location), velocity, false);
//		BulletManager::GetInstance()->AddBullet(bullet);
//	}
//}

// Checks if boids go out of the window and if so, wraps them around to the other side.
//void Factory::borders()
//{
//	if (location.x < 0) location.x += window_width;
//	if (location.y < 0) location.y += window_height;
//	if (location.x > window_width) location.x -= window_width;
//	if (location.y > window_height) location.y -= window_height;
//}

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
		float d = location.distance(asts[i]->getPos());
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
		float d = location.distance(powUps[i]->getPos());
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
	if (player->getPosition().distance(location) < 400) {
		return true;
	}
	return false;
}

bool Factory::lost()
{
	if (player->getPosition().distance(location) > 450) {
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

void Factory::State()
{
	sprite.setRotation(sprite.getRotation() + 0.1);
	sprite2.setRotation(sprite2.getRotation() - 0.1);

	if (scared == false)
	{
		if (travelling == true)
		{
			Pvector vel;
			vel.x = position.x - destination.x;
			vel.y = position.y - destination.y;
			vel.normalize();
			vel.mulScalar(speed);
			position.x = position.x - vel.x;
			position.y = position.y - vel.y;
		}
		else
		{
			destination.x = rand() % (int)(globalBounds.x - 100) + 100;
			destination.y = rand() % (int)(globalBounds.y - 100) + 100;
			travelling = true;
		}

		if (Distance(position, destination) < 50)
		{
			travelling = false;
		}

		if (Distance(position, player->getPosition()) < 400)
		{
			scared = true;
			travelling = false;
		}
	}
	else
	{
		Pvector vel;
		vel.x = position.x - player->getPosition().x;
		vel.y = position.y - player->getPosition().y;
		vel.normalize();
		//vel.mulScalar(speed);
		destination.x = position.x + (vel.x * 400);
		destination.y = position.y + (vel.y * 400);
		//position.x = position.x + vel.x;
		//position.y = position.y + vel.y;

		if (Distance(position, player->getPosition()) > 399)
		{
			scared = false;
			travelling = false;
		}
	}
}

void Factory::Movement()
{
	prevRotation = rotation;
	position = Pvector(sprite.getPosition());
	//Vector2i mp = Mouse::getPosition(*window);
	//used to convert to view coordinates
	//sf::Vector2f worldMousePos = window->mapPixelToCoords(mp);
	//Pvector mousePos = Pvector(worldMousePos);
	Pvector mousePos = destination;
	Pvector wantedVector = mousePos.subTwoVector(mousePos, position);

	float angleBetweenTwo = atan2(mousePos.y - position.y, mousePos.x - position.x);

	rotation = CurveAngle(rotation, angleBetweenTwo, 0.01f);

	//error check, rotation was crashing every so often, this is a loose fix
	if (isnan(rotation))
		rotation = prevRotation;

	direction = Pvector(cos(rotation), sin(rotation));
	//normalize(direction);
	direction.mulScalar(speed);

	//if space is held down, will fly forwards
	position.addVector(direction);

	//sprite.setRotation(radiansToDegrees(rotation) + 90);
	sprite.setPosition(sf::Vector2f(position.x, position.y));
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
	if (scared == true)
	{
		if (shootTimer < 0 && maxMissiles < 5)
		{
			MissileManager::GetInstance()->AddMissile(new Missile(position, *player));
			shootTimer = 120;
			maxMissiles++;
		}
		shootTimer--;
	}
	if (maxMissiles > 0)
	{
		missileTimer--;
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

float Factory::CurveAngle(float from, float to, float step)
{
	if (step == 0)
		return from;
	if (from == to || step == 1)
		return to;

	Pvector fromVector = Pvector((float)cos(from), (float)sin(from));
	Pvector toVector = Pvector((float)cos(to), (float)sin(to));

	Pvector currentVector = Slerp(fromVector, toVector, step);

	return (float)atan2(currentVector.y, currentVector.x);
}

Pvector Factory::Slerp(Pvector from, Pvector to, float step)
{
	if (step == 0) return from;
	if (from == to || step == 1) return to;

	double theta = acos(dotProduct(from, to));
	if (theta == 0) return to;

	double sinTheta = sin(theta);
	from.mulScalar((float)(sin((1 - step) * theta) / sinTheta));
	to.mulScalar((float)(sin(step * theta) / sinTheta));
	from.addVector(to);
	return from;
}

Pvector Factory::normalize(Pvector source)
{
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}

float  Factory::degreeToRadian(float angle) {
	float pi = 3.14159265358979323846;
	return  pi * angle / 180.0;
}

float Factory::radiansToDegrees(float angle) {
	float pi = 3.14159265358979323846;
	return angle * (180.0 / pi);
}

float Factory::dotProduct(Pvector v1, Pvector v2) {
	v1 = normalize(v1);
	v2 = normalize(v2);
	return (v1.x * v2.x) + (v1.y * v2.y);
}