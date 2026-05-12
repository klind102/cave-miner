#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "player.h"
#include "linmath.h"

#define CAMERA_ZOOM_SPEED 100.0f

mat4x4 camera;

vec2 cameraPos = {0.0f, 0.0f};
float zoom = 0.0f;
float zoom = 1.0f;

void updatePlayer(GLFWwindow *window, float deltaTime)
{
  int windowWidth, windowHeight;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);

  vec3 input = {
      glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S),
      glfwGetKey(window, GLFW_KEY_A) - glfwGetKey(window, GLFW_KEY_D),
      glfwGetKey(window, GLFW_KEY_Q) - glfwGetKey(window, GLFW_KEY_E),
  };
  
  zoom += input[2] * CAMERA_ZOOM_SPEED * deltaTime;
  if (zoom < 0.1f) zoom = 0.1f;

  float boundsX = (windowWidth / 2.0f) / zoom;
  float boundsY = (windowHeight / 2.0f) / zoom;

  mat4x4_ortho(camera, -boundsX, boundsX, -boundsY, boundsY, -0.1f, 100.0f);
}