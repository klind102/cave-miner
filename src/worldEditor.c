#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <materials.h>
#include <worldEditor.h>
#include <world.h>

#define clamp(x, a, b) ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))
void EditorPaint(GLFWwindow *window)
{

  double screenX, screenY;
  int windowWidth, windowHeight;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);
  glfwGetCursorPos(window, &screenX, &screenY);

  for (int i = 0; i < BRUSH_SIZE; i++)
    for (int j = 0; j < BRUSH_SIZE; j++)
    {
      int worldX = clamp((screenX / windowWidth * WORLD_WIDTH) + i, 1, WORLD_WIDTH - 2);
      int worldY = clamp(((1 - (screenY / windowHeight)) * WORLD_HEIGHT) + j, 1, WORLD_HEIGHT - 2);

      getWorldData()[worldX + worldY * WORLD_WIDTH] = SAND;
    }
}

#undef clamp
