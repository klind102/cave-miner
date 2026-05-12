#ifndef WORLD_H
#define WORLD_H

#define CHUNK_WIDTH 400
#define Chunk_HEIGHT 400

typedef struct Chunk
{

  unsigned char data[CHUNK_WIDTH * Chunk_HEIGHT];

  struct Chunk *left;
  struct Chunk *right;
  struct Chunk *up;
  struct Chunk *down;

} Chunk;

#endif