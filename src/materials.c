#include <materials.h>
#include <world.h>
#include <stdlib.h>
#include <utils.h>

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
    *dx = (rand() % 2) ? -1 : 1;
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
#define cell(p, x, y) (p->data)[(x) + (y) * CHUNK_WIDTH]
#define randf() (float)rand() / (float)RAND_MAX

void materialMove(int x, int y, Chunk *current)
{
  Cell temp = cell(current, x, y);
  const Material *currMat = &MATERIAL_LOOKUP[temp.type];

  if (currMat->moveOrder[0] == -1)
    return;

  for (int i = 0; i < 5; i++)
  {
    if (currMat->moveOrder[i] == -1)
      break;

    int tx, ty;
    getDxDy(currMat->moveOrder[i], &tx, &ty);

    int nx = x + tx;
    int ny = y + ty;

    // Determine which chunk the target cell belongs to
    int ndx = (nx < 0) ? -1 : (nx >= CHUNK_WIDTH ? 1 : 0);
    int ndy = (ny < 0) ? -1 : (ny >= CHUNK_HEIGHT ? 1 : 0);

    // Get target chunk (handle current chunk or neighbor)
    Chunk *target = (ndx == 0 && ndy == 0) ? current : current->neigbors[(ndx + 1) + (ndy + 1) * 3];

    if (target == NULL)
      continue;

    // Wrap local coordinates for the target chunk
    int lnx = (nx + CHUNK_WIDTH) % CHUNK_WIDTH;
    int lny = (ny + CHUNK_HEIGHT) % CHUNK_HEIGHT;

    const Material *targetMat = &MATERIAL_LOOKUP[cell(target, lnx, lny).type];


    if (currMat->density > targetMat->density && randf() > (targetMat->density / currMat->density))
    {
      cell(current, x, y) = cell(target, lnx, lny);
      cell(target, lnx, lny) = temp;
      return; // Successfully moved
    }
  }
}
#undef cell