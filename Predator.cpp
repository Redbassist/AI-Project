#include "Predator.h" 
 
#define PI 3.141592635

using namespace std; 

Predator::Predator(float x, float y, Player* p) {
	player = p;
	window_height = globalBounds.y;
	window_width = globalBounds.x;
	maxSpeed = 7.5;
	maxForce = 0.5;
	velocity = Pvector(rand() % 3 - 1, rand() % 3 - 1);
	acceleration = Pvector(0, 0);
	location = Pvector(x, y);
	LoadAssets();

	currentState = State::SEARCH;
}

Predator::~Predator()
{
	cout << "Boid is being deleted by destructor!" << endl;
}

void Predator::LoadAssets()
{
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
		// If current boid is a predator and the boid we're looking at is also
		// a predator, then separate only slightly 
		if ((d > 0) && (d < desiredseparation))
		{
			Pvector pred2pred(0, 0);
			pred2pred = pred2pred.subTwoVector(location, boids[i]->location);
			pred2pred.normalize();
			pred2pred.divScalar(d);
			steer.addVector(pred2pred);
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

//Update modifies velocity, location, and resets acceleration with values that
//are given by the three laws.
void Predator::update(vector <Boid*> v)
{
	switch (currentState) {
	case(State::SEARCH) :
		run(v);
		if (search()) { 
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
	Pvector ali = Alignment(v);
	Pvector coh = Cohesion(v);
	// Arbitrarily weight these forces
	sep.mulScalar(1.5);
	ali.mulScalar(1.0); // Might need to alter weights for different characteristics
	coh.mulScalar(1.0);
	// Add the force vectors to acceleration
	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
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

bool Predator::search()
{
	if (player->getPosition().distance(location) < 600) {
		return true;
	}
	return false;
}

bool Predator::lost()
{
	if (player->getPosition().distance(location) > 600) {
		return true;
	}
	return false;
}

/*
void Predator::swarm(vector <Boid*> v)
{
			Vector R = me.position - you.position
	Real D = R.magnitude()
	Real U = -A / pow(D, N) + B / pow(D, M)
	R.normalise()

	Me.force += vrotate2D(me.Orientation, R*U)
	
	Pvector	R;
	int A = 100;
	int B = 5000;
	int N = 1;
	int M = 2;
	int count = 0;
	float totalForce = 0;
	Pvector sum(0, 0);

	for (int i = 0; i < v.size(); i++)
	{
		R = R.subTwoVector(location, v[i].location);
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
	update();
	borders();
}*/
