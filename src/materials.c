#include <materials.h>
#include <world.h>
#include <stdlib.h>

Material getMaterial(unsigned char type)
{
  if (type >= sizeof(MATERIAL_LOOKUP) / sizeof(Material))
    return MATERIAL_LOOKUP[AIR];

  return MATERIAL_LOOKUP[type];
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
    *dx = (rand() % 2) ? -1 : 1;
    *dy = 0;
    break;
  case UP_DIAG:
    *dx = switchLR ? -1 : 1;
    *dx = (rand() % 2) ? -1 : 1;
    *dy = 1;
    break;
  case DOWN_DIAG:
    *dx = switchLR ? -1 : 1;
    *dx = (rand() % 2) ? -1 : 1;
    *dy = -1;
    break;
  default:
    break;
  }
  switchLR = !switchLR;
}
#define cell(x, y) chunkData[(x) + (y) * CHUNK_WIDTH]
void materialMove(int x, int y, unsigned char *chunkData)
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
    if (nx >= 0 && nx < CHUNK_WIDTH && ny >= 0 && ny < Chunk_HEIGHT)
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
#undef cell