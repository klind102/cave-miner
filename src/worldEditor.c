#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <materials.h>
#include <worldEditor.h>
#include <world.h>
#include <linmath.h>

#define cell(x, y) chunk->data[(x) + (y) * CHUNK_WIDTH]
void worldEditor_paint(GLFWwindow *window, mat4x4 camera, Chunk *chunk, const Material *material)
{

  double screenX, screenY;
  int windowWidth, windowHeight;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);
  glfwGetCursorPos(window, &screenX, &screenY);

  // Convert screen coordinates to Normalized Device Coordinates
  float ndcX = (float)((2.0 * screenX / (double)windowWidth) - 1.0);
  float ndcY = (float)(1.0 - (2.0 * screenY / (double)windowHeight));

  //Un-project from clip space to world space
  mat4x4 invCamera;
  mat4x4_invert(invCamera, camera);
  vec4 ndcPos = {ndcX, ndcY, 0.0f, 1.0f};
  vec4 worldPos;
  mat4x4_mul_vec4(worldPos, invCamera, ndcPos);

  //Convert world space coordinates to chunk-relative cell coordinates.
  float localX = (worldPos[0] - (2.0f * (float)chunk->xpos - 1.0f)) / 2.0f * (float)CHUNK_WIDTH;
  float localY = (worldPos[1] - (2.0f * (float)chunk->ypos - 1.0f)) / 2.0f * (float)CHUNK_HEIGHT;

  for (int i = 0; i < BRUSH_SIZE; i++)
    for (int j = 0; j < BRUSH_SIZE; j++)
    {
      int x = clamp((int)localX + i, 1, CHUNK_WIDTH - 2);
      int y = clamp((int)localY + j, 1, CHUNK_HEIGHT - 2);

      cell(x, y) = (Cell){material->type, rand() % material->numVariants};
    }
}

#undef cell
