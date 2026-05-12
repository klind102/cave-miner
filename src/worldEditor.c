#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <worldEditor.h>
#include <world.h>

#define cell(x, y) chunkData[(x) + (y) * CHUNK_WIDTH]
void worldEditor_paint(GLFWwindow *window, unsigned char *chunkData)
{

  double screenX, screenY;
  int windowWidth, windowHeight;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);
  glfwGetCursorPos(window, &screenX, &screenY);

  for (int i = 0; i < BRUSH_SIZE; i++)
    for (int j = 0; j < BRUSH_SIZE; j++)
    {
      int worldX = clamp((screenX / windowWidth * CHUNK_WIDTH) + i, 1, CHUNK_WIDTH - 2);
      int worldY = clamp(((1 - (screenY / windowHeight)) * Chunk_HEIGHT) + j, 1, Chunk_HEIGHT - 2);

      cell(worldX, worldY) = SAND;
    }
}



#undef cell
