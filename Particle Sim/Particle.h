// Particle.h
// 2025 (c) Erick Andrade - All rights reserved

#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL3/SDL.h>
#include <cmath>
#include <vector>

class Particle {
public:
	float x, y;
	float vx, vy;
	float ax, ay;
	float mass;
	float radius;
	SDL_Color color;
	bool fixed;

	Particle(float x, float y, float mass = 1.0f, float radius = 5.0f);
	void applyForce(float fx, float fy);
	void update(float dt);
	void render(SDL_Renderer* renderer);
	void checkBoundaryCollision(int width, int height);
	void resolveCollision(Particle& other);
};

#endif 



