#include "Particle.h"

Particle::Particle(float x, float y, float mass, float radius) 
	: x(x), y(y), mass(mass), radius(radius), fixed(false)
{
	vx = vy = vy = ax = ay = 0.0f;
	color = { 255, 255, 255, 255 };
}

void Particle::applyForce(float fx, float fy)
{
	if(!fixed)
	{
		ax += fx / mass;
		ay += fy / mass;
	}
}

void Particle::update(float dt)
{
	if (fixed) return;

	// Verlet integration
	float oldvx = vx;
	float oldvy = vy;

	vx += ax * dt;
	vy += ay * dt;

	x += (oldvx + vx) * dt * 0.5f;
	y += (oldvy + vy) * dt * 0.5f;

	//Reset acceleration
	ax = 0.0f;
	ay = 0.0f;
}

void Particle::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	// Draw a circle
	for(int w = -radius; w <= radius; w++)
	{
		for(int h = -radius; h <= radius; h++)
		{
			if(w * w + h * h < radius * radius)
			{
				SDL_RenderPoint(renderer, static_cast<int>(x + w), static_cast<int>(y + h));
			}
		}
	}
}

void Particle::checkBoundaryCollision(int width, int height)
{
	if(fixed) return;

	if (x - radius < 0)
	{
		x = radius;
		vx = -vx * 0.8f;	
	}
	else if (x + radius > width) {
		x = width - radius;
		vx = -vx * 0.8f;
	}

	if (y - radius < 0) {
		y = radius;
		vy = -vy * 0.8f;
	}
	else if (y + radius > height) {
		y = height - radius;
		vy = -vy * 0.8f;
	}
}

void Particle::resolveCollision(Particle& other)
{
	if (fixed && other.fixed) return;

	float dx = other.x - x;
	float dy = other.y - y;
	float dist = sqrt(dx * dx + dy * dy);
	float minDist = radius + other.radius;

	if (dist < minDist && dist > 0) {
		// Colisão detectada
		float angle = atan2(dy, dx);
		float overlap = minDist - dist;

		// Separar partículas
		float separationX = cos(angle) * overlap * 0.5f;
		float separationY = sin(angle) * overlap * 0.5f;

		if (!fixed) {
			x -= separationX;
			y -= separationY;
		}
		if (!other.fixed) {
			other.x += separationX;
			other.y += separationY;
		}

		// Calcular novas velocidades (colisão elástica)
		float nx = dx / dist;
		float ny = dy / dist;
		float p = 2.0 * (vx * nx + vy * ny - other.vx * nx - other.vy * ny) /
			(mass + other.mass);

		if (!fixed) {
			vx = vx - p * other.mass * nx;
			vy = vy - p * other.mass * ny;
		}
		if (!other.fixed) {
			other.vx = other.vx + p * mass * nx;
			other.vy = other.vy + p * mass * ny;
		}
	}
}
