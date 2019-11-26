#include "stdafx.h"
#include "Drifter.h"
#include "Matrix2D.h"

// Update's the Drifter's position, if it is still alive
void Drifter::update()
{
	if (m_alive)
	{
		// Update the position, and anything the derived class wants to
		m_position = m_position + m_velocity;
//		updateExtra();	// Access violation ??
	}
}

void Drifter::resetObject(Point2D position, Vector2D velocity)
{
	if (m_alive) return;

	// set the new position and velocity 
	m_position = position;
	m_velocity = velocity;

	m_alive = true;	// i will rule the world!
}

// A Bullet is just a dot
void Bullet::draw(SDL_Renderer * renderer) const
{
	if (m_alive)
		SDL_RenderDrawPointF(renderer, m_position.x, m_position.y);
}

// Creates an Asteroid with the given parameters, scaling the points up to the correct size.
Asteroid::Asteroid(Point2D position, Vector2D velocity, float scale, float rotationSpeed) :
	Drifter(position, velocity), m_scale(scale), m_rotationSpeed(rotationSpeed)
{
	for (unsigned i = 0; i < NumVerts; ++i)
		m_localVerts[i] = m_localVerts[i].asVector() * m_scale;
}

// Draws the asteroid on the screen
void Asteroid::draw(SDL_Renderer * renderer) const
{
	if (m_alive)
	{
		// Find the local points in world space by applying a matrix transformation
		Matrix2D worldTransform;
		worldTransform.setTransform(m_position, m_rotation);

		Point2D worldVerts[NumVerts];
			
		// work out the first vert in world space, then we'll get the next required vert in the for loop
		worldVerts[0] = worldTransform * m_localVerts[0];

		// Draw the transformed points
		for (unsigned i = 0; i < NumVerts - 1; ++i)
		{
			worldVerts[i+1] = worldTransform * m_localVerts[i+1];	// work out the next vert.
			SDL_RenderDrawLineF(renderer, worldVerts[i].x, worldVerts[i].y, worldVerts[i + 1].x, worldVerts[i + 1].y);

			// Draw some collider debug lines.
			if (c_debug_drawCollider && i > 1)
			{
				SDL_SetRenderDrawColor(renderer, 92, 0, 0, 128);
				SDL_RenderDrawLineF(renderer, worldVerts[i].x, worldVerts[i].y, worldVerts[0].x, worldVerts[0].y);
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			}

		}

		SDL_RenderDrawLineF(renderer, worldVerts[NumVerts - 1].x, worldVerts[NumVerts - 1].y, worldVerts[0].x, worldVerts[0].y);
	}
}

// Returns true if the given point is currently inside the Asteroid, false if not.
bool Asteroid::pointIsInside(Point2D point) const
{
	if ((point - m_position).magnitude() > 50)
		return false;
	// Find the local points in world space by applying a matrix transformation
	Matrix2D worldTransform;
	worldTransform.setTransform(m_position, m_rotation);
	Point2D worldVerts[NumVerts];
	for (unsigned i = 0; i < NumVerts; ++i)
		worldVerts[i] = worldTransform * m_localVerts[i];

	// Sum the signed angles between all pairs of vertices and the test point;
	// if the sum is near zero, the point is inside, otherwise it's outside.
	Vector2D v0 = worldVerts[NumVerts-1] - point, v1;
	float len = v0.magnitude();
	if (len < 0.0f)
		return true;	// point and vertex coincide
	v0.normalise();

	float angle = 0.0f, dot;
	for (unsigned i = 0; i < NumVerts; ++i)
	{
		v1 = worldVerts[i] - point;
		len = v1.magnitude();
		if (len < 0.0f)
			return true;	// point and vertex coincide
		v1.normalise();

		// Check if v1 is to the 'left' or 'right' of v0
		dot = v0.dot(v1);
		if (dot < -1.0f)
			angle += M_PI;	// Point lies on the edge, so add 180 degrees
		else if (dot < 1.0f)
		{
			if (v0.x * v1.y - v1.x * v0.y >= 0.0f)
				angle += acosf(dot);
			else
				angle -= acosf(dot);
		}

		// Move to next point
		v0 = v1;
	}

	return fmod( fabs(angle) + M_PI, 4.0f * M_PI ) > 2.0f * M_PI;
}

bool Asteroid::pointIsInside_convex(Point2D point) const
{

	if ((point - m_position).magnitude() > 50) return false;

	Matrix2D worldTransform;
	worldTransform.setTransform(m_position, m_rotation);
	Point2D worldVerts[NumVerts];

	// only find the first two verts in world space. we'll get the reset just befor we work out the collision.
	for (unsigned i = 0; i < 2; ++i)
		worldVerts[i] = worldTransform * m_localVerts[i];

	// we can start from point 2, since there must be three edges to a triangle
	for (unsigned i = 2; i < NumVerts - 1; ++i)
	{
		worldVerts[i] = worldTransform * m_localVerts[i];

		if (barycentricCollision(point, worldVerts[0], worldVerts[i - 1], worldVerts[i]))	// find if we are in a triangle
			return true;	

	}

	return false;

}

bool Asteroid::barycentricCollision(Point2D point, Point2D tri_origin, Point2D triPoint_b, Point2D triPoint_c) const
{
	// barycentric works by defining a origin on the plance that connects to the other to edges
	// then we can get any position with the plan by move along (b-origin) or (c-Origin)
	// if x/y is < 0 we have not reached the object yet, if x/y > 1 then we have pased the edge.
	// if the sum of x/y is > 1 then we have also left the tri.

	// find vectors
	Vector2D dif_pointBOrigin = triPoint_b - tri_origin;
	Vector2D dif_pointCOrigin = triPoint_c - tri_origin;
	Vector2D dif_pointOrigin = point - tri_origin;

	// Get the dot products
	// TODO Cache.
	float dot_CC = dif_pointCOrigin.dot(dif_pointCOrigin);
	float dot_CB = dif_pointCOrigin.dot(dif_pointBOrigin);
	float dot_COrigin = dif_pointCOrigin.dot(dif_pointOrigin);
	float dot_BB = dif_pointBOrigin.dot(dif_pointBOrigin);
	float dot_BOrigin = dif_pointBOrigin.dot(dif_pointOrigin);

	// find the coords
	float denorm = 1.0f / (dot_CC * dot_BB - dot_CB * dot_CB);	// cache
	float x = (dot_BB * dot_COrigin - dot_CB * dot_BOrigin) * denorm;
	float y = (dot_CC * dot_BOrigin - dot_CB * dot_COrigin) * denorm;


	// is point in tri?
	return x >= 0 && y >= 0 && (x + y < 1); // we dont have to check if X or Y is > 1 since we must check X+Y < 1
}