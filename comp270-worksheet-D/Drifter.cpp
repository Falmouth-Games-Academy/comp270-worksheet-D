#include "stdafx.h"
#include "Drifter.h"
#include "Matrix2D.h"

// Update's the Drifter's position, if it is still alive
void Drifter::update()
{
	if (this != nullptr && m_alive)
	{
		// Update the position, and anything the derived class wants to
		m_position = m_position + m_velocity;
		//updateExtra();
	}
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

	// Find the local points in world space by applying a matrix transformation
	Matrix2D worldTransform;
	worldTransform.setTransform(m_position, m_rotation);
	Point2D worldVerts[NumVerts];
	for (unsigned i = 0; i < NumVerts; ++i)
		worldVerts[i] = worldTransform * m_localVerts[i];
	
	// Compute the triangle areas to reduce computation time on collision detection
	for (unsigned i = 1; i < NumVerts - 2; i++)
	{
		triangleAreas[i] = Point2D::getTriangleArea2(worldVerts[i], worldVerts[i + 1], worldVerts[i + 2]);
	}
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
		for (unsigned i = 0; i < NumVerts; ++i)
			worldVerts[i] = worldTransform * m_localVerts[i];

		// Draw the transformed points
		for (unsigned i = 0; i < NumVerts - 1; ++i)
			SDL_RenderDrawLineF(renderer, worldVerts[i].x, worldVerts[i].y, worldVerts[i + 1].x, worldVerts[i + 1].y);
		SDL_RenderDrawLineF(renderer, worldVerts[NumVerts - 1].x, worldVerts[NumVerts - 1].y, worldVerts[0].x, worldVerts[0].y);
	}
}

bool Asteroid::pointIsInsideTriangle(const Point2D &p, const Point2D &a, const Point2D &b, const Point2D &c, int triangleIndex, int decimalErrorMargin) const
{
	float area = triangleAreas[triangleIndex];
	float pArea = Point2D::getTriangleArea2(p, a, b) + Point2D::getTriangleArea2(p, a, c) + Point2D::getTriangleArea2(p, b, c);

	int iArea = (int)(area * pow(10, decimalErrorMargin));
	int iPArea = (int)(pArea * pow(10, decimalErrorMargin));
	return iArea == iPArea;
}

// Returns true if the given point is currently inside the Asteroid, false if not.
bool Asteroid::pointIsInside(Point2D point) const
 {
	// Find the local points in world space by applying a matrix transformation
	Matrix2D worldTransform;
	worldTransform.setTransform(m_position, m_rotation);
	Point2D worldVerts[NumVerts];
	for (unsigned i = 0; i < NumVerts; ++i)
		worldVerts[i] = worldTransform * m_localVerts[i];

	int collidingTriangles = 0;

	// Check for collisions with all possible triangles
	for (unsigned i = 1; i < NumVerts - 1; i++)
	{
		collidingTriangles += pointIsInsideTriangle(point, worldVerts[0], worldVerts[i], worldVerts[i + 1], i - 1);
	}

	return collidingTriangles % 2;
}
