#ifndef PLAYER_H
#define PLAYER_H
#include "linmath.h"


#define CAMERA_ZOOM_SPEED 10.0f
#define CAMERA_MOVE_SPEED 800.0f

typedef struct Player
{
  mat4x4 camera_transform;
  vec2 camera_pos;
  float camera_zoom;

} Player;

#endif
