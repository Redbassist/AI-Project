#include "Swarmer.h" 

#define PI 3.141592635

using namespace std;

Swarmer::Swarmer(float x, float y, Player* p) {
	player = p;
	window_height = globalBounds.y;
	window_width = globalBounds.x;
	maxSpeed = 5.5;
	maxForce = 0.5;
	velocity = Pvector(rand() % 3 - 1, rand() % 3 - 1);
	acceleration = Pvector(0, 0);
	location = Pvector(x, y);
	LoadAssets();

	currentState = State::SEARCH;

	health = 1000; 
}

Swarmer::~Swarmer()
{
	cout << "Boid is being deleted by destructor!" << endl;
}

void Swarmer::LoadAssets()
{
	if (!uitexture.loadFromFile("Sprites/blip.png"))
	{
		cout << "cant find image";
	}

	uitexture.setSmooth(true);
	uisprite.setTexture(uitexture);
	uisprite.setOrigin(uitexture.getSize().x / 2, uitexture.getSize().y / 2);
	uisprite.setScale(0.1, 0.1);

	if (!texture.loadFromFile("Sprites/predator.png"))
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
Pvector Swarmer::Separation(vector<Boid*> boids)
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

Pvector Swarmer::AvoidAsteroids()
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
Pvector Swarmer::Alignment(vector<Boid*> Boids)
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
Pvector Swarmer::Cohesion(vector<Boid*> Boids)
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

Pvector Swarmer::ChasePlayer()
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

//Update modifies velocity, location, and resets acceleration with values that
//are given by the three laws.
void Swarmer::update(vector <Boid*> v)
{ 
	//"FINITE STATE MACHINE"
	switch (currentState) {
	case(State::SEARCH) :
		run(v);
		if (search()) {
			currentState = State::ATTACK;
			maxSpeed = 6.5;
		}
		else if (checkAsteroids()) {
			currentState = State::AVOIDASTEROID;
		}
		break;

	case(State::AVOIDASTEROID) :
		avoid(v);
		if (!checkAsteroids()) {
			currentState = State::SEARCH;
			maxSpeed = 5.5;
		}
		break;

	case(State::ATTACK) :
		chase(v);
		if (lost()) {
			currentState = State::SEARCH;
			maxSpeed = 5.5;
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
void Swarmer::run(vector <Boid*> v)
{
	swarm(v);
	borders();
} 

void Swarmer::chase(vector <Boid*> v)
{
	Pvector sep = Separation(v); 
	Pvector chs = ChasePlayer();
	sep.mulScalar(1.0); 
	chs.mulScalar(1.5);
	applyForce(chs);
	applyForce(sep); 
	borders(); 
}

void Swarmer::avoid(vector<Boid*> v)
{
	Pvector sep = Separation(v);
	Pvector astAvoid = AvoidAsteroids();
	sep.mulScalar(1.0);
	astAvoid.mulScalar(1.0);
	applyForce(sep);
	applyForce(astAvoid);
	borders();
} 

// Checks if boids go out of the window and if so, wraps them around to the other side.
void Swarmer::borders()
{
	if (location.x < 0) location.x += window_width;
	if (location.y < 0) location.y += window_height;
	if (location.x > window_width) location.x -= window_width;
	if (location.y > window_height) location.y -= window_height;
}

// Calculates the angle for the velocity of a boid which allows the visual 
// image to rotate in the direction that it is going in.
float Swarmer::angle(Pvector v)
{
	// From the definition of the dot product
	float angle = (float)(atan2(v.x, -v.y) * 180 / PI);
	return angle;
}

bool Swarmer::checkAsteroids()
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

bool Swarmer::search()
{
	if (player->getPosition().distance(location) < 400) {
		return true;
	}
	return false;
}

bool Swarmer::lost()
{
	if (player->getPosition().distance(location) > 400) {
		return true;
	}
	return false;
}

void Swarmer::swarm(vector <Boid*> v)
{
	/*Vector R = me.position - you.position
		Real D = R.magnitude()
		Real U = -A / pow(D, N) + B / pow(D, M)
		R.normalise()

		Me.force += vrotate2D(me.Orientation, R*U)*/

	Pvector	R;
	int A = 100;
	int B = 3000;
	int N = 1;
	int M = 2;
	int count = 0;
	float totalForce = 0;
	Pvector sum(0, 0);

	for (int i = 0; i < v.size(); i++)
	{
		R = R.subTwoVector(location, v[i]->location);
		float D = R.magnitude();
		if (D > 0)
		{
			float U = -A / pow(D, N) + B / pow(D, M);
			R.normalize();
			R.mulScalar(U);
			sum.addVector(R);
		}

	}
	sum.divScalar(v.size() - 1);
	applyForce(sum);  
}

