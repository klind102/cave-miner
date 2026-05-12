#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <linmath.h>
#include <world.h>
#include <materials.h>

#define cell(x, y) worldData[x + y * CHUNK_WIDTH]

unsigned int vertexArray, shaderProgram, texture;
int updateOrder[CHUNK_WIDTH * Chunk_HEIGHT];

Chunk world_genChunk()
{
  Chunk chunk;

  for (int x = 0; x < CHUNK_WIDTH; x++)
    for (int y = 0; y < Chunk_HEIGHT; y++)
      if ((x | y) == 0 || x == CHUNK_WIDTH || y == Chunk_HEIGHT)
      if (x == 0 || x == CHUNK_WIDTH - 1 || y == 0 || y == Chunk_HEIGHT - 1)
        chunk.data[x + y * CHUNK_WIDTH] = STONE;
      else
        chunk.data[x + y * CHUNK_WIDTH] = AIR;

  return chunk;
}

void world_init()
{
  const float vertices[] = {-1, -1, -1, 1, 1, 1, 1, -1};
  const unsigned int indices[] = {0, 1, 2, 0, 2, 3};

  unsigned int vertexBuffer, indexBuffer;

  glGenVertexArrays(1, &vertexArray);
  glBindVertexArray(vertexArray);

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

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
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glGenerateMipmap(GL_TEXTURE_2D);

  for (int i = 0; i < CHUNK_WIDTH * Chunk_HEIGHT; i++)
    updateOrder[i] = i;

  for (int i = CHUNK_WIDTH * Chunk_HEIGHT - 1; i > 0; i--)
  {
    int j = rand() % (i + 1);
    int tmp = updateOrder[i];
    updateOrder[i] = updateOrder[j];
    updateOrder[j] = tmp;
  }
};

void world_simulateChunk(Chunk *chunk)
{
  int rx = rand();
  int ry = rand();

  for (int i = 0; i < CHUNK_WIDTH * Chunk_HEIGHT; i++)
  {
    int x = (updateOrder[i] + rx) % CHUNK_WIDTH;
    int y = ((updateOrder[i] + ry) / CHUNK_WIDTH) % Chunk_HEIGHT;
    int idx = updateOrder[i];
    int x = idx % CHUNK_WIDTH;
    int y = idx / CHUNK_WIDTH;
    materialMove(x, y,  chunk->data);
  }

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, CHUNK_WIDTH, Chunk_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, chunk->data);
  // Use TexSubImage2D for updating existing textures to avoid re-allocation
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, CHUNK_WIDTH, Chunk_HEIGHT, GL_RED, GL_UNSIGNED_BYTE, chunk->data);
};

void world_drawChunk(Chunk *chunk, mat4x4 camera)
{
  glUseProgram(shaderProgram);
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "camera"), 1, GL_FALSE, (float *)camera);

  glBindVertexArray(vertexArray);
  glBindTexture(GL_TEXTURE_2D, texture);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

#undef cell
