#ifndef MATERIALS_H
#define MATERIALS_H

typedef enum
{
  UP,
  DOWN,
  LEFT_RIGHT,
  UP_DIAG,
  DOWN_DIAG,

} MaterialMove;

typedef struct Material
{
  unsigned char type;
  int numVariants;
  float density;
  int moveOrder[5];

} Material;

// material types
typedef enum MaterialType
{
  UNKNOWN = -1,
  AIR,
  SAND,
  STONE,
  WATER,
  LAVA,
  STEAM,
  FIRE,
  SMOKE,

  NUM_MATERIALS

} MaterialType;

static const Material MATERIAL_LOOKUP[] = {
    {AIR, 1, 0, {-1}},
    {SAND, 10, 1.5, {DOWN, DOWN_DIAG, -1}},
    {STONE, 10, 2.7, {-1}},
    {WATER, 1, 1.0, {DOWN, DOWN_DIAG, LEFT_RIGHT, -1}},
    {LAVA, 1, 2.4, {DOWN, DOWN_DIAG, LEFT_RIGHT, -1}},
    {STEAM, 1, 0.006, {UP, UP_DIAG, LEFT_RIGHT, -1}},
    {FIRE, 10, 0.001, {UP, UP_DIAG, LEFT_RIGHT, -1}},
    {SMOKE, 1, 0.004, {UP, UP_DIAG, LEFT_RIGHT, -1}},

};

typedef struct Reaction
{
  char result;
  float chance;

} Reaction;

static Reaction REACTIONS_LOOKUP[NUM_MATERIALS][NUM_MATERIALS];

#define INIT_REACTIONS()                                    \
  for (int i = 0; i < NUM_MATERIALS; i++)                   \
    for (int j = 0; j < NUM_MATERIALS; j++)                 \
      REACTIONS_LOOKUP[i][j] = (Reaction){UNKNOWN, 0.0f};   \
                                                            \
  REACTIONS_LOOKUP[SAND][LAVA] = (Reaction){LAVA, 0.1f};    \
                                                            \
  REACTIONS_LOOKUP[AIR][LAVA] = (Reaction){FIRE, 0.005f};   \
                                                            \
  REACTIONS_LOOKUP[WATER][LAVA] = (Reaction){STEAM, 0.9f};  \
  REACTIONS_LOOKUP[LAVA][WATER] = (Reaction){STONE, 0.05f}; \
                                                            \
  REACTIONS_LOOKUP[STEAM][AIR] = (Reaction){WATER, 0.001f}; \
                                                            \
  REACTIONS_LOOKUP[FIRE][AIR] = (Reaction){AIR, 0.1f};      \
  REACTIONS_LOOKUP[AIR][FIRE] = (Reaction){SMOKE, 0.01f};

#endif