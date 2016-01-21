#include "Predator.h" 

#define PI 3.141592635

using namespace std;

Predator::Predator(float x, float y, Player* p, int f) {
	player = p;
	currentFlock = f;
	window_height = globalBounds.y;
	window_width = globalBounds.x;
	maxSpeed = 7.5;
	maxForce = 0.5;
	velocity = Pvector(rand() % 3 - 1, rand() % 3 - 1);
	acceleration = Pvector(0, 0);
	location = Pvector(x, y);
	LoadAssets();

	currentState = State::SEARCH;

	health = 1000;
	fireRate = 120;
}

Predator::~Predator()
{
	cout << "Boid is being deleted by destructor!" << endl;
}

void Predator::LoadAssets()
{
	if (!texture.loadFromFile("Sprites/predator" + to_string(currentFlock) + ".png"))
	{
		cout << "cant find image";
	}

	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

	float scale = 0.04;
	sprite.setScale(scale, scale);

	radius = texture.getSize().x / 2 * scale;
	sprite.setPosition(sf::Vector2f(location.x, location.y));

}

// Function that checks and modifies the distance
// of a boid if it breaks the law of separation.
Pvector Predator::Separation(vector<Boid*> boids)
{
	// Distance of field of vision for separation between boids
	float desiredseparation = 20;

	Pvector steer(0, 0);
	int count = 0;
	// For every boid in the system, check if it's too close
	for (int i = 0; i < boids.size(); i++)
	{
		// Calculate distance from current boid to boid we're looking at
		float d = location.distance(boids[i]->location);
		// If this is a fellow boid and it's too close, move away from it
		if ((d > 0) && (d < desiredseparation))
		{
			Pvector diff(0, 0);
			diff = diff.subTwoVector(location, boids[i]->location);
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

Pvector Predator::AvoidAsteroids()
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
Pvector Predator::Alignment(vector<Boid*> Boids)
{
	float neighbordist = 150;

	Pvector sum(0, 0);
	int count = 0;
	for (int i = 0; i < Boids.size(); i++)
	{
		float d = location.distance(Boids[i]->location);
		if ((d > 0) && (d < neighbordist)) // 0 < d < 50
		{
			sum.addVector(Boids[i]->velocity);
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
Pvector Predator::Cohesion(vector<Boid*> Boids)
{
	float neighbordist = 40;

	Pvector sum(0, 0);
	int count = 0;
	for (int i = 0; i < Boids.size(); i++)
	{
		float d = location.distance(Boids[i]->location);
		if ((d > 0) && (d < neighbordist))
		{
			sum.addVector(Boids[i]->location);
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

Pvector Predator::ChasePlayer()
{
	Pvector chaseVect;
	chaseVect = player->getPosition() - location;

	chaseVect.normalize();	   		// Turn sum into a unit vector, and
	chaseVect.mulScalar(maxSpeed);    // Multiply by maxSpeed
								// Steer = Desired - Velocity
	Pvector steer;
	steer = steer.subTwoVector(chaseVect, velocity); //sum = desired(average)  
	steer.limit(maxForce);
	return steer;
}

Pvector Predator::CollectPowerup()
{
	Pvector collectVect;

	PowerUp* closestPowerup = NULL;
	float shortestDist = 10000;

	int size = PowerUpManager::GetInstance()->powerups.size();
	
	for (int i = 0; i < size; i++) {
		float distance = location.distance(PowerUpManager::GetInstance()->powerups[i]->getPos());
		if (distance < shortestDist) {
			closestPowerup = PowerUpManager::GetInstance()->powerups[i];
			shortestDist = distance;
		}
	} 
	if (closestPowerup != NULL) {
		collectVect = closestPowerup->getPos() - location;

		collectVect.normalize();	   		// Turn sum into a unit vector
		collectVect.mulScalar(maxSpeed);    // Multiply by maxSpeed
	}				  

	Pvector steer;
	steer = steer.subTwoVector(collectVect, velocity);
	steer.limit(maxForce);
	return steer;
}

void Predator::LimitAcceleration()
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

//Update modifies velocity, location, and resets acceleration with values that
//are given by the three laws.
void Predator::update(vector <Boid*> v)
{
	fireTimer++;

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
		else if (checkPowerUps()) {
			currentState = State::POWERUP;
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
		else if (checkPowerUps()) {
			currentState = State::POWERUP;
		}
		break;
	case(State::POWERUP) :
		collect(v);
		if (!checkPowerUps()) {
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
	location.addVector(velocity);
	// Reset accelertion to 0 each cycle
	acceleration.mulScalar(0);

	sprite.setRotation(angle(velocity));
	sprite.setPosition(Vector2f(location.x, location.y));
}

//Run runs flock on the flock of boids for each boid.
//Which applies the three rules, modifies accordingly, updates data, checks is data is
//out of range, fixes that for SFML, and renders it on the window.
void Predator::run(vector <Boid*> v)
{
	flock(v);
	borders();
}

//Applies all three laws for the flock of boids and modifies to keep them from
//breaking the laws.
void Predator::flock(vector<Boid*> v)
{
	Pvector sep = Separation(v);
	Pvector ali = Alignment(BoidManager::GetInstance()->flocks[currentFlock]);
	Pvector coh = Cohesion(BoidManager::GetInstance()->flocks[currentFlock]);
	// Arbitrarily weight these forces
	sep.mulScalar(1.5);
	ali.mulScalar(1.0);
	coh.mulScalar(1.0);
	// Add the force vectors to acceleration
	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
}

void Predator::chase(vector <Boid*> v)
{
	Pvector sep = Separation(v);
	Pvector ali = Alignment(BoidManager::GetInstance()->flocks[currentFlock]);
	Pvector coh = Cohesion(BoidManager::GetInstance()->flocks[currentFlock]);
	Pvector chs = ChasePlayer();
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
	if (distancePlayer < 300) {
		Shoot();
	}
}

void Predator::collect(vector<Boid*> v)
{
	Pvector sep = Separation(v); 
	Pvector col = CollectPowerup();
	sep.mulScalar(1.0); 
	col.mulScalar(1.5);
	applyForce(col);
	applyForce(sep);  
	borders(); 
}

void Predator::avoid(vector<Boid*> v)
{
	Pvector sep = Separation(v);
	Pvector astAvoid = AvoidAsteroids();
	sep.mulScalar(1.0);
	astAvoid.mulScalar(1.7);
	applyForce(sep);
	applyForce(astAvoid);
	borders();
}

void Predator::Shoot()
{
	if (fireTimer > fireRate) {
		fireTimer = 0;
		Bullet* bullet = new Bullet((location), velocity, false);
		BulletManager::GetInstance()->AddBullet(bullet);
	}
}

// Checks if boids go out of the window and if so, wraps them around to the other side.
void Predator::borders()
{
	if (location.x < 0) location.x += window_width;
	if (location.y < 0) location.y += window_height;
	if (location.x > window_width) location.x -= window_width;
	if (location.y > window_height) location.y -= window_height;
}

// Calculates the angle for the velocity of a boid which allows the visual 
// image to rotate in the direction that it is going in.
float Predator::angle(Pvector v)
{
	// From the definition of the dot product
	float angle = (float)(atan2(v.x, -v.y) * 180 / PI);
	return angle;
}

bool Predator::checkAsteroids()
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

bool Predator::checkPowerUps()
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

bool Predator::search()
{
	if (player->getPosition().distance(location) < 450) {
		return true;
	}
	return false;
}

bool Predator::lost()
{
	if (player->getPosition().distance(location) > 500) {
		return true;
	}
	return false;
} 

