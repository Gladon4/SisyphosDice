#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "raylib.h"
#include "lists.h"

typedef struct ParticlePart
{
    Vector2 position;
    Vector2 velocity;

    float remaining_lifespan;
    Vector2LinkedList trailPositions;

} ParticlePart;


typedef struct Particle 
{
    float size;
    float glowStrength;
    float trailLength;
    Color colour;
    bool isAffectedByGravity;

    float total_lifespan;

    ParticlePart** parts;
} Particle;

Particle* createParticle(Vector2 position, Vector2 absoluteVelocity, float relativeVelocity, float size, float amout, float density, float glowStrength, float trailLength, Color colour, bool isAffectedByGravity, float lifespan);

void UpdateParticle(Particle* particle, float gravity, float deltaTime);

void DrawParticle(Particle* perticle);

#endif