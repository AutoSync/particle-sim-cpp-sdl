#include "particleSystem.h"

ParticleSystem::ParticleSystem() : gravity(98.0f), enableGravity(true), enableCollisions(true) {}

void ParticleSystem::addParticle(const Particle& particle) {
    particles.push_back(particle);
}

void ParticleSystem::update(float deltaTime, int width, int height) {
    // Aplicar for�as
    for (auto& particle : particles) {
        if (enableGravity) {
            particle.applyForce(0, gravity * particle.mass);
        }
    }

    // Atualizar part�culas
    for (auto& particle : particles) {
        particle.update(deltaTime);
        particle.checkBoundaryCollision(width, height);
    }

    // Detectar colis�es entre part�culas
    if (enableCollisions) {
        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                particles[i].resolveCollision(particles[j]);
            }
        }
    }
}

void ParticleSystem::render(SDL_Renderer* renderer) {
    for (auto& particle : particles) {
        particle.render(renderer);
    }
}

void ParticleSystem::clear() {
    particles.clear();
}