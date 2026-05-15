#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "player.h"
#include "linmath.h"
#include <stdlib.h>

Player* player_init()
{
  Player* player = malloc(sizeof(Player));
  mat4x4_identity(player->camera_transform);
  player->camera_pos[0] = 0.0f;
  player->camera_pos[1] = 0.0f;
  player->camera_zoom = 1.0f;

  return player;
};

void player_updateCamera(GLFWwindow *window, Player *player, float deltaTime)
{
  int windowWidth, windowHeight;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);

  vec3 input = {
      glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S),
      glfwGetKey(window, GLFW_KEY_A) - glfwGetKey(window, GLFW_KEY_D),
      glfwGetKey(window, GLFW_KEY_Q) - glfwGetKey(window, GLFW_KEY_E),
  };

  player->camera_zoom += input[2] * CAMERA_ZOOM_SPEED * player->camera_zoom * deltaTime;
  if (player->camera_zoom < 0.1f)
    player->camera_zoom = 0.1f;

  float boundsX = (windowWidth / 2.0f) / player->camera_zoom;
  float boundsY = (windowHeight / 2.0f) / player->camera_zoom;

  mat4x4_ortho(player->camera_transform, -boundsX, boundsX, -boundsY, boundsY, -0.1f, 100.0f);
}