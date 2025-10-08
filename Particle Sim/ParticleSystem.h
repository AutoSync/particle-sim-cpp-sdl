#pragma once
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Particle.h"
#include <vector>

class ParticleSystem
{
private:
	std::vector<Particle> particles;
	float gravity;
	bool enableGravity;
	bool enableCollisions;

public:
	ParticleSystem();
	void addParticle(const Particle& p);
	void update(float dt, int width, int height);
	void render(SDL_Renderer* renderer);
	void clear();

	void setGravity(float g) { gravity = g; }
	void setEnableGravity(bool g) { enableGravity = g; }
	void setEnableCollisions(bool c) { enableCollisions = c; }

	std::vector<Particle>& getParticles() { return particles; }
};

#endif