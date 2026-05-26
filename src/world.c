#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <linmath.h>
#include <world.h>
#include <materials.h>
#include <utils.h>

unsigned int shaderProgram;
unsigned int texture;

int updateOrder[CHUNK_WIDTH * CHUNK_HEIGHT];
Chunk *world_rootChunk;
int numLoadedChunks = 0;

Chunk *world_genChunk(int x, int y)
{

  if (numLoadedChunks >= MAX_NUM_CHUNKS)
  {
    printf("max number of loaded chunks exceded!");
    return NULL;
  }
  Chunk *chunk = malloc(sizeof(Chunk));

  chunk->xpos = x;
  chunk->ypos = y;
  chunk->loadedID = numLoadedChunks;
  for (int i = 0; i < 9; i++)
    chunk->neigbors[i] = NULL;

  for (int i = 0; i < CHUNK_WIDTH * CHUNK_HEIGHT; i++)
    chunk->data[i].type = AIR;

  numLoadedChunks++;
  return chunk;
}

void world_linkChunks(Chunk *c0, Chunk *c1)
{

  int dx = c1->xpos - c0->xpos;
  int dy = c1->ypos - c0->ypos;

  if (!(dx || dy) || dx > 1 || dx < -1 || dy > 1 || dy < -1)
  {
    printf("Cannot link chunks (invalid offset)\n");
    return;
  }

  c0->neigbors[(dx + 1) + (dy + 1) * 3] = c1;
  c1->neigbors[(-dx + 1) + (-dy + 1) * 3] = c0;
}

void world_init()
{
  const char *vertexShaderSource = LoadShaderSource("assets/world-vert-shader.vert");
  const char *fragmentShaderSource = LoadShaderSource("assets/world-frag-shader.frag");

  if (!vertexShaderSource || !fragmentShaderSource)
  {
    fprintf(stderr, "ERROR: Failed to load shader sources\n");
    return;
  }

  unsigned int vertexShader, fragmentShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

  glCompileShader(vertexShader);
  glCompileShader(fragmentShader);

  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  free((void *)vertexShaderSource);
  free((void *)fragmentShaderSource);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_3D, texture);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RG8, CHUNK_WIDTH, CHUNK_HEIGHT, MAX_NUM_CHUNKS, 0, GL_RG, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  for (int i = 0; i < CHUNK_WIDTH * CHUNK_HEIGHT; i++)
    updateOrder[i] = i;

  for (int i = CHUNK_WIDTH * CHUNK_HEIGHT - 1; i > 0; i--)
  {
    int j = rand() % (i + 1);
    int tmp = updateOrder[i];
    updateOrder[i] = updateOrder[j];
    updateOrder[j] = tmp;
  }

  world_rootChunk = world_genChunk(0, 0);
}

void world_simulateChunk(Chunk *chunk)
{
  int rx = rand();
  int ry = rand();

  for (int i = 0; i < CHUNK_WIDTH * CHUNK_HEIGHT; i++)
  {
    int x = (updateOrder[i] + rx) % CHUNK_WIDTH;
    int y = ((updateOrder[i] + ry) / CHUNK_WIDTH) % CHUNK_HEIGHT;
    materialMove(x, y, chunk);
  }

  glBindTexture(GL_TEXTURE_3D, texture);

  glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, chunk->loadedID, CHUNK_WIDTH, CHUNK_HEIGHT, 1, GL_RG, GL_UNSIGNED_BYTE, chunk->data);
}

void world_drawChunk(Chunk *chunk, mat4x4 camera)
{
  glUseProgram(shaderProgram);
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "camera"), 1, GL_FALSE, (float *)camera);
  glUniform2f(glGetUniformLocation(shaderProgram, "chunkPos"), 2 * (float)chunk->xpos, 2 * (float)chunk->ypos);
  glUniform1f(glGetUniformLocation(shaderProgram, "chunkSlice"), (chunk->loadedID + 0.5f) / (float)MAX_NUM_CHUNKS);

  glBindVertexArray(UTIL_SPRITE_VAO);
  glBindTexture(GL_TEXTURE_3D, texture);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void world_freeAll()
{
  glDeleteTextures(1, &texture);
  glDeleteProgram(shaderProgram);
}
