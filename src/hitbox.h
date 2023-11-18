#ifndef HITBOX_H_
#define HITBOX_H_

#include "raylib.h"

typedef struct Hitbox
{
    Rectangle rect;
    char* tag;
} Hitbox;

#endif
