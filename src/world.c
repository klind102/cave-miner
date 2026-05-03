#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "materials.h"

unsigned int vertexArray, shaderProgram, texture;

#define WORLD_WIDTH 100
#define WORLD_HEIGHT 100
unsigned char worldData[WORLD_WIDTH * WORLD_HEIGHT];

#define cell(x, y) worldData[x + y * WORLD_WIDTH]

void initWorld()
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

  // glDeleteBuffers(1, &vertexBuffer);
  // glDeleteBuffers(1, &indexBuffer);

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

  for (int x = 0; x < WORLD_WIDTH; x++)
  {
    for (int y = 0; y < WORLD_HEIGHT; y++)
    {
      if (x == WORLD_WIDTH - 1 || y == WORLD_HEIGHT - 1 || x == 0 || y == 0)
        cell(x, y) = STONE;
      else
        cell(x, y) = AIR;
    }
  }

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WORLD_WIDTH, WORLD_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, worldData);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glGenerateMipmap(GL_TEXTURE_2D);
};

void SimulateWorld()
{
  int rx = WORLD_WIDTH / 2;
  int ry = WORLD_HEIGHT / 2;

  cell(rx, ry) = SAND;

  for (int i = 0; i < 1000; i++)
  {
    int x = rand() % WORLD_WIDTH;
    int y = rand() % WORLD_HEIGHT;
    materialMove(x, y, WORLD_WIDTH, WORLD_HEIGHT, worldData);
  }

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WORLD_WIDTH, WORLD_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, worldData);
};

void drawWorld()
{
  glUseProgram(shaderProgram);
  glBindVertexArray(vertexArray);
  glBindTexture(GL_TEXTURE_2D, texture);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
