#ifndef MATERIALS_H
#define MATERIALS_H
// material types
#define AIR (unsigned char) 0
#define SAND (unsigned char) 1
#define STONE (unsigned char) 2
#define WATER (unsigned char) 3
#define LAVA (unsigned char) 4




// directions
#define UP 0
#define DOWN 1
#define LEFT_RIGHT 2
#define UP_DIAG 3
#define DOWN_DIAG 4

typedef struct _Material
{
  unsigned char type;
  int numVariants;
  float density;

  
  int moveOrder[5];
} Material;

static const Material MATERIAL_LOOKUP[] = {
    {AIR, 1, 0, {-1}},
    {SAND, 10, 1.5, {DOWN, DOWN_DIAG, -1}},
    {STONE, 10, 2.7, {-1}},
    {WATER, 1, 1.0, {DOWN, DOWN_DIAG, LEFT_RIGHT, -1}},
    {LAVA, 1, 2.4, {DOWN, DOWN_DIAG, LEFT_RIGHT, -1}},

  };
    

#endif