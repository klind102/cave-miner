#ifndef MATERIALS_H
#define MATERIALS_H
// material types
#define AIR (unsigned char) 0
#define SAND (unsigned char) 1
#define STONE (unsigned char) 2

// directions
#define UP 0
#define DOWN 1
#define LEFT_RIGHT 2
#define UP_DIAG 3
#define DOWN_DIAG 4

typedef struct _Material
{
  unsigned char type;
  int moveOrder[5];
} Material;

static const Material MATERIAL_LOOKUP[] = {
    {AIR, {-1}},
    {SAND, {DOWN, DOWN_DIAG, -1}},
    {STONE, {-1}}};

#endif