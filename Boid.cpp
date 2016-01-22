// This file defines the boid class. This includes the attributes found in
// boids -- speed, location on the board, acceleration, etc.
#include "Boid.h" 

// Global Variables for borders()
// desktopTemp gets screen resolution of PC running the program 
const int window_height = globalBounds.y;
const int window_width = globalBounds.x;

#define w_height window_height
#define w_width window_width
#define PI 3.141592635

using namespace std;

// =============================================== //
// ======== Boid Functions from Boid.h =========== //
// =============================================== //

// Adds force Pvector to current force Pvector
void Boid::applyForce(Pvector force)
{
	acceleration.addVector(force);
}

// Function that checks and modifies the distance
// of a boid if it breaks the law of separation.
Pvector Boid::Separation(vector<Boid*> boids)
{
	return Pvector(0, 0);
}

// Alignment calculates the average velocity in the field of view and 
// manipulates the velocity of the Boid passed as parameter to adjust to that
// of nearby boids.
Pvector Boid::Alignment(vector<Boid*> Boids)
{
	return Pvector(0, 0);
}

// Cohesion finds the average location of nearby boids and manipulates the 
// steering force to move in that direction.
Pvector Boid::Cohesion(vector<Boid*> Boids)
{
	return Pvector(0, 0);
}

// Seek function limits the maxSpeed, finds necessary steering force and
// normalizes the vectors.
Pvector Boid::seek(Pvector v)
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

//Update modifies velocity, location, and resets acceleration with values that
//are given by the three laws.
void Boid::update(vector <Boid*> v)
{ 
}

void Boid::draw()
{
	window->draw(sprite);
}

void Boid::drawui()
{
	View tempView = window->getView();
	Vector2f viewPos = tempView.getCenter();
	
	float d = location.distance(player->getPosition());
	if (d < 1300) {
		Pvector uiPos = location - player->getPosition();

		uiPos.divScalar(20);

		uisprite.setPosition(Vector2f(viewPos.x + 520 + uiPos.x, viewPos.y - 270 + uiPos.y));

		window->draw(uisprite);
	}
}

//Run runs flock on the flock of boids for each boid.
//Which applies the three rules, modifies accordingly, updates data, checks is data is
//out of range, fixes that for SFML, and renders it on the window.
void Boid::run(vector <Boid*> v)
{
}

//Applies all three laws for the flock of boids and modifies to keep them from
//breaking the laws.
void Boid::flock(vector<Boid*> v)
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
void Boid::borders()
{
	if (location.x < 0) location.x += w_width;
	if (location.y < 0) location.y += w_height;
	if (location.x > w_width) location.x -= w_width;
	if (location.y > w_height) location.y -= w_height;
}

// Calculates the angle for the velocity of a boid which allows the visual 
// image to rotate in the direction that it is going in.
float Boid::angle(Pvector v)
{
	// From the definition of the dot product
	float angle = (float)(atan2(v.x, -v.y) * 180 / PI);
	return angle;
}

void Boid::swarm(vector <Boid*> v)
{
	/*		Vector R = me.position - you.position
	Real D = R.magnitude()
	Real U = -A / pow(D, N) + B / pow(D, M)
	R.normalise()

	Me.force += vrotate2D(me.Orientation, R*U)
	*/
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
	update(v);
	borders();
}

float Boid::getHealth()
{
	return health;
}

void Boid::setHealth(float h)
{
	health = h;
}

float Boid::getRadius()
{
	return radius;
}

Pvector Boid::getPos()
{
	Pvector temp;
	temp.x = sprite.getPosition().x;
	temp.y = sprite.getPosition().y;

	return temp;
}

bool Boid::LifeCheck()
{
	bool destroy = false;

	if (health <= 0)
	{
		destroy = true;
	}

	return destroy;
}

