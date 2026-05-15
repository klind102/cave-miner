#ifndef WORLD_H
#define WORLD_H

#define CHUNK_WIDTH 256
#define CHUNK_HEIGHT 256
#define MAX_NUM_CHUNKS 16

typedef struct Chunk
{

  unsigned char data[CHUNK_WIDTH * CHUNK_HEIGHT];


  struct Chunk *left;
  struct Chunk *right;
  struct Chunk *up;
  struct Chunk *down;


} Chunk;

#endif