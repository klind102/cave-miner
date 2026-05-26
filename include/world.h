#ifndef WORLD_H
#define WORLD_H

#define CHUNK_WIDTH 256
#define CHUNK_HEIGHT 256
#define MAX_NUM_CHUNKS 16

#define CHUNK_UP_LEFT 0
#define CHUNK_UP 1
#define CHUNK_UP_RIGHT 2
#define CHUNK_LEFT 3
#define CHUNK_RIGHT 5
#define CHUNK_DOWN_LEFT 6
#define CHUNK_DOWN 7
#define CHUNK_DOWN_RIGHT 8

typedef struct Cell{
  unsigned char type;
  unsigned char variant;

} Cell;

typedef struct Chunk
{

  Cell data[CHUNK_WIDTH * CHUNK_HEIGHT];

  int xpos;
  int ypos;
  int loadedID;

  //Indices:
  // 0 1 2
  // 3 4 5
  // 6 7 8
  struct Chunk *neigbors[9];



} Chunk;

#endif