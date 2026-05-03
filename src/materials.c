#include "materials.h"

// directions
#define UP 0
#define DOWN 1
#define LEFT_RIGHT 2
#define UP_DIAG 3
#define DOWN_DIAG 4

#define MAT_AIR \
  (Material) { .type = AIR, .moveOrder = {-1} }
#define MAT_SAND \
  (Material) { .type = SAND, .moveOrder = {DOWN, DOWN_DIAG, -1} }
#define MAT_STONE \
  (Material) { .type = STONE, .moveOrder = {-1} }

Material getMaterial(unsigned char type)
{
  switch (type)
  {
  case AIR:
    return MAT_AIR;
  case SAND:
    return MAT_SAND;
  case STONE:
    return MAT_STONE;
    
  default:
    return MAT_AIR;
  }
}

int switchLR = 1;
void getDxDy(int direction, int *dx, int *dy)
{
  switch (direction)
  {
  case UP:
    *dx = 0;
    *dy = 1;
    break;
  case DOWN:
    *dx = 0;
    *dy = -1;
    break;
  case LEFT_RIGHT:
    *dx = switchLR ? -1 : 1;
    *dy = 0;
    break;
  case UP_DIAG:
    *dx = switchLR ? -1 : 1;
    *dy = 1;
    break;
  case DOWN_DIAG:
    *dx = switchLR ? -1 : 1;
    *dy = -1;
    break;
  default:
    break;
  }
  switchLR = !switchLR;
}

#define cell(x, y) world[(x) + (y) * width]

void materialMove(int x, int y, int width, int height, unsigned char *world)
{
  Material m = getMaterial(cell(x, y));
  int dx = 0, dy = 0;
  for (int i = 0; i < 5; i++)
  {
    if (m.moveOrder[i] == -1)
      break;

    int tx, ty;
    getDxDy(m.moveOrder[i], &tx, &ty);

    int nx = x + tx;
    int ny = y + ty;

    // Check bounds and if the target cell is AIR
    if (nx >= 0 && nx < width && ny >= 0 && ny < height)
    {
      if (cell(nx, ny) == AIR)
      {
        dx = tx;
        dy = ty;
        break; // Found a valid move
      }
    }
  }

  if (dx || dy)
  {
    unsigned char temp = cell(x, y);
    cell(x, y) = cell(x + dx, y + dy);
    cell(x + dx, y + dy) = temp;
  }
}