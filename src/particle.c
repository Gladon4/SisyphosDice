#include "particle.h"
#include "stdlib.h"
#include "lists.h"


Particle* createParticle(Vector2 position, Vector2 absoluteVelocity, float relativeVelocity, float size, float amout, float density, float glowStrength, float trailLength, Color colour, bool isAffectedByGravity, float lifespan)
{
    Particle* particle = malloc(sizeof(Particle));

    ParticlePart** parts = malloc(sizeof(ParticlePart*)*amout);

    for(int i=0; i<amout; i++)
    {
        float positionX = ((float)rand()/(float)(RAND_MAX)) * 2 - 1;
        float positionY = ((float)rand()/(float)(RAND_MAX)) * 2 - 1;

        positionX *= (10/density);
        positionY *= (10/density);

        Vector2 position = (Vector2) {positionX, positionY};

        float velocityX = absoluteVelocity.x + (positionX / 100) + (relativeVelocity / 30) * positionX;
        float velocityY = absoluteVelocity.y + (positionY / 100) + (relativeVelocity / 30) * positionY;

        Vector2 velocity = (Vector2) {velocityX, velocityY};

        ParticlePart* part = malloc(sizeof(ParticlePart));
        *part = (ParticlePart)
        {
            .position = position,
            .velocity = velocity,

            .remaining_lifespan = lifespan,
            .trailPositions = createLinkedList()
        };
    }

    *particle = (Particle)
    {
        .size = size,
        .glowStrength = glowStrength,
        .trailLength = trailLength,
        .colour = colour,
        .isAffectedByGravity = isAffectedByGravity,
        .total_lifespan = lifespan,
        .parts = parts
    };

    return particle;
}

