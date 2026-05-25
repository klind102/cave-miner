#ifndef WORLD_H
#define WORLD_H

#define CHUNK_WIDTH 256
#define CHUNK_HEIGHT 256
#define MAX_NUM_CHUNKS 16

typedef struct Chunk
{

  unsigned char data[CHUNK_WIDTH * CHUNK_HEIGHT];

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